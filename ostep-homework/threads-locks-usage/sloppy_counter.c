/**
 * sloppy_counter.c
 *
 * @author Julian Klimek
 * @version 0.1
 * @date 2022-12-20
 *
 */
#include "common_threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUMCPUS 6

typedef struct __counter_t {
    int global;                     // global count
    pthread_mutex_t glock;          // global lock
    int local[NUMCPUS];             // local count
    pthread_mutex_t llock[NUMCPUS]; // local locks
    int threshold;                  // the max value stored in local, before transfering to global
} counter_t;

void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    Pthread_mutex_init(&c->glock, NULL);  // initialize global lock
    for (int i = 0; i < NUMCPUS; i++) {
        c->local[i] = 0;  // set local counter to zero
        Pthread_mutex_init(&c->llock[i], NULL);  // initialize local locks
    }
}
void update(counter_t *c, int threadID, int amt) {
    int cpu = threadID % NUMCPUS;
    Pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold) {  // when a local counter has exceeded the threshold...
        Pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];  // add local to global
        Pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;  // reset local counter
    }
    Pthread_mutex_unlock(&c->llock[cpu]);
}

int get(counter_t *c) {
    Pthread_mutex_lock(&c->glock);
    int val = c->global;
    Pthread_mutex_unlock(&c->glock);
    return val;
}

typedef struct __myargs_t {
    int loops;
    counter_t *counter;
    int mytid;
} myargs_t;

void *worker(void *arg) {
    myargs_t *a = (myargs_t *) arg;
    int i;
    for (i = 0; i < a->loops; i++) {
        update(a->counter, a->mytid, 1);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc <= 3) {
        fprintf(stderr, "Usage: ./main <loops> <threads> <threshold>\n");
        return 1;
    }

    int loops, num_threads, threshold;
    loops = strtol(argv[1], NULL, 10);
    num_threads = strtol(argv[2], NULL, 10);
    threshold = strtol(argv[3], NULL, 10);

    pthread_t threads[num_threads];
    struct timeval start, end;
    counter_t counter;
    init(&counter, threshold);
    myargs_t args[num_threads];

    // start measurement
    int i, rc;
    rc = gettimeofday(&start, NULL);
    if (rc != 0) { fprintf(stderr, "gettimeofday error\n"); return 1; }

    for (i = 0; i < num_threads; i++) {
        args[i].mytid = i;
        args[i].loops = loops;
        args[i].counter = &counter;
        Pthread_create(&threads[i], NULL, worker, &args[i]);
    }
    for (i = 0; i < num_threads; i++) {
        Pthread_join(threads[i], NULL);
    }

    // stop measurement
    rc = gettimeofday(&end, NULL);
    if (rc != 0) { fprintf(stderr, "gettimeofday error\n"); return 1; }

    printf("%d thread(s) incrementing a sloppy counter %d times each took(s) %f seconds.\n",
            num_threads, loops, ((end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec)) / 1e6);

    printf("Expected value: %d\nActual value: %d\n",
            loops * num_threads, get(&counter));
}
