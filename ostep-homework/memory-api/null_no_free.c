#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int *x = (int *) malloc(sizeof(int));
    x = NULL;

    return 0;
}