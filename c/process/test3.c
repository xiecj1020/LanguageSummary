#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
extern char **environ;
int main(int argc, char** argv){
    pid_t pid;
    int status;
    pid = fork();
    if(pid<0){
	printf("fork error\n");
        return 2;
    }else if(pid==0){
      exit(7);
    }else{
       //parent proccess
       //sleep(2);
    }
    if(wait(&status)!=pid){
       printf("wait error\n");
        return 2;
    }
     printf("status= %d\n",status);
    pid = fork();
    if(pid<0){
	printf("fork error\n");
        return 2;
    }else if(pid==0){
      status /=0;
    }else{
       //parent proccess
       //sleep(2);
    }
  if(wait(&status)!=pid){
       printf("wait error\n");
        return 2;
    }
     printf("status= %d\n",status);
    return 0;
}


