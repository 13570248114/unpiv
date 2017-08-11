/*************************************************************************
	> File Name: mymainserver.c
	> Author: Lin
	> Mail: 455477402@qq.com
	> Created Time: Wed Jul 26 12:20:38 2017
 ************************************************************************/

#include"fifo.h"
void server(int,int);

int main(int argc,char** argv)
{
    int readfifo,wrritefifo,dummyfd,fd;
    char *ptr,buff[MAXLINE+1],fifoname[MAXLINE];
    pid_t pid;
    ssize_t n;

    if((mkfifo(SERV_FIFO,FILE_MODE)<0))&&(errno!=EEXIST))
        err_sys("cant create %s",SERV_FIFO);

    readfifo=Open(SERV_FIFO,O_RDONLY,0);
    dummyfd=Open(SERV_FIFO,O_WRONLY,0);

    while((n=Readline(readfifo,buff,MAXLINE))>0)
    {
        if(buff[n-1]=='\n')
            n--;
        buff[n]='\0';

        if((ptr=strchr(buff,' '))==NULL)
        {
            err_msg("bogus request :%s",buff);
            continue;
        }
        *ptr++=0;
        pid=atol(buff);
        snprintf(fifoname,sizeof(fifoname),"/tmp/fifo.%ld",(long)pid);
        if((wrritefifo=open(fifoname,O_WRONLY,0))<0)
        {
            err_msg("cant not open file%s",fifoname);
            continue;
        }

        if((fd=open(ptr,O_RDONLY))<0)
        {
            snprintf(buff+n,sizeof(buff)-n,"cant open %s",strerror(errno));
            n=strlen(ptr);
            Write(wrritefifo,ptr,n);
            Close(wrritefifo);
        }
        else
        {
            while((n=Read(fd,buff,MAXLINE))>0)
                Write(wrritefifo,buff,n);
            Close(fd);
            Close(wrritefifo);
        }
    }
    exit(0);
}

