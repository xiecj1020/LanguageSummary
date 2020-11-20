//编译方法:gcc -g pipe2.c -o pipe2
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#define MAXLINE 1000
#define PAGER "${PAGER:-more}"
int main(int argc, char** argv){
    FILE *fpin,*fpout;
    int n;
    int fd[2];
    pid_t pid;
    char *pager,*argv0;
    char line[MAXLINE];
    if(argc!=2){
       printf("usage:./pipe1 pathname.\n");
       return 0;
    }
    if((fpin=fopen(argv[1],"r"))==NULL){
       printf("can't open %s.\n",argv[1]);
       return 0;
    }
    if((fpout=popen(PAGER,"w"))==NULL){
       printf("can't open %s.\n",PAGER);
       return 0;
    }
    while(fgets(line,MAXLINE,fpin)!=NULL){
          if(fputs(line,fpout)==EOF){
	      printf("fputs error to pipe.\n");
              return 0; 
          }
    }
   if(ferror(fpin)!=0){
           printf("fgets error to pipe.\n");
           return 0; 
   }
   if(pclose(fpout)==-1){
        printf("pclose error.\n");
        return 0; 
   }

    return 0;
}

