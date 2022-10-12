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
        //int rc_wait = wait(NULL);
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        //printf("(rc_wait:%d)\n", rc_wait);
    } else
    {
        int rc_wait = wait(NULL);
        printf("hello, I am parent of %d (pid:%d)\n",
                rc, (int) getpid());
    }
    return 0;
}

/**
 * wait() gibt die PID vom Kindprozess zurück.
 * Im Kindprozess gibt wait() -1 zurück, d.h. einen Fehler, da das Kind keine Kindprozess hat.
 */