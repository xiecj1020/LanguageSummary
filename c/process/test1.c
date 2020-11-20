#include<stdio.h>
#include<unistd.h>
int global = 6;
char buf[] = "a write to stdout\n";
int main(int argc, char** argv){
    printf("main pid = %d\n",getpid());
    int var = 88;
    pid_t pid;
    size_t n = sizeof(buf);
    printf("buf length is %lu\n",n);
    if(write(STDOUT_FILENO,buf,n-1)!=(n-1)){
	printf("write error.\n");
        return 2;
    }
    printf("before fork.\n");
     pid = fork();
    if(pid<0){
	printf("fork error\n");
        return 2;
    }else if(pid==0){
      //child process
      global++;
      var++;
    }else{
       //parent proccess
       sleep(2);
    }
    printf("var=%d  global=%d pid=%d.\n",var,global,getpid());
    return 0;
}


