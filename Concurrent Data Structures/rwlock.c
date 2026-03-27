#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "rwlock.h"

struct rwlock {
    PRIORITY priority;
    int N;
    int active_readers;
    int active_writers;
    int waiting_readers;
    int waiting_writers;
    int readers_since_last_write;
    pthread_mutex_t *mutex;
    pthread_cond_t *condition;
};

rwlock_t *rwlock_new(PRIORITY p, uint32_t n) {
    rwlock_t *rw = calloc(1, sizeof(rwlock_t));

    rw->priority = p;
    rw->N = n;
    rw->active_readers = 0;
    rw->active_writers = 0;
    rw->waiting_readers = 0;
    rw->waiting_writers = 0;
    rw->readers_since_last_write = 0;

    rw->mutex = calloc(1, sizeof(pthread_mutex_t));
    pthread_mutex_init(rw->mutex, NULL);

    rw->condition = calloc(1, sizeof(pthread_cond_t));
    pthread_cond_init(rw->condition, NULL);

    return rw;
}

void rwlock_delete(rwlock_t **rw) {
    rwlock_t *rwp = *rw;

    pthread_cond_destroy(rwp->condition);
    free(rwp->condition);
    rwp->condition = NULL;

    pthread_mutex_destroy(rwp->mutex);
    free(rwp->mutex);
    rwp->mutex = NULL;

    free(rwp);
    *rw = NULL;
}

void reader_lock(rwlock_t *rw) {
    // acquire lock for updating rw object
    pthread_mutex_lock(rw->mutex);
    rw->waiting_readers += 1;
    switch (rw->priority) {
    case READERS:
        while (rw->active_writers > 0) {
            pthread_cond_wait(rw->condition, rw->mutex);
        }
        break;

    case WRITERS:
        while (rw->waiting_writers > 0 || rw->active_writers > 0) {
            pthread_cond_wait(rw->condition, rw->mutex);
        }
        break;

    case N_WAY:
        while (rw->active_writers > 0
               || (rw->waiting_writers > 0 && rw->readers_since_last_write >= rw->N)) {
            pthread_cond_wait(rw->condition, rw->mutex);
        }

        // rw->readers_since_last_write += 1;

        // CHANGED: only increment quota IF a writer is waiting
        if (rw->waiting_writers > 0) {
            rw->readers_since_last_write += 1;
        }
        break;
    }

    rw->waiting_readers -= 1;
    rw->active_readers += 1;

    pthread_mutex_unlock(rw->mutex);
}

void reader_unlock(rwlock_t *rw) {
    pthread_mutex_lock(rw->mutex);
    rw->active_readers -= 1;

    if (rw->waiting_readers > 0 || rw->waiting_writers > 0) {
        pthread_cond_broadcast(rw->condition);
    }

    pthread_mutex_unlock(rw->mutex);
}

void writer_lock(rwlock_t *rw) {
    // acquire lock for updating rw object
    pthread_mutex_lock(rw->mutex);
    rw->waiting_writers += 1;
    switch (rw->priority) {
    case READERS:
        while (rw->active_writers > 0 || rw->active_readers > 0 || rw->waiting_readers > 0) {
            pthread_cond_wait(rw->condition, rw->mutex);
        }
        break;

    case WRITERS:
        while (rw->active_writers > 0 || rw->active_readers > 0) {
            pthread_cond_wait(rw->condition, rw->mutex);
        }
        break;

    case N_WAY:
        while (rw->active_writers > 0 || rw->active_readers > 0
               || (rw->waiting_readers > 0 && rw->readers_since_last_write < rw->N)) {
            pthread_cond_wait(rw->condition, rw->mutex);
        }
        break;
    }

    rw->waiting_writers -= 1;
    rw->active_writers += 1;

    pthread_mutex_unlock(rw->mutex);
}

void writer_unlock(rwlock_t *rw) {
    pthread_mutex_lock(rw->mutex);
    rw->active_writers -= 1;

    if (rw->priority == N_WAY) {
        rw->readers_since_last_write = 0;
    }

    if (rw->waiting_readers > 0 || rw->waiting_writers > 0) {
        pthread_cond_broadcast(rw->condition);
    }

    pthread_mutex_unlock(rw->mutex);
}
