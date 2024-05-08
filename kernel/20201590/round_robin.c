#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PROCESSES 5
#define TIME_QUANTUM 10

// 프로세스 정보를 담을 구조체
typedef struct {
    int pid;
    int burst_time;
    int remaining_time;
} Process;

void schedule_round_robin(Process processes[], int n) {
    int time = 0;

    // 모든 프로세스가 완료될 때까지 반복
    while (1) {
        int all_done = 1; // 모든 프로세스가 완료되었는지 체크

        for (int i = 0; i < n; i++) {
            if (processes[i].remaining_time > 0) {
                all_done = 0; // 아직 완료되지 않은 프로세스가 있다면 all_done을 0으로 설정
                int run_time = (processes[i].remaining_time > TIME_QUANTUM) ? TIME_QUANTUM : processes[i].remaining_time;
                processes[i].remaining_time -= run_time;
                time += run_time;
                printf("PID: %d runs for %d \tTotal time: %d\n", processes[i].pid, run_time, time);
            }
        }

        if (all_done) {
            break; // 모든 프로세스가 완료되었다면 while 루프 종료
        }
    }

    printf("Total scheduling time: %d time units.\n", time);
}

void round_robin() {
    Process processes[NUM_PROCESSES];
    srand(time(NULL));  // 난수 생성기 초기화

    // 프로세스 초기화
    for (int i = 0; i < NUM_PROCESSES; i++) {
        processes[i].pid = i + 1;
        processes[i].burst_time = rand() % 50 + 1;  // 1부터 50까지의 랜덤 burst time
        processes[i].remaining_time = processes[i].burst_time;
    }

    // 생성된 프로세스 정보 출력
    printf("Initial Processes Burst Time:\n");
	printf("Time Quantum: %d\n", TIME_QUANTUM);
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("PID: %d, Burst Time: %d\n", processes[i].pid, processes[i].burst_time);
    }
    
    // Round Robin 스케줄링 실행
	printf("\nResult of Round Robin Scheduler:\n");
    schedule_round_robin(processes, NUM_PROCESSES);
}