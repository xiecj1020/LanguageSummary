#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
char* envinit[]={"user=xie","path=/tmp",NULL};
int main(int argc, char** argv){
    pid_t pid;
    int status;
    pid = fork();
    if(pid<0){
	printf("fork error\n");
        return 2;
    }else if(pid==0){
        int result=execle("/home/user/test/c/test","test","arg1","arg2",(char*)NULL,envinit);
        if(result<0){
            printf("execle error\n");
            return 2;
        }
    }else{
       //parent proccess
       //sleep(2);
    }
    if((waitpid(pid,NULL,0))<0){

 printf("waitpid error\n");
            return 2;
    }
    
    return 0;
}


