//使用gcc和gdb调试步骤 
//1.为了能够使用gdb调试程序，在编译阶段加上-g参数，
//2.使用gdb命令调试程序：gdb ./pthread_test3
//3.进入gdb后，运行程序： 
//  (gdb) run
//4.完成调试后，输入quit命令退出gdb：
//  (gdb) quit
//编译方法:gcc pthread_test3.c -o pthread_test3 -lpthread
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
struct foo{
    int a,b,c,d;
};
void printfoo(const char *s,const struct foo *fp){
    printf("%s",s);
    printf(" structure at 0x%lx\n",(unsigned long)fp);
    printf(" foo.a = %d\n",fp->a);
    printf(" foo.b = %d\n",fp->b);
    printf(" foo.c = %d\n",fp->c);
    printf(" foo.d = %d\n",fp->d);
}
void *thr_fn1(void *args){
    struct foo f = {1,2,3,4};
    printfoo("thread1:\n",&f);
    return ((void*)&f);
}
void *thr_fn2(void *args){
    printf("thread 2 id is %lu.\n",(unsigned long)pthread_self());
    pthread_exit((void*)0);
}
int main(int argc, char** argv){
    int err;
    pthread_t tid1,tid2;
    struct foo *fp;
    err = pthread_create(&tid1,NULL,thr_fn1,NULL);
    if(err!=0){
       printf("Can't create thread 1.\n");
       return 0;
    }
    err = pthread_join(tid1,(void**)&fp);
    if(err!=0){
       printf("Can't join thread 1.\n");
       return 0;
    }
    sleep(1);
    printf("Parent start thread 2.\n");
    err = pthread_create(&tid2,NULL,thr_fn2,NULL);
    if(err!=0){
       printf("Can't create thread 2.\n");
       return 0;
    }
    printfoo("parent:\n",fp);
    printf("main over.\n");
    return 0;
}

