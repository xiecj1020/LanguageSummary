//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test10
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc -g pthread_test10.c -o pthread_test10 -lpthread
//自旋锁与互斥量类似，但它不是休眠使进程阻塞，而是在获取锁之前一直处于忙等(自旋)阻塞状态。
//自旋可以用于以下情况:锁被持有的时间短，而且线程不想再重新调度上花费太多的成本。
//一次只有一个线程可以占有写模式下的读写锁，但是可以多个线程同时占有读模式下的读写锁。
//pthread_spinlock_t   自旋变量
//pthread_spin_init    自旋变量初始化
//pthread_spin_destroy 自旋变量销毁
//pthread_spin_lock    自旋加锁
//pthread_spin_trylock 自旋加锁    
//pthread_spin_unlock  自旋解锁

//##########################################################
//屏障是用户协调多个线程并行工作的同步机制。屏障允许每个线程等待，直到所有的合作线程都到达某个点
//然后从该点继续执行。
//pthread_barrier_t    屏障变量
//pthread_barrier_init 屏障变量初始化
//pthread_barrier_destory 屏障变量销毁
//pthread_barrier_wait 表明线程已经完成工作，等待所有线程赶上来


#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<limits.h>
#include<sys/time.h>
#define NTHR 8
#define NUMNUM 8000000L
#define TNUM (NUMNUM/NTHR)
long nums[NUMNUM];
long snums[NUMNUM];
pthread_barrier_t b;
//#ifdef SOLARIS
//#define heapsort qsort
//#else
//extern int heapsort(void*,size_t,size_t,int(*)(const void *,const void *));
//#endif
int heapsort(void *base,size_t nel,size_t width,int(*comp)(const void *,const void *)){
    return 0;
}
int complong(const void *arg1,const void *arg2){
   long l1 = *((long*)arg1); 
   long l2 = *((long*)arg2);
   if(l1==l2){
      return 0;
   }else if(l1<l2){
      return -1;
   }else{ 
      return 1;
   }

}
void* thr_fn(void * arg){
    long idx = (long)arg;
    heapsort(&nums[idx],TNUM,sizeof(long),complong);
    pthread_barrier_wait(&b);
    return ((void*)0);
}
void merge(){
    long idx[NTHR];
    long i,minidx,sidx,num;
    for(i=0;i<NTHR;i++){
        idx[i] = i * TNUM;
    }
    for(sidx=0;sidx<NUMNUM;sidx++){
        num = LONG_MAX;
        for(i=0;i<NTHR;i++){
            if((idx[i]<(i+1)*TNUM) && (nums[idx[i]]<num)){
		num = nums[idx[i]];
                minidx = i;
            }
            //printf("sidx= %ld minidx= %ld idx[%ld]=%ld.\n",sidx,minidx,minidx,idx[minidx]);
            snums[sidx]=nums[idx[minidx]];
            //idx[minidx]++;  
        }
    }
}
int main(int argc, char** argv){
    unsigned long i;
    struct timeval start,end;
    long long startusec,endusec;
    double elpased;
    int err;
    pthread_t tid;
    srandom(1);
    for(i=0;i<NUMNUM;i++){
        nums[i] = random();
    }
    gettimeofday(&start,NULL);
    pthread_barrier_init(&b,NULL,NTHR+1);
    for(i=0;i<NTHR;i++){
        err = pthread_create(&tid,NULL,thr_fn,(void*)(i*TNUM));
        if(err!=0){
            printf("can't create thread %ld.\n",i);
            return 0;
        }
    }
    pthread_barrier_wait(&b);
    merge();
    gettimeofday(&end,NULL);
    startusec = start.tv_sec * 1000000+start.tv_usec; 
    endusec = end.tv_sec * 1000000+end.tv_usec;
    elpased = (endusec-startusec)/1000000.0;
    printf("sort take %.4f seconds.\n",elpased);
    for(i=0;i<NUMNUM;i++){
        printf("%ld\n",snums[i]);
    }
    return 0;
}
