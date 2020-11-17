#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define BUFFER 4096
int main(int argc, char** argv){
    int fd = open("test.txt",O_RDWR|O_CREAT,S_IRWXU);
    if(fd<0){
        printf("can't open file.\n");
        return 0;
    }
    char buff[BUFFER];
    int n;
    while((n=read(fd,buff,BUFFER))>0){
       if(write(STDOUT_FILENO,buff,n)!=n){
          printf("write fail.\n");
          break;
       }
    }
    if(n<0){
            printf("read fail.\n");
    }
    int err = close(fd);
    if(err==0){
        printf("close success.\n");
    }else{
        printf("close fail is %d.\n",err);
    }
    return 0;
}
