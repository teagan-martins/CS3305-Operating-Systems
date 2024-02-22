#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int input_array[4];

void *sum(void *arg) {
    int X;
    int Y;
    X = input_array[0];
    Y = input_array[1];
    printf("thread_1 (TID %ld) reads X = %d and Y = %d from input_array[]\n", (long)pthread_self(), X, Y);

    int S = X + Y;
    
    input_array[2] = S;
    printf("thread_1 (TID %ld) writes X+Y = %d to the input_array[2]\n", (long)pthread_self(), S);

    pthread_exit(NULL);
}

void *multiplication(void *thread_id) {
    
    int X;
    int Y;
    int M;
    
    X = input_array[0];
    Y = input_array[1];
    
    M = X * Y;
    
    input_array[3] = M;
    
}

void *even_odd(void *arg) {
    int S;
    S = input_array[2];
    printf("thread_2 (TID %ld) reads %d from the input_array[2]\n", (long)pthread_self(), S);

    if (S % 2 == 0) {
        printf("thread_2 (TID %ld) identifies that %d is an even number\n", (long)pthread_self(), S);
    } else {
        printf("thread_2 (TID %ld) identifies that %d is an odd number\n", (long)pthread_self(), S);
    }
    
    
    multiplication(NULL);
    
    printf("thread_2 (TID %ld) reads X and Y from input_array[], writes X * Y = %d to input_array[3]\n", (long)pthread_self(), input_array[3]);

    pthread_exit(NULL);
}

void *reverse_num(void *arg) {
    int M;
    M = input_array[3];
    printf("thread_3 (TID %ld) reads %d from input_array[3]\n", (long)pthread_self(), M);

    int reversed_number = 0;
    int remainder;
    int x = M;

    while (x != 0) {
        remainder = x % 10;
        reversed_number = reversed_number * 10 + remainder;
        x = x / 10;
    }

    printf("thread_3 (TID %ld) reverses the number %d -> %d\n", (long)pthread_self(), M, reversed_number);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    int X = atoi(argv[1]);
    int Y = atoi(argv[2]);

    input_array[0] = X;
    input_array[1] = Y;

    printf("parent (PID %d) receives X = %d and Y = %d from the user\n", getpid(), X, Y);
    printf("parent (PID %d) writes X = %d and Y = %d to input_array[]\n", getpid(), X, Y);

    pthread_t threads[3];

    pthread_create(&threads[0], NULL, sum, NULL);
    pthread_create(&threads[1], NULL, even_odd, NULL);
    pthread_create(&threads[2], NULL, reverse_num, NULL);

    
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    return 0;
}

