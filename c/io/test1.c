#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#define BUFFER 100
int main(int argc, char** argv){
    //int n;
    //char buff[BUFFER];
    //while((n=read(STDIN_FILENO,buff,BUFFER))>0){
    //    printf("read %d chars.\n",n);
   //     if(write(STDOUT_FILENO,buff,n)!=n){
    //         printf("write fail.\n");
    //         break; 
    //    }else{
    //         printf("\n");
    //         printf("write success.\n");
    //    }
    //}
    //12345会立即输出
    int n = write(STDOUT_FILENO,"12345",5);
    //会在程序退出的时候输出,说明有缓冲
    printf("n=%d",n);
    printf("char bit=%d\n",CHAR_BIT); 
    sleep(3);
    return 0;
}
