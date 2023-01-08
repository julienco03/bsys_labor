#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct __ns_mutex_t {
    int    queue1;
    int    queue2;
    sem_t  s1;
    sem_t  s2;
    sem_t  lock;
} ns_mutex_t;

ns_mutex_t m;

void ns_mutex_init(ns_mutex_t *m) {
    m->queue1 = 0;
    m->queue2 = 0;
    Sem_init(&m->s1, 1);
    Sem_init(&m->s2, 0);
    Sem_init(&m->lock, 1);
}

void ns_mutex_acquire(ns_mutex_t *m) {
    Sem_wait(&m->lock);
    m->queue1++;
    Sem_post(&m->lock);

    Sem_wait(&m->s1);
    m->queue2++;
    Sem_wait(&m->lock);
    m->queue1--;

    if (m->queue1 == 0) {
        Sem_post(&m->s2);
    } else {
        Sem_post(&m->s1);
    }

    Sem_post(&m->lock);

    /* use queue2 as a barrier */
    Sem_wait(&m->s2);
    m->queue2--;
}

void ns_mutex_release(ns_mutex_t *m) {
    if (m->queue2 == 0) {
        Sem_post(&m->s1);
    }
    else {
        Sem_post(&m->s2);
    }
}


void *worker(void *arg) {
    ns_mutex_acquire(&m);
    printf("%d is running...\n", (int) pthread_self());
    ns_mutex_release(&m);
    return NULL;
}

int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    pthread_t p[num_threads];

    printf("parent: begin\n");

    ns_mutex_init(&m);
    for (int i = 0; i < num_threads; i++)
    Pthread_create(&p[i], NULL, worker, NULL);
    for (int i = 0; i < num_threads; i++)
    Pthread_join(p[i], NULL);

    printf("parent: end\n");

    return 0;
}

