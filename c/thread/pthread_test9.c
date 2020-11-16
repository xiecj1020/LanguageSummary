//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test8
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc -g pthread_test9.c -o pthread_test9 -lpthread
//条件本身是由互斥量保护的，线程在改变条件变量之前必须锁住互斥量。
//一次只有一个线程可以占有写模式下的读写锁，但是可以多个线程同时占有读模式下的读写锁。
//pthread_cond_t       条件变量
//pthread_cond_init    条件变量初始化
//pthread_cond_destroy 条件变量销毁
//pthread_cond_wait    等待条件标量变为真
//pthread_cond_timedwait 等待条件标量变为真，带有超时时间    
//pthread_cond_signal   至少能够唤醒一个等待该条件的线程
//pthread_cond_broadcast   能够唤醒等待该条件的所有线程
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
struct msg{
    struct msg *m_next;
};
struct msg *workq;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;
void process_msg(){
    struct msg *mp;
    for(;;){
        pthread_mutex_lock(&qlock);
        //条件是工作队列的状态,用互斥量保护条件,在while循环中判断条件
        while(workq==NULL){
            pthread_cond_wait(&qready,&qlock);
        }
        mp = workq;
        printf("process_msg %p.\n",mp);
        workq = mp->m_next;
        pthread_mutex_unlock(&qlock);
        free(mp);
        sleep(2);
    }
}
void enqueue_msg(struct msg *mp){
     printf("enqueue_msg %p.\n",mp);
     pthread_mutex_lock(&qlock);
     mp->m_next = workq;
     workq = mp;
     pthread_mutex_unlock(&qlock);
     pthread_cond_signal(&qready);
}
void *thr_fn1(void *args){
    struct msg * first_msg = malloc(sizeof(struct msg));
    enqueue_msg(first_msg);
    sleep(1);
    first_msg = malloc(sizeof(struct msg));
    enqueue_msg(first_msg);
    return ((void*)1);
}

int main(int argc, char** argv){
    pthread_t tid1;
    int err;
    err = pthread_create(&tid1,NULL,thr_fn1,NULL);
    if(err!=0){
       printf("Can't create thread 1.\n");
       return 0;
    }
    process_msg();
    printf("main end.\n");
    return 0;
}

