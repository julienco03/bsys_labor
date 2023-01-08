#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.

typedef struct __barrier_t {
   sem_t    *lock;
   sem_t    *sem;
   int      num_arrived;
   int      num_threads;
} barrier_t;


// the single barrier we are using for this program
barrier_t b;

void barrier_init(barrier_t *b, int num_threads) {
    b->lock = malloc(sizeof(sem_t));
    b->sem = malloc(sizeof(sem_t));
    Sem_init(b->lock, 1);
    Sem_init(b->sem, 0);
    b->num_arrived = 0;
    b->num_threads = num_threads;
}

void barrier(barrier_t *b) {
    Sem_wait(b->lock);
    b->num_arrived++;
    if (b->num_arrived == b->num_threads) {
        Sem_post(b->sem);
    }
    Sem_post(b->lock);
    Sem_wait(b->sem);
    Sem_post(b->sem);
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}


// run with a single argument indicating the number of
// threads you wish to create (1 or more)
int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);

    int i;
    for (i = 0; i < num_threads; i++) {
	t[i].thread_id = i;
	Pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++)
	Pthread_join(p[i], NULL);

    printf("parent: end\n");

    free(b.lock);
    free(b.sem);
    return 0;
}

