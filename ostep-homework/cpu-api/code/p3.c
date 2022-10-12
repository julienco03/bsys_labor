#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    printf("hello world (pid:%d)\n", (int) getpid());


    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0)
    {
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        printf("hello\n");
    } else
    {
        sleep(1);
        printf("hello, I am parent of %d (pid:%d)\n",
                rc, (int) getpid());
        printf("goodbye\n");
    }
    return 0;
}

/**
 * Der Elternprozess ist immer(?) zuerst fertig.
 * Entweder die nicht ganz so saubere Lösung mit sleep(1), also indem man
 * im Kindprozess wartet und damit die Ausgabe von "hello" verschiebt oder
 * oder man benutzt Signale bzw. sowas wie sleep/notify.
 */