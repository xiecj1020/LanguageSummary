//编译方法:gcc -g pipe1.c -o pipe1
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#define MAXLINE 1000
#define DEF_PAGER "/bin/more"
int main(int argc, char** argv){
    int n;
    int fd[2];
    pid_t pid;
    char *pager,*argv0;
    char line[MAXLINE];
    FILE *fp;
    if(argc!=2){
       printf("usage:./pipe1 pathname.\n");
       return 0;
    }
    if((fp=fopen(argv[1],"r"))==NULL){
       printf("can't open %s.\n",argv[1]);
       return 0;
    }
    if(pipe(fd)<0){
       printf("pipe error.\n");
       return 0;
    }
    if((pid=fork())<0){
       printf("fork error.\n");
       return 0;
    }else if(pid>0){//parent process
       close(fd[0]);//close read end
       //parent copies argv[1] to pipe
       while(fgets(line,MAXLINE,fp)!=NULL){
	  n = strlen(line);
          if(write(fd[1],line,n)!=n){
	      printf("write error to pipe.\n");
              return 0; 
          }
       }
       if(ferror(fp)!=0){
           printf("fgets error to pipe.\n");
           return 0; 
       }
       close(fd[1]);
       printf("%d process wait %d process.\n",getpid(),pid);
       if(waitpid(pid,NULL,0)<0){
           printf("waitpid error.\n");
           return 0; 
        }
        printf("%d process exit.\n",getpid());
        exit(0);
   
    }else{
        close(fd[1]);
        if(fd[0]!=STDIN_FILENO){
           printf("fd[0] is not STDIN_FILENO %d %d.\n",fd[0],fd[1]);
           if(dup2(fd[0],STDIN_FILENO)!=STDIN_FILENO){
               printf("dup2 error.\n");
                return 0; 
           }
           close(fd[0]);
        }
        if((pager=getenv("PAGER"))==NULL){
		pager = DEF_PAGER;
        }
        printf("pager=%s\n",pager);
        argv0=strrchr(pager,'/');
        if(argv0!=NULL){
	    argv0++;
        }else{
            argv0 = pager;
        }
          printf("argv0=%s\n",argv0);
        if(execl(pager,argv0,(char*)0)<0){
 printf("%s error.\n",pager);
exit(0);
        }
    }
    return 0;
}

