#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char** argv){
    printf("main\n");
    alarm(2);
    pause();
     printf("end\n");
    return 0;
}


