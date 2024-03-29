#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void* thread_summation(void* arg);

int sum = 0;

int main(int argc, char **argv) {
    pthread_t t_id1, t_id2;
    int range1[] = {1, 5};
    int range2[] = {6, 10};
    if (pthread_create(&t_id1, NULL, thread_summation, (void*)&range1) != 0) {
        perror("pthread_create() error");
        return -1;
    }

    if (pthread_create(&t_id2, NULL, thread_summation, (void*)&range2) != 0) {
        perror("pthread_create() error");
        return -1;
    }

    if (pthread_join(t_id1, NULL) != 0) {
        puts("pthread_join() error!");
        return -1;
    }

    if (pthread_join(t_id2, NULL) != 0) {
        perror("pthread_join() error");
        return -1;
    }

    printf("the sum of 1 to 10 is: %d\n", sum);
    return 0;
}

void* thread_summation(void* arg) {
    int start = ((int *)arg)[0];
    int end = ((int *)arg)[1];
    int i;
    for (i = start; i <= end; ++i)
    {
        sum += i;
    }
    return NULL;
}