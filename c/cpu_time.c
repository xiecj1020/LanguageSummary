#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <stdlib.h>
/***
* 打印某段代码执行的时间，包括3个值:
* 1.这段代码执行总的花费时间
* 2.这段代码用户态CPU花费的时间
* 3.这段代码系统太CPU花费时间
*/ 
int main(void){
    //获取滴答数
    int clktck = 0;
    if ((clktck = sysconf(_SC_CLK_TCK)) < 0) {
        printf("%s\n", "sysconf error");
        exit(0);
    }
    printf("clktck=%d\n", clktck);
    struct tms  tmsstart, tmsend;
    clock_t     start, end;
     
    //获取开始时间
    if ((start = times(&tmsstart)) == -1) {
        printf("%s\n", "times error");
        exit(0);
    }
    //测试代码，循环10000次调用打印方法
    int i=0;
    for(i=0;i<10000;i++){
       printf("abc\n");
    }
    //获取结束时间
    if ((end = times(&tmsend)) == -1) {
        printf("%s\n", "times error");
        exit(0);
    }
 
    printf("real: %7.5f\n", (end - start)/(double) clktck);
    printf("user: %7.5f\n",
            (tmsend.tms_utime - tmsstart.tms_utime)/(double) clktck);
    printf("sys:  %7.5f\n",
            (tmsend.tms_stime - tmsstart.tms_stime)/(double) clktck);
    printf("child user: %7.5f\n",
            (tmsend.tms_cutime - tmsstart.tms_cutime)/(double) clktck);
    printf("child sys:  %7.5f\n",
            (tmsend.tms_cstime - tmsstart.tms_cstime)/(double) clktck);
 
    return 0;
}
