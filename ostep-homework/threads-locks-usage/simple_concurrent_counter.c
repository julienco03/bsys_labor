/**
 * simple_concurrent_counter.c
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

typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;
typedef struct __myargs_t {
    int loops;
    counter_t *counter;
} myargs_t;

void init(counter_t *c) {
    c->value = 0;
    Pthread_mutex_init(&c->lock, NULL);
}
void increment(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value++;
    Pthread_mutex_unlock(&c->lock);
}
int get(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    int res = c->value;
    Pthread_mutex_unlock(&c->lock);
    return res;
}

void *worker(void *arg) {
    myargs_t *a = (myargs_t *) arg;
    for (int i = 0; i < a->loops; i++) {
        increment(a->counter);
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc <= 2) {
        fprintf(stderr, "Usage: ./simple_concurrent_counter <loops> <threads>\n");
        return 1;
    }

    int loops, num_threads;
    loops = strtol(argv[1], NULL, 10);
    num_threads = strtol(argv[2], NULL, 10);

    pthread_t threads[num_threads];
    struct timeval start, end;
    counter_t counter;
    init(&counter);
    myargs_t args;
    args.loops = loops;
    args.counter = &counter;

    // start measurement
    int rc, i;
    rc = gettimeofday(&start, NULL);
    if (rc != 0) { fprintf(stderr, "gettimeofday error\n"); return 1; }

    for (i = 0; i < num_threads; i++) {
        Pthread_create(&threads[i], NULL, worker, &args);
    }
    for (i = 0; i < num_threads; i++) {
        Pthread_join(threads[i], NULL);
    }

    // stop measurement
    rc = gettimeofday(&end, NULL);
    if (rc != 0) { fprintf(stderr, "gettimeofday error\n"); return 1; }

    printf("%d thread(s) took(s) %.2f microseconds to increment a locked counter %d times concurrently\n",
            num_threads, (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec), loops * num_threads);
    printf("Expected value: %d\nActual value: %d\n",
            loops * num_threads, get(&counter));
}
