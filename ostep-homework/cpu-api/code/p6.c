#include <stdio.h>
#include <sys/wait.h>
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
    } else
    {
        int rc_wait = waitpid(-1, NULL, 0);
        printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n",
                rc, rc_wait, (int) getpid());
    }
    return 0;
}

/**
 * Die Routine waitpid(pid_t pid, int *wstatus, int options) kann nützlich sein,
 * wenn man nicht auf einen x-beliebigen terminierten Kindprozess warten will,
 * sondern z.B auf ein Kind mit einer bestimmten pid warten will (ob terminiert oder nicht).
 *
 *
 * Note: The call wait(&status) is equivalent to waitpid(-1, &status, 0).
 */