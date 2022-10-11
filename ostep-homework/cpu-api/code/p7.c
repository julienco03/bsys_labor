#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid());

    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0)
    {
        close(STDOUT_FILENO);
        printf("hello, I am child (pid:%d)\n", (int) getpid());
    } else
    {
        printf("hello, I am parent of %d (pid:%d)\n",
                rc, (int) getpid());
    }
    return 0;
}

/**
 * Schließt man die Standardausgabe und ruft anschließend prinft() im Kindprozess auf
 * wird nichts ausgegeben.
 */