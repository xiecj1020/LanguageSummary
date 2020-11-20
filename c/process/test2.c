#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int global = 6;
int main(int argc, char** argv){
    int var = 88;
    pid_t pid;
    printf("before fork.\n");
     pid = vfork();
    if(pid<0){
	printf("fork error\n");
        return 2;
    }else if(pid==0){
      //child process
      global++;
      var++;
      _exit(0);
    }else{
       //parent proccess
       //sleep(2);
    }
    printf("var=%d  global=%d pid=%d.\n",var,global,getpid());
    return 0;
}


