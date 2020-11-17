#include<stdio.h>
#include<unistd.h>
#define BUFFER 100
//一.标准I/O的缓存--标准输出为例：（这里都是指缺省情况下）
//(1)当STDOUT连接到终端设备时，那么它就是行缓存的，也就是标准IO库每看到一个新行符“\n”时就刷新一次缓存（即执行一次实际的输出操作）。
//这一特性可以通过如下测试代码来验证
//(2)当STDOUT被重定向到一个具体文件时，那么标准输出是全缓存的，也就是说只有当输出缓存被塞满或者调用fflush或fclose时才会执行实际的写入操作，这里就不给出具体例子.
//可以通过freopen将STDOUT重定向到一个具体文件来进行测试。
//二.标准出错STDERR：为了尽快的看到出错信息，标准出错是不带任何缓存的
int main(int argc, char** argv){
      printf("This first Line Should be Cached...");
      //这时候在终端上是看不到任何输出
      sleep(3);
      printf("\nThis second Line Should be Cached Again");
       //这时候可以看到第一个printf的输出，因为被换行符刷新了
      sleep(3);
      printf("This third Line Should Not be Cached Again\n");
       //这时候可以看到第二个和第三个printf的输出，因为被结尾的/n刷新     
      sleep(3);
      getchar();
    return 0;
}
