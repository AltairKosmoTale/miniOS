#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

int producer(){
	const int SIZE = 4096;
	const char *name = "OS";
	const char *message_0 = "Hello";
	const char *message_1 = "World!";
	int fd;
	char *ptr, *mapped_ptr; // 시작 위치 저장을 위함
	int pid;
	pid = getpid();
	char addr[100]; // system("") 명령어를 담기 위한 버퍼
	
	// "OS"라는 이름의 shared_memory 흔적 + Memory Map Size 출력
	sprintf(addr, "pmap %d | grep -e OS -e total", pid);
	printf("\n[Producer pmap: before shm_open]\n"); // 1. Producer가 shm_open 전 정보
	system(addr); // 명령어 실행
	
	// Producer Shared_memory 생성 및 Mapping
	fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(fd, SIZE);
	ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	mapped_ptr = ptr; // 시작위치 저장
	printf("\n[Producer pmap: after shm_open]\n"); // 2. Producer가 shm_open 후 정보
	system(addr);
	
	// 메세지 출력
	sprintf(ptr, "%s", message_0);
	ptr += strlen(message_0);
	sprintf(ptr, "%s", message_1);
	ptr += strlen(message_1);
	sleep(2); // Consumer가 받을 수 있도록 동시성 해결
	// 추후 mutex semaphore whatever
	
	printf("\n[Producer pmap: after shm_unlink]\n");
	shm_unlink(name);
	system(addr); // 7. Producer가 shm_unlink 후 정보
	
	printf("\n[Producer pmap: after munmap, close]\n");
	munmap(mapped_ptr, SIZE); // 8. Producer가 munmap 후 정보
	close(fd);
	system(addr);
	return 0;
}

int consumer(){
	sleep(1); // parent 입력 대기
	const int SIZE = 4096;
	const char *name = "OS";
	int fd;
	char *ptr;
	int pid;
	pid = getpid();
	char addr[100];
	// "OS"라는 이름의 shared_memory 흔적 + Memory Map Size 출력
	printf("\n[Consumer pmap: before shm_open]\n");
	sprintf(addr, "pmap %d | grep -e OS -e total", pid);
	system(addr); // 3. Consumer가 shm_open 전 정보
	
	// Consumer Shared_memory 생성 및 Mapping
	fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	ftruncate(fd, SIZE);
	ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	printf("\n[Consumer pmap: after shm_open]\n");
	system(addr); // 4. Consumer가 shm_open 후 정보
	printf("%s\n", (char *)ptr);
	shm_unlink(name);
	printf("\n[Consumer pmap: after shm_unlink]\n");
	system(addr); // 5. Consumer가 shm_unlink 후 정보
	printf("\n[Consumer pmap: after munmap, close]\n");
	munmap(ptr, SIZE);
	close(fd);
	system(addr); // 6. Consumer가 munmap 후 정보
	return 0;
}

int shared_memory_test(){
	int status;
	pid_t pid;
	pid = fork();
	
	if (pid < 0){
		fprintf(stderr, "Fork failed");
		exit(1);
	}
	
	if (pid == 0) {
		consumer();
		exit(EXIT_SUCCESS);
	}
	
	else {
		producer();
		while(pid != wait(&status));
	}
	printf("\n[After wait from parent pmap]\n");
	pid = getpid();
	char addr[100];
	// 9. minios kenel로 돌아가기 전 Process의 Memory Map 정보
	sprintf(addr, "pmap %d | grep -e OS -e total", pid);
	system(addr); 
	return 0;
}
