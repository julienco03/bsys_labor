/**
 * linked_list.c
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

typedef struct __node_t {
    int key;
    struct __node_t *next;
    pthread_mutex_t lock;
} node_t;

typedef struct __list_t {
    node_t *head;
    pthread_mutex_t lock;
} list_t;
typedef struct __myargs_t {
    int loops;
    int mytid;
    list_t *list;
    int key;
} myargs_t;

void list_init(list_t *l) {
    l->head = NULL;
    Pthread_mutex_init(&l->lock, NULL);
}
void list_insert(list_t *l, int key) {
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL) {
        perror("malloc");
        return;
    }
    new->key = key;
    // just lock critical section
    Pthread_mutex_lock(&l->lock);
    new->next = l->head;
    l->head = new;
    Pthread_mutex_unlock(&l->lock);;
}
int list_lookup(list_t *L, int key) {
    int rv = -1;
    pthread_mutex_lock(&L->lock);
    node_t *curr = L->head;
    while (curr) {
        if (curr->key == key) {
            rv = 0;
            break;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&L->lock);
    return rv;
}
int list_length(list_t *l) {
    Pthread_mutex_lock(&l->lock);
    if (l->head == NULL) {
        Pthread_mutex_unlock(&l->lock);
        return 0;
    }
    node_t *node = l->head;
    Pthread_mutex_lock(&node->lock);
    Pthread_mutex_unlock(&l->lock);
    int i = 1;
    while (node->next) {
        Pthread_mutex_lock(&node->next->lock);
        Pthread_mutex_unlock(&node->lock);
        i++;
        node = node->next;
    }
    Pthread_mutex_unlock(&node->lock);
    return i;
}

void *build_list(void *arg) {
    myargs_t *a = (myargs_t *) arg;
    int i, key = 0;
    for (i = 0; i < a->loops; i++) {
        list_insert(a->list, key);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc <= 2) {
        fprintf(stderr, "Usage: ./main <loops> <threads>\n");
        return 1;
    }
    int loops = strtol(argv[1], NULL, 10);
    int num_threads = strtol(argv[2], NULL, 10);

    pthread_t threads[num_threads];
    struct timeval start, end;
    list_t list;
    list_init(&list);
    myargs_t args[num_threads];

    // start measurement
    int i, rc;
    rc = gettimeofday(&start, NULL);

    for (i = 0; i < num_threads; i++) {
        args[i].mytid = i;
        args[i].loops = loops;
        args[i].list = &list;
        Pthread_create(&threads[i], NULL, build_list, &args[i]);
    }
    for (i = 0; i < num_threads; i++) {
        Pthread_join(threads[i], NULL);
    }

    // stop measurement
    rc = gettimeofday(&end, NULL);
    if (rc != 0) { fprintf(stderr, "gettimeofday error\n"); return 1; }

    printf("%d thread(s) inserting a node %d times each took(s) %f seconds.\n",
            num_threads, loops, ((end.tv_sec - start.tv_sec) * 1e6 + (end.tv_usec - start.tv_usec)) / 1e6);
    printf("Expected nodes: %d\nActual nodes: %d\n",
            num_threads * loops, list_length(&list));
}
