#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int fork_test(){
    int status;
	int value = 0;
	int *pointer = &value;
        
	pid_t pid;
        printf("Parent: Initial value is %d\n", value);
        pid = fork();

        if(pid < 0 )
                exit(-1);
        else if (pid == 0){
                *pointer = 1;
                printf("child: %p\n", &pointer);
                printf("child: %p\n", pointer);
                printf("child: %d\n", *pointer);
                exit(EXIT_SUCCESS);
        }
        else{
                *pointer = 2;
                while(pid != wait(&status));
                printf("parent: %p\n", &pointer);
                printf("parent: %p\n", pointer);
                printf("parent: %d\n", *pointer);
        }
        return 0;
}
