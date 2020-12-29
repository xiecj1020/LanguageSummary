#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include <unistd.h>
#include<sys/shm.h>
#include<sys/mman.h>
#define NLOOPS 1000
#define SIZE   sizeof(long)
static int update(long *ptr){
    return ((*ptr)++);
}
int main(int argc, char *argv[]){
    int fd,i,counter;
    pid_t pid;
    void *area;
    if((fd=open("/dev/zero",O_RDWR))<0){
        printf("open error.\n");
        exit(1);
    }
    if((area=mmap(0,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED){
        printf("open error.\n");
        exit(1);
    }
    close(fd);
    //TELL_WAIT();
    if((pid=fork())<0){
        printf("fork error.\n");
        exit(1);
    }else if(pid>0){
        for(i=0;i<NLOOPS;i+=2){
           if((counter=update((long*)area))!=i){
        	printf("parent: expected %d,got %d.\n",i,counter);
        	exit(1);
           }
           //TELL_CHILD(pid);
           //WAIT_CHILD();
        }  
    }else{
        for(i=1;i<NLOOPS+1;i+=2){
            //WAIT_PARENT(); 
           if((counter=update((long*)area))!=i){
        	printf("child: expected %d,got %d.\n",i,counter);
        	exit(1);
           }
           //TELL_PARENT(getppid());
        } 
    }
    exit(0);
}
 

