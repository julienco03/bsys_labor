#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    printf("hello world (pid:%d)\n", (int) getpid());
    close(STDOUT_FILENO);
    open("test.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);

    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0)
    {
        printf("hello, I am child (pid:%d)\n", (int) getpid());
    } else
    {
        printf("hello, I am parent of %d (pid:%d)\n",
                rc, (int) getpid());
    }
    return 0;
}

/**
 * Ja, beide können auf den Dateideskriptor zugreifen.
 * Die Reihenfolge ist nicht deterministisch.
 */