/*************************************************************************
	> File Name: mymainclient.c
	> Author: Lin
	> Mail: 455477402@qq.com
	> Created Time: Wed Jul 26 20:25:34 2017
 ************************************************************************/

#include"fifo.h"

int main(int argc,char** argv)
{
    int readfifo,writefifo;
    size_t len;
    ssize_t n;
    char *ptr, fifoname[MAXLINE],buff[MAXLINE];
    pid_t pid;

    pid=getpid();
    snprintf(fifoname,sizeof(fifoname),"/tmp/fifo.%ld",(long)pid);
    if((mkfifo(fifoname,FILE_MODE)<0)&&(errno!=EEXIST))
        err_sys("cant creat%s",fifoname);

    snprintf(buff,sizeof(buff),"%ld",(long)pid);
    len=strlen(buff);
    ptr=buff+len;

    Fgets(ptr,MAXLINE-len,stdin);
    len=strlen(buff);

    writefifo=Open(SERV_FIFO,O_WDONLY,0);
    Write(writefifo,buff,len);

    readfifo=Open(fifoname,O_RDONLY,0);

    while((n=Read(readfifo,buff,MAXLINE))>0)
        Write(STDOUT_FILENO,buff,n);

    Close(readfifo);
    Unlink(fifoname);
    exit(0);

}

