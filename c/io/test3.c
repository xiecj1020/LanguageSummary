#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#define BUFFER 100
int TEST_FILE_SIZE = 516581760;
int main(int argc, char** argv){
    int fd = open("test.txt",O_RDWR|O_CREAT,S_IRWXU);
    if(fd<0){
        printf("can't open file.\n");
        return 0;
    }
    int i = 0;
    char buff[1];
    for(i=0;i<TEST_FILE_SIZE;i++){
       buff[0] = 'a';
       int n = write(fd,buff,1);
       if(n==0){
           printf("do not write anything.\n");
       }else if(n<0){
           printf("write error.\n");
           break;
       }else if(n>1){
           printf("write more n=%d.\n",n);
       }
    }
    int err = close(fd);
    if(err==0){
        printf("close success.\n");
    }else{
        printf("close fail is %d.\n",err);
    }
    return 0;
}
