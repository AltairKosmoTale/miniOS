#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int pipe_test(){
	char write_msg[BUFFER_SIZE] = "Greetings\n";
	char read_msg[BUFFER_SIZE];
	int fd[2];
	int status;
	pid_t pid;
	
	if (pipe(fd) == -1){
		fprintf(stderr, "Pipe failed");
		exit(1);
	}
	pid = fork();
	
	if (pid < 0){
		fprintf(stderr, "Fork failed");
		exit(1);
	}
	
	if (pid == 0) {
		close(fd[WRITE_END]);
		read(fd[READ_END], read_msg, BUFFER_SIZE);
		close(fd[READ_END]);
		printf("read: %s", read_msg);
		exit(EXIT_SUCCESS);
	}
	else {
		close(fd[READ_END]);
		write(fd[WRITE_END], write_msg, strlen(write_msg)+1);
		close(fd[WRITE_END]);
		while(pid != wait(&status));
	}
	return 0;
}
	
