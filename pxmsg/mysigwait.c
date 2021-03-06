/*************************************************************************
	> File Name: mysigwait.c
	> Author: Lin
	> Mail: 455477402@qq.com
	> Created Time: Thu Jul 27 23:11:35 2017
 ************************************************************************/

#include"unpipc.h"


int main(int argc,char** argv)
{
    int signo;
    mqd_t mqd;
    void* buff;
    ssize_t n;
    sigset_t newmask;
    struct mq_attr attr;
    struct sigevent sigev;

    if(argc!=2)
        err_quit("usage error");

    mqd=Mq_open(argv[1],O_RDONLY|O_NONBLOCK);
    Mq_getattr(mqd,&attr);
    buff=Malloc(attr.mq_msgsize);

    Sigemptyset(&newmask);
    Sigaddset(&newmask,SIGUSR1);
    Sigpromask(SIG_BLOCK,&newmask,NULL);
    
    sigev.sigev_notify=SIGEV_SIGNAL;
    sigev.sigev_signo=SIGUSR1;
    Mq_notify(mqd,&sigev);

    while(1)
    {
        Sigwait(&newmask,&signo);
        if(signo==SIGUSR1)
        {
            Mq_notify(mqd,&sigev);
            while((n=mq_receiver(mqd,buff,attr.mq_msgsize,NULL))>=0)
            {
               printf("read %ld bytes\n",(long) n);
            }
            if(errno!=EAGAIN)
            {
                err_sys("error");
            }

        }

    }
    exit(0);
}
