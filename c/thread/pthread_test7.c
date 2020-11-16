//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test7
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc -g pthread_test7.c -o pthread_test7 -lpthread
//互斥量要么是锁住状态，要么是没锁住状态，一次只有一个线程对其加锁
//如果线程对同一个互斥量加锁两次，将会产生死锁。
//pthread_mutex_t        互斥量变量
//pthread_mutex_init     互斥量初始化
//pthread_mutex_destory  互斥量销毁
//pthread_mutex_lock     互斥量锁定锁
//pthread_mutex_unlock   互斥量释放锁
//这个程序故意对它已经有的互斥量加锁，目的是演示pthread_mutex_timedlock是如何工作的。
//不推荐在实际中使用这种策略，因为会导致死锁。
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc, char** argv){
    struct timespec tout;
    struct tm *tmp;
    int err;
    char buf[64];
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&lock);
    printf("mutex is locked.\n");
    clock_gettime(CLOCK_REALTIME,&tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf,sizeof(buf),"%r",tmp);
    printf("current time is %s.\n",buf);
    tout.tv_sec += 10;
    pthread_t tid1,tid2;
    err = pthread_mutex_timedlock(&lock,&tout);
    clock_gettime(CLOCK_REALTIME,&tout);
    tmp = localtime(&tout.tv_sec);
    strftime(buf,sizeof(buf),"%r",tmp);
    printf("the time is now %s.\n",buf);
    if(err==0){
       printf("mutex lock again.\n");
    }else{
       printf("can't mutex lock again:%d.\n",err);
    }
    
    printf("main end.\n");
    return 0;
}

