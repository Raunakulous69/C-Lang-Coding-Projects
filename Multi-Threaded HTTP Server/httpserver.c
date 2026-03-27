// Asgn 4: A simple HTTP server.
// By:  Andi Quinn

#include "connection.h"
#include "listener_socket.h"
#include "request.h"
#include "response.h"
#include "queue.h"
#include "rwlock.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/stat.h>

// #define TIMEOUT 5

// bool has_timeout(struct timeval* t) {
//     struct timeval now;
//     gettimeofday(&now, NULL);
//     double timedif = now.tv_sec - t->tv_sec + (now.tv_usec - t->tv_usec)/1000000;
//     return timedif > TIMEOUT;
// }

typedef struct job {
    int num_threads;
    int num_working;
    bool done;
    rwlock_t *job_lock;
    rwlock_t *audit_lock;
    rwlock_t **uri_lock;
    char **uri_list;
    int *uri_count;
    queue_t *job_queue;
} job_t;

job_t *job_new(int num_threads) {
    job_t *job = malloc(sizeof(job_t));
    job->num_threads = num_threads;
    job->num_working = 0;
    job->done = false;
    job->job_lock = rwlock_new(READERS, 0);
    job->audit_lock = rwlock_new(WRITERS, 0);
    job->uri_lock = calloc(num_threads, sizeof(rwlock_t *));

    for (int i = 0; i < job->num_threads; i++) {
        job->uri_lock[i] = rwlock_new(N_WAY, 2);
    }

    job->uri_list = calloc(job->num_threads, sizeof(char *));
    job->uri_count = calloc(job->num_threads, sizeof(int));

    for (int i = 0; i < job->num_threads; i++) {
        job->uri_list[i] = NULL;
        job->uri_count[i] = 0;
    }

    job->job_queue = queue_new(4096);

    return job;
}

void job_delete(job_t **p_job) {
    job_t *job = *p_job;
    queue_delete(&(job->job_queue));

    for (int i = 0; i < job->num_threads; i++) {
        if (job->uri_list[i] != NULL) {
            free(job->uri_list[i]);
            job->uri_list[i] = NULL;
        }
    }

    free(job->uri_list);
    job->uri_list = NULL;

    free(job->uri_count);
    job->uri_count = NULL;

    for (int i = 0; i < job->num_threads; i++) {
        rwlock_delete(&(job->uri_lock[i]));
    }

    free(job->uri_lock);
    job->uri_lock = NULL;

    rwlock_delete(&(job->audit_lock));
    rwlock_delete(&(job->job_lock));

    free(job);
    *p_job = NULL;
}

void job_start(job_t *job) {
    writer_lock(job->job_lock);
    job->num_working += 1;
    writer_unlock(job->job_lock);
}

void job_end(job_t *job) {
    writer_lock(job->job_lock);
    job->num_working -= 1;
    writer_unlock(job->job_lock);
}

int job_count(job_t *job) {
    reader_lock(job->job_lock);
    int num = job->num_working;
    reader_unlock(job->job_lock);
    return num;
}

void job_set_done(job_t *job) {
    writer_lock(job->job_lock);
    job->done = true;
    writer_unlock(job->job_lock);
}

bool job_is_done(job_t *job) {
    reader_lock(job->job_lock);
    bool result = job->done;
    reader_unlock(job->job_lock);
    return result;
}

void job_audit_lock(job_t *job) {
    writer_lock(job->audit_lock);
}

void job_audit_unlock(job_t *job) {
    writer_unlock(job->audit_lock);
}

void job_push(job_t *job, int connfd) {
    int *p = malloc(sizeof(int));
    *p = connfd;
    queue_push(job->job_queue, p);
}

int job_pop(job_t *job) {
    int *p;
    queue_pop(job->job_queue, (void **) &p);
    int connfd = *p;
    free(p);
    return connfd;
}

int job_find_uri(job_t *job, const char *uri) {
    for (int i = 0; i < job->num_threads; i++) {
        if (job->uri_list[i] != NULL) {
            if (strcmp(job->uri_list[i], uri) == 0) {
                return i;
            }
        }
    }
    return -1;
}

rwlock_t *job_get_lock(job_t *job, const char *uri) {
    int idx = job_find_uri(job, uri);
    if (idx == -1) {
        for (int i = 0; i < job->num_threads; i++) {
            if (job->uri_count[i] == 0) {
                idx = i;
                job->uri_list[idx] = strdup(uri);
                break;
            }
        }
    }
    job->uri_count[idx] += 1;
    return job->uri_lock[idx];
}

void job_return_lock(job_t *job, const char *uri) {
    int idx = job_find_uri(job, uri);
    if (idx != -1) {
        job->uri_count[idx] -= 1;
        if (job->uri_count[idx] == 0) {
            free(job->uri_list[idx]);
            job->uri_list[idx] = NULL;
        }
    }
}

void job_reader_lock_uri(job_t *job, const char *uri) {
    writer_lock(job->job_lock);
    rwlock_t *rw = job_get_lock(job, uri);
    writer_unlock(job->job_lock);
    reader_lock(rw);
}

void job_reader_unlock_uri(job_t *job, const char *uri) {
    writer_lock(job->job_lock);
    int idx = job_find_uri(job, uri);
    rwlock_t *rw = job->uri_lock[idx];
    job_return_lock(job, uri);
    reader_unlock(rw);
    writer_unlock(job->job_lock);
}

void job_writer_lock_uri(job_t *job, const char *uri) {
    writer_lock(job->job_lock);
    rwlock_t *rw = job_get_lock(job, uri);
    writer_unlock(job->job_lock);
    writer_lock(rw);
}

void job_writer_unlock_uri(job_t *job, const char *uri) {
    writer_lock(job->job_lock);
    int idx = job_find_uri(job, uri);
    rwlock_t *rw = job->uri_lock[idx];
    job_return_lock(job, uri);
    writer_unlock(rw);
    writer_unlock(job->job_lock);
}

const Response_t *handle_get(conn_t *conn) {
    char *uri = conn_get_uri(conn);
    struct stat path_stat;
    bool file_exists = (stat(uri, &path_stat) == 0);

    if (file_exists) {
        if (S_ISDIR(path_stat.st_mode)) {
            conn_send_response(conn, &RESPONSE_FORBIDDEN);
            return &RESPONSE_FORBIDDEN;
        }

        else if (access(uri, R_OK) != 0) {
            conn_send_response(conn, &RESPONSE_FORBIDDEN);
            return &RESPONSE_FORBIDDEN;
        }
    }

    else {
        conn_send_response(conn, &RESPONSE_NOT_FOUND);
        return &RESPONSE_NOT_FOUND;
    }

    int fd = open(uri, O_RDONLY, 0666); // this is fine

    if (fd == -1) {
        if (errno == EACCES) {
            conn_send_response(conn, &RESPONSE_FORBIDDEN);
            return &RESPONSE_FORBIDDEN;
        } else {
            conn_send_response(conn, &RESPONSE_INTERNAL_SERVER_ERROR);
            return &RESPONSE_INTERNAL_SERVER_ERROR;
        }
    }

    if (S_ISDIR(path_stat.st_mode)) {
        conn_send_response(conn, &RESPONSE_FORBIDDEN);
        return &RESPONSE_FORBIDDEN;
    }

    conn_send_file(conn, fd, path_stat.st_size);
    close(fd);

    return &RESPONSE_OK;
}

const Response_t *handle_put(conn_t *conn, job_t *job) {

    const Response_t *res = NULL;
    char temp_file_name[] = "_XXXXXX";
    int temp_fd = mkstemp(temp_file_name);

    if (temp_fd < 0) {
        res = &RESPONSE_INTERNAL_SERVER_ERROR;
        goto out;
    }

    // receive the file
    res = conn_recv_file(conn, temp_fd); // conn --> fd, NULL return is good
    close(temp_fd);

    // Check if file already exists
    char *uri = conn_get_uri(conn);
    job_writer_lock_uri(job, uri);
    bool existed = access(uri, F_OK) == 0;

    // renaming temporary file to target uri
    int rc = rename(temp_file_name, uri);

    job_writer_unlock_uri(job, uri);

    if (rc == -1) {
        unlink(temp_file_name);
        if (errno == EACCES || errno == EISDIR || errno == ENOENT) {
            res = &RESPONSE_FORBIDDEN;
            goto out;
        } else {
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            goto out;
        }
    }

    // check to see if it was OK.
    if (res == NULL) {
        if (existed) {
            res = &RESPONSE_OK;
        } else {
            res = &RESPONSE_CREATED;
        }
    }

out:
    conn_send_response(conn, res);
    return res;
}

// const Response_t *handle_put(conn_t *conn) {

//     char *uri = conn_get_uri(conn);

//     const Response_t *res = NULL;

//     // Check if file already exists before opening it.
//     bool existed = access(uri, F_OK) == 0;

//     // Open the file
//     int fd = open(uri, O_CREAT | O_WRONLY | O_TRUNC, 0600);

//     if (fd < 0) {
//         if (errno == EACCES || errno == EISDIR || errno == ENOENT) {
//             res = &RESPONSE_FORBIDDEN;
//             goto out;
//         } else {
//             res = &RESPONSE_INTERNAL_SERVER_ERROR;
//             goto out;
//         }
//     }

//     /* fd is valid */
//     // receive the file
//     res = conn_recv_file(conn, fd); // conn --> fd, NULL return is good

//     // check to see if it was OK.
//     if (res == NULL) {
//         if (existed) {
//             res = &RESPONSE_OK;
//         } else {
//             res = &RESPONSE_CREATED;
//         }
//     }

//     close(fd);

// out:
//     conn_send_response(conn, res);
//     return res;
// }

void *handle_request(void *arg) {
    job_t *job = (job_t *) arg;

    while (!job_is_done(job)) {
        int connfd = job_pop(job);
        job_start(job);
        conn_t *conn = conn_new(connfd);
        const Response_t *res = conn_parse(conn);
        const Request_t *req = conn_get_request(conn);
        char *uri = conn_get_uri(conn);
        char *req_id = conn_get_header(conn, "Request-Id");

        if (req_id == NULL) {
            req_id = "0";
        }

        if (res != NULL) {
            conn_send_response(conn, res);
        }

        else {

            if (req == &REQUEST_PUT) {
                // job_writer_lock_uri(job, uri);
                res = handle_put(conn, job);
                // job_writer_unlock_uri(job, uri);
            }

            else if (req == &REQUEST_GET) {
                job_reader_lock_uri(job, uri);
                res = handle_get(conn);
                job_reader_unlock_uri(job, uri);
            }

            else {
                res = conn_send_response(conn, &RESPONSE_NOT_IMPLEMENTED);
            }
        }

        job_audit_lock(job);
        fprintf(stderr, "%s,%s,%d,%s\n", request_get_str(req), uri, response_get_code(res), req_id);
        job_audit_unlock(job);

        conn_delete(&conn);
        close(connfd);
        job_end(job);
    }

    return NULL;
}

#define OPTIONS     "t:"
#define NUM_THREADS 4

job_t *job;

int main(int argc, char **argv) {
    int port;
    int num_threads = NUM_THREADS;
    int opt = 0;

    // reading the number of thread options
    char *endptr = NULL;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't':
            num_threads = (size_t) strtoull(optarg, &endptr, 10);
            if (endptr && *endptr != '\0') {
                warnx("Invalid thread count: %s\n", optarg);
                return EXIT_FAILURE;
            }
            break;

        default: fprintf(stderr, "Invalid Option\n"); return EXIT_FAILURE;
        }
    }

    if ((argc - optind) == 1) {
        port = (size_t) strtoull(argv[optind], &endptr, 10);
        if (endptr && *endptr != '\0') {
            warnx("Invalid Port Number: %s\n", argv[optind]);
            return EXIT_FAILURE;
        }
    }

    else {
        fprintf(stderr, "Missing Port Number\n");
        return EXIT_FAILURE;
    }

    signal(SIGPIPE, SIG_IGN);

    Listener_Socket_t *sock = ls_new(port);
    if (!sock) {
        warnx("cannot open socket");
        return EXIT_FAILURE;
    }

    // creating the queue to hold the requests
    job = job_new(num_threads);

    // creating the worker threads
    for (int i = 0; i < (int) num_threads; i++) {
        pthread_t thread;
        int rc = pthread_create(&thread, NULL, handle_request, job);

        if (rc != 0) {
            fprintf(stderr, "Failed to Create Thread\n");
            job_delete(&job);
            return EXIT_FAILURE;
        }
    }

    while (1) {
        int connfd = ls_accept(sock);
        job_push(job, connfd);
    }
    ls_delete(&sock);

    return EXIT_SUCCESS;
}
