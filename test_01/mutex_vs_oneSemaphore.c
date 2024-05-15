#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define NUM_ITERATIONS 1000000000
#define NUM_THREADS 16

int counter_mutex = 0;
int counter_semaphore = 0;
pthread_mutex_t mutex;
sem_t semaphore;

void *increment_with_mutex(void *arg) {
    for (int i = 0; i < NUM_ITERATIONS / NUM_THREADS; ++i) {
        pthread_mutex_lock(&mutex);
        counter_mutex++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *increment_with_semaphore(void *arg) {
    for (int i = 0; i < NUM_ITERATIONS / NUM_THREADS; ++i) {
        sem_wait(&semaphore);
        counter_semaphore++;
        sem_post(&semaphore);
    }
    pthread_exit(NULL);
}

void measure_mutex() {
    pthread_t threads[NUM_THREADS];
    struct timeval tv;
    double start, end;

    pthread_mutex_init(&mutex, NULL);

    gettimeofday(&tv, NULL);
    start = tv.tv_sec + tv.tv_usec / 1000000.0;

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, increment_with_mutex, NULL);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&tv, NULL);
    end = tv.tv_sec + tv.tv_usec / 1000000.0;

    printf("Final counter value with mutex: %d\n", counter_mutex);
    printf("Time taken with mutex: %f seconds\n", end - start);

    pthread_mutex_destroy(&mutex);
}

void measure_semaphore() {
    pthread_t threads[NUM_THREADS];
    struct timeval tv;
    double start, end;

    sem_init(&semaphore, 0, 1);

    gettimeofday(&tv, NULL);
    start = tv.tv_sec + tv.tv_usec / 1000000.0;

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, increment_with_semaphore, NULL);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&tv, NULL);
    end = tv.tv_sec + tv.tv_usec / 1000000.0;

    printf("Final counter value with semaphore: %d\n", counter_semaphore);
    printf("Time taken with semaphore: %f seconds\n", end - start);

    sem_destroy(&semaphore);
}

int main() {
    printf("Measuring performance with mutex:\n");
    measure_mutex();

    printf("\nMeasuring performance with semaphore:\n");
    measure_semaphore();

    return 0;
}