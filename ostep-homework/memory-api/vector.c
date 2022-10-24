#include <stdio.h>
#include <vector.h>

void _insert_(struct vector *v, int entry) {
    // if array is full then copy entries in a bigger array
    if (v->size == v->capacity-1)
    {
        v->capacity = v->capacity * 2;
        v->data = (int *) realloc(v->data, v->capacity * sizeof(int));
    }
    // insert entry at end of array
    v->data[v->size++] = entry;
}
void _delete_(struct vector *v) {
    // set last entry in array to zero
    v->data[--v->size] = 0;
}
void _free_(struct vector *v) {
    free(v->data);
    v->size = 0;
    v->capacity = 0;
}

int main(int argc, char *argv[]) {
    struct vector v = {v.data = (int *) malloc(sizeof(int)), v.size = 0, v.capacity = 1};
    _insert_(&v, 1);
    _insert_(&v, 2);
    _insert_(&v, 3);
    _insert_(&v, 4);
    _delete_(&v);
    _delete_(&v);

    printf("data[0]: %d\n", v.data[0]);
    printf("data[1]: %d\n", v.data[1]);
    printf("data[2]: %d\n", v.data[2]);
    printf("data[3]: %d\n", v.data[3]);
    printf("size: %d\n", v.size);
    printf("capacity: %d\n", v.capacity);

    _free_(&v);

    return 0;
}