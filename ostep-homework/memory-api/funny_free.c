#include <stdio.h>
#include <stdlib.h>
#define ARR_SIZE 100

int main(int argc, char *argv[]) {
    int *data = (int *) malloc(ARR_SIZE * sizeof(int));
    free(&data[50]);

    return 0;
}