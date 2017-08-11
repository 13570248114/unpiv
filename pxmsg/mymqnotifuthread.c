/*************************************************************************
	> File Name: mymqnotifuthread.c
	> Author: Lin
	> Mail: 455477402@qq.com
	> Created Time: Sat Jul 29 23:18:11 2017
 ************************************************************************/

#include"unpipc.h"

mqt_t mqd;
struct mq_attr attr;
struct sigevent sigev;

static void notify_thread(union sigval);

int main(int argc,char** argv)
{
    if(argc!=2)
        err_quit("error");
    mqd=Mq_open(argv[1],O_RDONLY|O_NONBLOCK);
    Mq_getattr(mqd,&attr);

    sigev.sigev_notify=SIGEV_THREAD;
    sigev.sigev_value.sival_ptr=NULL;
    sigev.sigev_notify_function=notify_thread;
    sigev.sigev_notify_attributes=NULL;
    Mq_notify(mqd,&sigev);

    while(1)
    {
        pause();
    }
    exit(0);
}

static void notify_thread(union sigval arg)
{
    ssize_t n;
    void* buff;
    printf("notify_thread\n");
    buff=Malloc(attr.mq_msgsize);
    Mq_notify(mqd,&sigev);

    while((n=mq_receive(mqd,buff,attr.mq_msgsize,NULL))>=0)
    {
        printf("read %ld",(long)n);
    }
    if(errno!=EAGAIN)
        err_sys("mq_receive error");
    free(buff);
    pthread_exit(NULL);
}
