//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test1
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc pthread_test1.c -o pthread_test1 -lpthread
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
pthread_t ntid;
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
    int err;
    //线程属性参数
    pthread_attr_t *attr = NULL; 
    //线程执行的函数 
    void*(*start_run)(void*) = thr_fn;
    //执行函数的参数
    void *args = NULL;
    err = pthread_create(&ntid,attr,start_run,args);
    if(err!=0){
        printf("can't create thread,the error =% d\n",err);
        return 1;
    }
    printids("main thread: ");
    sleep(1);
    printf("main over.\n");
    return 0;
}

