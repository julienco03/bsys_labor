#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("hello world (pid:%d)\n", (int) getpid());
    int x = 100;

    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0)
    {
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        x = 200;
        printf("%d\n", x);
    } else
    {
        printf("hello, I am parent of %d (pid:%d)\n",
                rc, (int) getpid());
        printf("%d\n", x);
    }
    return 0;
}

/**
* Die Variable x hat sowohl im Elternprozess als auch im Kindprozess den Wert 100.
* Ändert man den Wert von x im Kindprozess, bleibt der Wert von x im Elternprozess bei 100.
*/