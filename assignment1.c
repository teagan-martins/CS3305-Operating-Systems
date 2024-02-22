#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        printf("Fork Unsuccessful");
        exit(1);
    }

    if (pid > 0) {
        printf("parent (PID %d): process started\n\n", getpid());
        printf("parent (PID %d): forking child_1\n", getpid());
        printf("parent (PID %d): fork successful for child_1 (PID %d)\n", getpid(), pid);
        printf("parent (PID %d): waiting for child_1 (PID %d) to complete\n", getpid(), pid);


        int status;
        wait(&status);

        pid = fork();

        if (pid == 0) {
            printf("parent (PID %d): forking child_2\n", getppid());
            printf("parent (PID %d): fork successful for child_2 (PID %d)\n", getppid(), getpid());
            printf("child_2 (PID %d): process started from parent (PID %d)\n", getpid(), getppid());
            printf("child_2 (PID %d): calling an external program [./external_program2.out]\n", getpid());
            
            execl("./external_program2.out", argv[3], NULL);
            
            printf("parent (PID %d): completed parent", getppid());

            exit(1);
        }

        wait(NULL);
    }
    
    else {
        printf("child_1 (PID %d): process started from parent (PID %d)\n\n", getpid(), getppid());

        pid = fork();

        if (pid > 0) {
            printf("child_1 (PID %d): forking child_1.1\n", getpid());
        }

        if (pid == 0) {
            printf("child_1.1 (PID %d): process started from child_1 (PID %d)\n", getpid(), getppid());
            printf("child_1.1 (PID %d): calling an external program [./external_program1.out]\n", getpid());

            execl("./external_program1.out", argv[1], NULL);

            exit(1);
        }

        wait(NULL);
        printf("child_1 (PID %d): completed child_1.1\n\n", getpid());

        pid = fork();

        if (pid == 0) {
            printf("child_1 (PID %d): forking child_1.2\n", getppid());
            printf("child_1.2 (PID %d): process started from child_1 (PID %d)\n", getpid(), getppid());
            printf("child_1.2 (PID %d): calling an external program [./external_program1.out]\n", getpid());

            execl("./external_program1.out", argv[2], NULL);

            exit(1);
        }
        
        wait(NULL);
        printf("child_1 (PID %d): completed child_1.2\n\n", getpid());
    }

    return 0;
}

