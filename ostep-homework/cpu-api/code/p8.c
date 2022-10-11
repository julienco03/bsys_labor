#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int fd[2];
    int nbytes;
    pid_t childpid;
    char string[]  = "Hello world\n";
    char readbuffer[20];

    pipe(fd);

    if((childpid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    if (childpid == 0)  /* Child reads from pipe */
    {
        close(fd[1]);  /* Close unused write end */
        nbytes = read(fd[0], readbuffer, sizeof(string) + 1);
        printf("Read %d bytes\n", nbytes);
        printf("Received string: %s", readbuffer);
        exit(0);
    } else  /* Parent writes string to pipe */
    {
        close(fd[0]);  /* Close unused read end */
        write(fd[1], string, strlen(string) + 1);
        wait(NULL);  /* Wait for child */
    }

    return 0;
}

/**
 * Note: pipe() creates a pipe, a unidirectional data channel that can be used
 * for interprocess communication. The array pipefd is used to return two
 * file descriptors referring to the ends of the pipe. pipefd[0] refers to the
 * read end of the pipe. pipefd[1] refers to the write end of the pipe.
 */