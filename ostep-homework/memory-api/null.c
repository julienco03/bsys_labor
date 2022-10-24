#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int *x = (int *) malloc(sizeof(int));
    x = NULL;
    printf("x: %d\n", *x);
    free(x);

    return 0;
}