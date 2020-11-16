//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test5
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc pthread_test5.c -o pthread_test5 -lpthread
//互斥量要么是锁住状态，要么是没锁住状态，一次只有一个线程对其加锁
//如果线程对同一个互斥量加锁两次，将会产生死锁。
//pthread_mutex_t        互斥量变量
//pthread_mutex_init     互斥量初始化
//pthread_mutex_destory  互斥量销毁
//pthread_mutex_lock     互斥量锁定锁
//pthread_mutex_unlock   互斥量释放锁
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)
struct foo{
    int fcount;
    pthread_mutex_t flock;
    int fid;
    struct foo *fnext;
};
struct foo *fh[NHASH];
pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;
struct foo *foo_alloc(int id){
    struct foo *fp;
    int idx;
    if((fp=malloc(sizeof(struct foo)))!=NULL){
	fp->fcount = 1;
        fp->fid= id;
        if(pthread_mutex_init(&fp->flock,NULL)!=0){
	    free(fp);
            return NULL;
        }
        idx = HASH(id);
        pthread_mutex_lock(&hashlock);
        fp->fnext = fh[idx];
        fh[idx] = fp;
        pthread_mutex_lock(&fp->flock);
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->flock);
    }
    return fp;
}
void foo_hold(struct foo *fp){
    pthread_mutex_lock(&fp->flock);
    fp->fcount++;
    pthread_mutex_unlock(&fp->flock);
}
void foo_rele(struct foo *fp){
    pthread_mutex_lock(&fp->flock);
    if(--fp->fcount==0){
	pthread_mutex_unlock(&fp->flock);
        pthread_mutex_destroy(&fp->flock);
        free(fp);
    }else{
	pthread_mutex_unlock(&fp->flock);
    }
}

int main(int argc, char** argv){
    return 0;
}

