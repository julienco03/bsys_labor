#include <stdlib.h>
#ifndef VECTOR_H
#define VECTOR_H

struct vector {
    int *data;
    int size;
    int capacity;
};

void _insert_(struct vector *v, int entry);
void _delete_(struct vector *v);
void _free_(struct vector *v);

#endif
