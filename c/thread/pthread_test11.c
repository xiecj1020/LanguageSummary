//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test11
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc -g pthread_test11.c -o pthread_test11 -lpthread
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
int makethread(void *(*fn)(void *),void * arg){
    pthread_t tid;
    int err;
    pthread_attr_t attr; 
    err = pthread_attr_init(&attr);
    if(err!=0){
        return err;
    }
    err = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if(err==0){
        err = pthread_create(&tid,&attr,fn,arg);
    }
    err = pthread_attr_destroy(&attr);
    printf("pthread attr destroy result is %d.\n",err);
    return err;
}
void printids(const char *s){
   pid_t pid;
   pthread_t tid;
   pid = getpid();
   tid = pthread_self();
   printf("%s pid %lu tid %lu (0x%lx)\n",s,(unsigned long)pid,
         (unsigned long)tid,(unsigned long)tid); 
}
void *thr_fn(void *arg){
    printids(" new thread: ");
    return ((void*)0);
}
int main(int argc, char** argv){
    makethread(thr_fn,NULL);
    sleep(1);
    printf("main over.\n");
    return 0;
}

