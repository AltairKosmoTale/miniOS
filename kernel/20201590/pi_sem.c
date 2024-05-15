#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>

extern int number_of_points_in_circle;
extern int number_of_threads;
extern int total_number_of_points;
	
sem_t semaphore;

void *monte_carlo_pi_sem(void *arg) {
    int iterations = total_number_of_points / number_of_threads;
    int inside_circle_cnt = 0; // Thread 안에서 local로 계산 후 semaphore로 전역 변수로 내보냄
    unsigned int seed = time(NULL) + pthread_self();
    for (int i = 0; i < iterations; ++i) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0)
            inside_circle_cnt++;
    }
    sem_wait(&semaphore);
    number_of_points_in_circle += inside_circle_cnt;
    sem_post(&semaphore);
    pthread_exit(NULL);
}

int pi_sem() {
	number_of_points_in_circle = 0;
    number_of_threads = 0;
    total_number_of_points = 0;
	
    struct timeval  tv;
    double begin, end;
    printf("Number_of_Threads: ");
    scanf("%d",&number_of_threads);
    printf("Total_number_of_points: ");
    scanf("%d",&total_number_of_points);
    pthread_t threads[number_of_threads];
    if (sem_init(&semaphore, 0, 1) != 0) {
        printf("Semaphore initialization failed\n");
        return 1;
    }
    gettimeofday(&tv, NULL);
    begin = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    for (int i = 0; i < number_of_threads; ++i) {
        pthread_create(&threads[i], NULL, monte_carlo_pi_sem, NULL);
    }
    for (int i = 0; i < number_of_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&tv, NULL);
    end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    double pi_estimate = 4.0 * number_of_points_in_circle / total_number_of_points;
    printf("Estimated value of Pi: %.6f\n", pi_estimate);
    printf("Time taken: %.3f seconds\n", (end - begin) / 1000);

    sem_destroy(&semaphore);
    return 0;
}