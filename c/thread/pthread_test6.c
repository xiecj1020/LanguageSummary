//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test6
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc -g pthread_test6.c -o pthread_test6 -lpthread
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
struct foo{
    int fcount;
    pthread_mutex_t flock;
    int fid;
};
struct foo *foo_alloc(int id){
    struct foo *fp;
    if((fp=malloc(sizeof(struct foo)))!=NULL){
	fp->fcount = 0;
        fp->fid= id;
        if(pthread_mutex_init(&fp->flock,NULL)!=0){
	    free(fp);
            return NULL;
        }
    }
    return fp;
}
void testReentranLock(struct foo *fp){
    pthread_mutex_lock(&fp->flock);
    printf("testReentranLock.\n");
    pthread_mutex_unlock(&fp->flock);
}
void foo_hold(struct foo *fp){
    pthread_mutex_lock(&fp->flock);
    fp->fcount++;
    //下面行会引起死锁，mutex_lock不可以向java的ReentranLock实现锁从入
    //如果线程对同一个互斥量加锁两次，将会产生死锁。
    //testReentranLock(fp); 
    //waiting 2 seconds.
    sleep(2);
    pthread_mutex_unlock(&fp->flock);
}
void foo_rele(struct foo *fp){
    pthread_mutex_lock(&fp->flock);
    if(--fp->fcount==0){
	pthread_mutex_unlock(&fp->flock);
        pthread_mutex_destroy(&fp->flock);
        free(fp);
        printf("foo_rele free.\n");
    }else{
        printf("foo_rele.\n");
	pthread_mutex_unlock(&fp->flock);
    }
}

void *thr_fn1(void *args){
    printf("thread 1 start.\n");
    struct foo *fp = (struct foo *)args;
    foo_hold(fp);
    printf("thread 1 end.\n");
    return ((void*)1);
}
void *thr_fn2(void *args){
    printf("thread 2 start.\n");
    struct foo *fp = (struct foo *)args;
    foo_hold(fp);
    printf("thread 2 end.\n");
    return ((void*)2);
}

int main(int argc, char** argv){
    struct foo *fp = foo_alloc(1);
    int err;
    pthread_t tid1,tid2;
    err = pthread_create(&tid1,NULL,thr_fn1,fp);
    if(err!=0){
       printf("Can't create thread 1.\n");
       return 0;
    }
    err = pthread_create(&tid2,NULL,thr_fn2,fp);
    if(err!=0){
       printf("Can't create thread 2.\n");
       return 0;
    }
    sleep(5);
    foo_rele(fp);
    foo_rele(fp);
    printf("main end.\n");
    return 0;
}

