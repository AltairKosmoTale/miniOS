#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int number_of_threads = 0; // 총 Thread 개수 (입력 받음)
int total_number_of_points = 0; // 총 반복 횟수 (입력 받음)
int number_of_points_in_circle = 0; // circle로 판별 된 수 (계산 예정)

pthread_mutex_t mutex;
void *monte_carlo_pi(void *arg) {
    int iterations = total_number_of_points / number_of_threads;
    int inside_circle_cnt = 0; // Thread 안에서 local로 계산 후 mutex로 전역 변수로 내보냄
	unsigned int seed = time(NULL) + pthread_self();
    for (int i = 0; i < iterations; ++i) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0)
            inside_circle_cnt++;
	}
    pthread_mutex_lock(&mutex);
    number_of_points_in_circle += inside_circle_cnt;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int pi() {
	struct timeval  tv;
	double begin, end;
	printf("Number_of_Threads: ");
	scanf("%d",&number_of_threads);
	printf("Total_number_of_points: ");
	scanf("%d",&total_number_of_points);
    pthread_t threads[number_of_threads];
	if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Mutex initialization failed\n");
        return 1;
    }
	gettimeofday(&tv, NULL);
	begin = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    for (int i = 0; i < number_of_threads; ++i) {
        pthread_create(&threads[i], NULL, monte_carlo_pi, NULL);
    }
    for (int i = 0; i < number_of_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&tv, NULL);
	end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
    double pi_estimate = 4.0 * number_of_points_in_circle / total_number_of_points;
    printf("Estimated value of Pi: %.6f\n", pi_estimate);
    printf("Time taken: %.3f seconds\n", (end-begin)/1000);
	number_of_points_in_circle = 0;
	number_of_threads = 0;
	total_number_of_points = 0;
    return 0;
}

/*
	unsigned int seed = time(NULL);
	srand(seed);
    for (int i = 0; i < iterations; ++i) {
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0)
            inside_circle_cnt++;
	}
*/

/*
	unsigned int seed = time(NULL) + pthread_self();
    for (int i = 0; i < iterations; ++i) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
		printf("thread_%ld, %.2f, %.2f\n", pthread_self(), x, y);
        if (x * x + y * y <= 1.0)
            inside_circle_cnt++;
	}
*/