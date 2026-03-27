/**
 * @File httpserver.c
 *
 * This file contains the main function for the HTTP server.
 *
 * @author Raunak Anwar
 */

#include "listener_socket.h"
#include "iowrapper.h"
#include "debug.h"
#include "protocol.h"

#include <arpa/inet.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <regex.h>
#include <stdbool.h>

#define BUFFER_SIZE 4096

typedef enum httpstatus {
    OK = 0,
    CREATED = 1,
    BAD_REQUEST = 2,
    FORBIDDEN = 3,
    NOT_FOUND = 4,
    INTERNAL_SERVER_ERROR = 5,
    NOT_IMPLEMENTED = 6,
    VERSION_NOT_SUPPORTED = 7
} httpstatus;

const int STATUS_CODE[] = { 200, 201, 400, 403, 404, 500, 501, 505 };

const char *STATUS_TEXT[] = { "OK", "Created", "Bad Request", "Forbidden", "Not Found",
    "Internal Server Error", "Not Implemented", "Version Not Supported" };

typedef struct httpreq {
    char method[9];
    char uri[64];
    char version[9];
    ssize_t content_length;
    httpstatus status;
    int connfd;
} httpreq;

void convertToUpper(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = toupper((unsigned char) str[i]);
        i++;
    }
}

void parse_req_line(httpreq *req, char *line) {
    regex_t rx;
    int rc = regcomp(
        &rx, "^(" TYPE_REGEX ") (" URI_REGEX ") (" HTTP_REGEX ")$", REG_EXTENDED | REG_ICASE);

    if (rc != 0) {
        req->status = INTERNAL_SERVER_ERROR;
        return;
    }

    regmatch_t pmatch[4];
    size_t nmatch = 4;

    rc = regexec(&rx, line, nmatch, pmatch, 0);
    nmatch = rx.re_nsub;
    regfree(&rx);

    if (rc == 0) {
        int n = pmatch[1].rm_eo - pmatch[1].rm_so;
        strncpy(req->method, &line[pmatch[1].rm_so], n);
        req->method[n] = '\0';

        convertToUpper(req->method);

        n = pmatch[2].rm_eo - pmatch[2].rm_so;
        strncpy(req->uri, &line[pmatch[2].rm_so + 1], n - 1);
        req->uri[n] = '\0';

        n = pmatch[3].rm_eo - pmatch[3].rm_so;
        strncpy(req->version, &line[pmatch[3].rm_so], n);
        req->version[n] = '\0';
    }

    else if (rc == REG_NOMATCH) {
        req->status = BAD_REQUEST;
    }

    else {
        req->status = INTERNAL_SERVER_ERROR;
    }
}

void parse_header_field(httpreq *req, char *line) {
    char field[129] = { 0 };
    char value[129] = { 0 };

    regex_t rx;
    int rc = regcomp(&rx, "(" HEADER_FIELD_REGEX "): (" HEADER_VALUE_REGEX ")", REG_EXTENDED);

    if (rc != 0) {
        req->status = INTERNAL_SERVER_ERROR;
        return;
    }

    regmatch_t pmatch[3];
    size_t nmatch = 3;

    rc = regexec(&rx, line, nmatch, pmatch, 0);
    nmatch = rx.re_nsub;
    regfree(&rx);

    if (rc == 0) {
        int n = pmatch[1].rm_eo - pmatch[1].rm_so;
        strncpy(field, &line[pmatch[1].rm_so], n);
        field[n] = '\0';

        convertToUpper(field);
        if (strcmp(field, "CONTENT-LENGTH") == 0) {
            n = pmatch[2].rm_eo - pmatch[2].rm_so;
            strncpy(value, &line[pmatch[2].rm_so], n);
            value[n] = '\0';

            for (int i = 0; i < n; i++) {
                if (value[i] < '0' || value[i] > '9') {
                    req->status = BAD_REQUEST;
                    return;
                }
            }

            req->content_length = strtoul(value, NULL, 10);

            if (errno == ERANGE) {
                req->status = BAD_REQUEST;
                return;
            }
        }
    }

    else if (rc == REG_NOMATCH) {
        req->status = BAD_REQUEST;
    }

    else {
        req->status = INTERNAL_SERVER_ERROR;
    }
}

void parse_req(httpreq *req, char *buffer) {
    // extract the request line
    char *line = buffer;
    char *endptr = strstr(line, "\r\n");

    *endptr = '\0';

    parse_req_line(req, line);

    if (req->status != OK) {
        return;
    }

    // point to the start of the next line
    line = endptr + sizeof(char) * 2;
    while ((endptr = strstr(line, "\r\n")) != NULL) {
        *endptr = '\0';
        parse_header_field(req, line);

        if (req->status != OK) {
            return;
        }

        line = endptr + 2 * sizeof(char);
    }
}

void validate_req(httpreq *req) {
    // validating the method
    if (strcmp(req->method, "GET") != 0 && strcmp(req->method, "PUT") != 0) {
        req->status = NOT_IMPLEMENTED;
        return;
    }

    // validating the protocol
    if (strcmp(req->version, "HTTP/1.1") != 0) {
        req->status = VERSION_NOT_SUPPORTED;
        return;
    }

    // validating the URI
    struct stat fileinfo;
    bool file_exists = (stat(req->uri, &fileinfo) == 0);

    // // added this in
    // if (S_ISDIR(fileinfo.st_mode)) {
    //     req->status = FORBIDDEN;
    // }

    if (strcmp(req->method, "GET") == 0) {
        if (file_exists) {
            if (S_ISDIR(fileinfo.st_mode)) {
                req->status = FORBIDDEN;
            }

            else if (access(req->uri, R_OK) != 0) {
                req->status = FORBIDDEN; // not found to forbidden
            }

            else {
                req->content_length = fileinfo.st_size;
            }
        }

        else {
            req->status = NOT_FOUND;
        }

    }

    else {
        if (req->content_length == -1) {
            req->status = BAD_REQUEST;
        } else if (file_exists) {
            if (S_ISDIR(fileinfo.st_mode)) {
                req->status = FORBIDDEN; // editing
            }

            else if (access(req->uri, W_OK) != 0) {
                req->status = FORBIDDEN;
            }
        }

        // else if (!file_exists) {
        //     char curr_dir[_PC_PATH_MAX];
        //     if (getcwd(curr_dir, sizeof(curr_dir)) != NULL) {
        //         if (access(curr_dir, W_OK) != 0) {
        //             req->status = FORBIDDEN;
        //         }
        //     }
        // }
    }

    return;
}

void handle_err(httpreq *req) {
    char response[BUFFER_SIZE] = { 0 };
    char message_body[256] = { 0 };

    sprintf(message_body, "%s\n", STATUS_TEXT[req->status]);
    int content_length = strlen(message_body);

    sprintf(response,
        "HTTP/1.1 %d %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        STATUS_CODE[req->status], STATUS_TEXT[req->status], content_length, message_body);

    write_n_bytes(req->connfd, response, strlen(response));
}

void handle_get(httpreq *req) {
    int fd = open(req->uri, O_RDONLY, 0666); // this is fine
    // if (fd == -1) {
    //     req->status = NOT_FOUND; //changed this from Internal error server to not found (should be 404)
    //     handle_err(req);
    //     return;
    // }

    if (fd == -1) {
        if (errno == EACCES) {
            req->status = FORBIDDEN;
        } else {
            req->status = INTERNAL_SERVER_ERROR;
        }
        handle_err(req);
        return;
    }

    // added this
    struct stat path_stat;
    stat(req->uri, &path_stat);
    if (S_ISDIR(path_stat.st_mode)) {
        req->status = FORBIDDEN; // 403 error
    }

    char response[128] = { 0 };
    sprintf(response,
        "HTTP/1.1 %d %s\r\n"
        "Content-Length: %lu\r\n"
        "\r\n",
        STATUS_CODE[req->status], STATUS_TEXT[req->status], req->content_length);

    write_n_bytes(req->connfd, response, strlen(response));
    pass_n_bytes(fd, req->connfd, req->content_length);

    close(fd);
}

void handle_put(httpreq *req, char *bodyptr, ssize_t body_length) {
    struct stat fileinfo;
    bool file_exists = (stat(req->uri, &fileinfo) == 0);
    int fd = open(req->uri, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    // if (fd == -1) {
    //     req->status = NOT_FOUND; //changed this from Internal error server to not found
    //     handle_err(req);
    //     return;
    // }

    if (fd == -1) {
        if (errno == EACCES || errno == EISDIR) {
            req->status = FORBIDDEN;
        } else {
            req->status = INTERNAL_SERVER_ERROR;
        }
        handle_err(req);
        return;
    }

    // added this
    struct stat path_stat;
    stat(req->uri, &path_stat);
    if (S_ISDIR(path_stat.st_mode)) {
        req->status = FORBIDDEN;
    }

    if (body_length > 0) {
        if (body_length > req->content_length) {
            body_length = req->content_length;
        }

        int rc = write_n_bytes(fd, bodyptr, body_length);

        if (rc == -1) {
            req->status = INTERNAL_SERVER_ERROR;
            close(fd);
            handle_err(req);
            return;
        }
    }

    ssize_t bytes_to_pass = req->content_length - body_length;
    ssize_t rc = pass_n_bytes(req->connfd, fd, bytes_to_pass);

    if (rc == -1) {
        req->status = INTERNAL_SERVER_ERROR;
        close(fd);
        handle_err(req);
        return;
    }

    char response[128] = { 0 };
    char message_body[256] = { 0 };

    httpstatus status = file_exists ? OK : CREATED;

    sprintf(message_body, "%s\n", STATUS_TEXT[status]);
    int content_length = strlen(message_body);

    sprintf(response,
        "HTTP/1.1 %d %s\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        STATUS_CODE[status], STATUS_TEXT[status], content_length, message_body);

    write_n_bytes(req->connfd, response, strlen(response));
    close(fd);
}

/** @brief Handles a connection from a client.
 *
 *  @param connfd The file descriptor for the connection.
 *
 *  @return void
 */
void handle_connection(int connfd) {
    char buffer[BUFFER_SIZE] = { 0 };
    char *endptr = NULL;
    int bytes_read = 0;
    int rc = 0;
    ssize_t body_length = 0; // mumber of extra bytes after request header is done
    httpreq req = { 0 };
    req.status = OK;
    req.connfd = connfd;
    req.content_length = -1;

    while (endptr == NULL && bytes_read < MAX_HEADER_LEN) {
        rc = recv(connfd, &buffer[bytes_read], MAX_HEADER_LEN - bytes_read, MSG_DONTWAIT);
        if (rc > 0) {
            endptr = strstr(buffer, "\r\n\r\n"); // looking for this phrase inside buffer
            bytes_read += rc;
            if (endptr != NULL) {
                endptr[2] = '\0';
                body_length = bytes_read - strlen(buffer) - 2;
            }
        }

        else if (rc == 0) {
            req.status = BAD_REQUEST;
            break;
        }

        else {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                req.status = BAD_REQUEST;
                break;
            }
        }
    }

    if (endptr == NULL) {
        req.status = BAD_REQUEST;
    }

    if (req.status == OK) {
        parse_req(&req, buffer); //make function for this
    }

    if (req.status == OK) {
        validate_req(&req); // make function for this
    }

    if (req.status == OK) {
        convertToUpper(req.method);
        if (strcmp(req.method, "GET") == 0) {
            handle_get(&req); // make function for this
        }

        else {
            char *bodyptr = endptr + 4 * sizeof(char);

            if (body_length < 0) {
                body_length = 0;
            }

            handle_put(&req, bodyptr, body_length); // make function for this
        }
    }

    else {
        handle_err(&req); // make function for this
    }

    close(connfd);
    return;
}

/** @brief Main function for the HTTP server.
 *
 *  @param argc The number of arguments.
 *  @param argv The arguments.
 *
 *  @return EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int main(int argc, char **argv) {
    if (argc < 2) {
        warnx("wrong arguments: %s port_num", argv[0]);
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *endptr = NULL;
    size_t port = (size_t) strtoull(argv[1], &endptr, 10);

    /* Add error checking for the port number */

    signal(SIGPIPE, SIG_IGN);
    Listener_Socket_t *sock;
    sock = ls_new(port);

    while (1) {
        int connfd = ls_accept(sock);
        handle_connection(connfd);
    }

    return EXIT_SUCCESS;
}
