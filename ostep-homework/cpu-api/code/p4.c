#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid());

    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0)
    {
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        execl("/bin/ls", "ls", "-l", NULL);
    } else
    {
        printf("hello, I am parent of %d (pid:%d)\n",
                rc, (int) getpid());
    }
    return 0;
}

/**
 * Die Menge an exec()-Varianten gibt es vermutlich aus Gründen der Effizienz,
 * da diese Routine sehr häufig vom Betriebssystem gebraucht wird.
 */