#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        // pipe failed; exit
        fprintf(stderr, "pipe failed\n");
        exit(1);
    }

    int rc1 = fork();
    if (rc1 < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc1 == 0) {
        // child1 writes into pipe
        close(pipefd[0]);
        write(pipefd[1], "Hello, World!\n", 15);
        exit(0);
    } else {
        // parent goes down this path (original process)
        int rc2 = fork();
        if (rc2 < 0) {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        } else if (rc2 == 0) {
            // child:
            char str[15];

            // child2 reads from pipe
            close(pipefd[1]);
            read(pipefd[0], str, 15);
            printf("%s", str);
            exit(0);
        }

        // parent does not interact with pipe
        close(pipefd[0]);
        close(pipefd[1]);

        // wait for children
        wait(NULL);
        wait(NULL);
    }

    return 0;
}

/**
 * Note: pipe() creates a pipe, a unidirectional data channel that can be used
 * for interprocess communication. The array pipefd is used to return two
 * file descriptors referring to the ends of the pipe. pipefd[0] refers to the
 * read end of the pipe. pipefd[1] refers to the write end of the pipe.
 */