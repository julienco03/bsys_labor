#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "usage: memory-user 'memory' 'time'\n");
        exit(1);
    }

    printf("pid: %d\n", getpid());

    clock_t start = clock();
    long double elapsed_time;

    long memory = atol(argv[1]) * 1024 * 1024;  // convert memory from megabyte to byte
    int size = memory / sizeof(int);
    int time_ = atoi(argv[2]);
    int* array = (int*) malloc(memory);

    for (;;)
    {
        elapsed_time = (double) (clock() - start) / CLOCKS_PER_SEC;
        if (elapsed_time >= time_) break;
        for (int i = 0; i < size; i++)
        {
            array[i] = 0;
        }
    }

    free(array);
    return 0;
}