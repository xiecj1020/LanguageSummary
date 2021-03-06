//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test2
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc pthread_test2.c -o pthread_test2 -lpthread
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
void *thr_fn1(void *args){
    printf("thread 1 returing.\n");
    return ((void*)1);
}
void *thr_fn2(void *args){
    printf("thread 2 exiting.\n");
    pthread_exit((void*)2);
}
int main(int argc, char** argv){
    int err;
    pthread_t tid1,tid2;
    void* tret;
    err = pthread_create(&tid1,NULL,thr_fn1,NULL);
    if(err!=0){
       printf("Can't create thread 1.\n");
       return 0;
    }
    err = pthread_create(&tid2,NULL,thr_fn2,NULL);
    if(err!=0){
       printf("Can't create thread 2.\n");
       return 0;
    }
    err = pthread_join(tid1,&tret);
    if(err!=0){
       printf("Can't join thread 1.\n");
       return 0;
    }
    printf("thread 1 exit code is %ld.\n",(long)tret);
    err = pthread_join(tid2,&tret);
    if(err!=0){
       printf("Can't join thread 2.\n");
       return 0;
    }
    printf("thread 2 exit code is %ld.\n",(long)tret);
    printf("main over.\n");
    return 0;
}

