#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include "queue.h"

struct queue {
    int size;
    void **list;
    int front;
    int back;
    pthread_mutex_t *front_mutex;
    pthread_mutex_t *back_mutex;
    sem_t *elem_available_sem;
    sem_t *slot_available_sem;
};

queue_t *queue_new(int size) {
    queue_t *q = calloc(1, sizeof(queue_t));
    q->size = size;
    q->list = calloc(size, sizeof(void *));
    q->front = 0;
    q->back = 0;

    q->front_mutex = calloc(1, sizeof(pthread_mutex_t));
    pthread_mutex_init(q->front_mutex, NULL);

    q->back_mutex = calloc(1, sizeof(pthread_mutex_t));
    pthread_mutex_init(q->back_mutex, NULL);

    q->elem_available_sem = calloc(1, sizeof(sem_t));
    sem_init(q->elem_available_sem, 0, 0);

    q->slot_available_sem = calloc(1, sizeof(sem_t));
    sem_init(q->slot_available_sem, 0, size);

    return q;
}

void queue_delete(queue_t **q) {
    queue_t *p = *q;

    sem_destroy(p->slot_available_sem);
    free(p->slot_available_sem);
    p->slot_available_sem = NULL;

    sem_destroy(p->elem_available_sem);
    free(p->elem_available_sem);
    p->elem_available_sem = NULL;

    pthread_mutex_destroy(p->back_mutex);
    free(p->back_mutex);
    p->back_mutex = NULL;

    pthread_mutex_destroy(p->front_mutex);
    free(p->front_mutex);
    p->front_mutex = NULL;

    free(p->list);
    p->list = NULL;

    free(p);
    *q = NULL;
}

bool queue_push(queue_t *q, void *elem) {
    if (q == NULL) {
        return false;
    }

    sem_wait(q->slot_available_sem);
    pthread_mutex_lock(q->back_mutex);

    q->list[q->back] = elem;
    q->back = (q->back + 1) % (q->size);

    pthread_mutex_unlock(q->back_mutex);
    sem_post(q->elem_available_sem);

    return true;
}

bool queue_pop(queue_t *q, void **elem) {
    if (q == NULL) {
        return false;
    }

    sem_wait(q->elem_available_sem);
    pthread_mutex_lock(q->front_mutex);

    *elem = q->list[q->front];
    q->front = (q->front + 1) % (q->size);

    pthread_mutex_unlock(q->front_mutex);
    sem_post(q->slot_available_sem);

    return true;
}
