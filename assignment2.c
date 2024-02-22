#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int summation(int start, int end)
{
    int sum = 0;
    if (start < end)
    {
        sum = ((end * (end + 1)) - (start * (start - 1))) / 2;
    }
    return sum;
}

int ith_part_start(int i, int N, int M)
{
    int part_size = N / M;
    int start = i * part_size;
    return start;
}

int ith_part_end(int i, int N, int M)
{
    int part_size = N / M;
    int end = (i < M - 1) ? ((i + 1) * part_size - 1) : N;
    return end;
}

int main(int argc, char **argv)
{
    
    printf("parent(PID %d): process started\n\n", getpid());
    
    int count = 1;
    
    int port[2];

    int M = atoi(argv[2]);
    int N = atoi(argv[1]);
    

    if (pipe(port) == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid_t pid_child_1 = fork();
    
    printf("parent(PID %d): forking child_1\n", getpid());

    if (pid_child_1 == -1)
    {
        perror("fork");
        exit(1);
    }

    if (pid_child_1 == 0)
    {

        printf("child_1(PID %d): process started from parent(PID %d)\n", getpid(), getppid());
        printf("child_1(PID %d): forking child_1.1...,child_1.%d\n\n", getpid(),M);

        
        for (int x = 0; x < M; x++)
        {   
            
            pid_t pid_child_x = fork();

            if (pid_child_x == -1)
            {
                perror("fork");
                exit(1);
            }

            if (pid_child_x == 0)
            {
                int start = ith_part_start(x, N, M);
                int end = ith_part_end(x, N, M);

                int sum_partial = summation(start, end);

                printf("child_1.%d(PID %d): fork() successful\n", count, getpid());
                printf("child_1.%d(PID %d): partial sum: [%d - %d] = %d\n", count, getpid(), start, end, sum_partial);


                write(port[1], &sum_partial, sizeof(int));

                exit(0);
            }
            
            count = count + 1;
        }


        int sum_total = 0;
        for (int x = 0; x < M; x++)
        {
            int sum_partial;
            read(port[0], &sum_partial, sizeof(int));

            sum_total = sum_total + sum_partial;
        }
        printf("\nchild_1(PID %d): total Sum: %d\n", getpid(), sum_total);
        printf("child_1(PID %d): child_1 completed\n\n", getpid());

        exit(0);
    }
    else
    {
        printf("parent(PID %d): fork successful for child_1(PID %d)\n", getpid(), pid_child_1);
        printf("parent(PID %d): waiting for child_1(PID %d) to complete\n\n", getpid(), pid_child_1);
        wait(NULL);

        printf("parent(PID %d): parent completed\n", getpid());
    }

    return 0;
}

