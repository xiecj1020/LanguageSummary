//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test8
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc -g pthread_test8.c -o pthread_test8 -lpthread
//读写锁有3中状态，读模式下加锁状态，写模式下加锁状态，不加锁状态。
//一次只有一个线程可以占有写模式下的读写锁，但是可以多个线程同时占有读模式下的读写锁。
//pthread_rw_t          读写锁变量
//pthread_rw_init       读写锁初始化
//pthread_rw_destroy    读写锁销毁
//pthread_rw_rdlock     读写锁读状态锁定
//pthread_rw_wrlock     读写锁锁状态锁定
//pthread_rw_unlock     读写锁释放
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
struct job{
    struct job *j_next;
    struct job *j_prev;
    pthread_t j_id;
};
struct queue{
   struct job* q_head;
   struct job* q_tail;
   pthread_rwlock_t q_lock; 
};
int queue_init(struct queue *qp){
   int err;
   qp->q_head = NULL;
   qp->q_tail = NULL;
   err = pthread_rwlock_init(&qp->q_lock,NULL);
   if(err!=0){
      return err;
   } 
   return 0;
}
void job_insert(struct queue *qp,struct job *job){
    pthread_rwlock_wrlock(&qp->q_lock);
    job->j_next = qp->q_head;
    job->j_prev = NULL;
    if(qp->q_head!=NULL){
       qp->q_head->j_prev = job;
    }else{
       qp->q_tail = job; //queue is empty
    }
    qp->q_head = job;
    pthread_rwlock_unlock(&qp->q_lock);
}
void job_append(struct queue *qp,struct job *job){
    pthread_rwlock_wrlock(&qp->q_lock);
    job->j_next = NULL;
    job->j_prev = qp->q_tail;
    if(qp->q_tail!=NULL){
        qp->q_tail->j_next = job;
    }else{
        qp->q_head = job; //queue is empty
    }
    pthread_rwlock_unlock(&qp->q_lock);
}
void job_remove(struct queue *qp,struct job *job){
    pthread_rwlock_wrlock(&qp->q_lock);
    if(job==qp->q_head){
        qp->q_head = job->j_next;
        if(qp->q_tail==job){
            qp->q_tail = NULL;  
        }else{
            job->j_next->j_prev = job->j_prev;
        }
    }else if(job==qp->q_tail){
        qp->q_tail = job->j_prev;
        job->j_prev->j_next = job->j_next;

    }else{
       job->j_prev->j_next = job->j_next;
       job->j_next->j_prev = job->j_prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}
struct job *job_find(struct queue *qp,pthread_t id){
    struct job *jp;
    if(pthread_rwlock_rdlock(&qp->q_lock)!=0){
        return NULL;
    }
    for(jp=qp->q_head;jp!=NULL;jp=jp->j_next){
         if(pthread_equal(jp->j_id,id)){
              break;
         }
    }
    pthread_rwlock_unlock(&qp->q_lock);
    return jp;
}
int main(int argc, char** argv){
    printf("main end tid=%lu.\n",pthread_self());
    return 0;
}

