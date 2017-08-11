/*************************************************************************
	> File Name: mytest1.c
	> Author: Lin
	> Mail: 455477402@qq.com
	> Created Time: Sun Jul 30 14:16:40 2017
 ************************************************************************/

#include"unpipc.h"
static void sig_rt(int,siginfo_t,void*);

int main(int argc,char** argv)
{
    int i,j;
    pid_t pid;
    sigset_t newset;
    union sigval val;

    printf("SIGRTMIN=%d,SIGRTMAX=%d\n",(int)SIGRTMIN,(int)SIGRTMAX);
    if((pid=Fork())==0)
    {
        Sigemptyset(&newset);
        Sigaddset(&newset,SIGRTMAX);
        Sigaddset(&newset,SIGRTMAX-1);
        Sigaddset(&newset,SIGRTMAX-2);
        Sigprocmask(SIG_BLOCK,&newset,NULL);

        Signal_rt(SIGRTMAX,sig_rt,&newset);
        Signal_rt(SIGRTMAX-1,sig_rt,&newset);
        Signal_rt(SIGRTMAX-2,sig_rt,&newset);

        sleep(6);
        Sigprocmask(SIG_UNBLOCK,&newset,NULL);
        sleep(3);
        exit(0);
    }

    sleep(3);
    for(i=SIGRTMAX;i>=SIGRTMAX-2;i--)
    {
        for(j=0;j<=2;j++)
        {
            val.sival_int=j;
            Sigqueue(pid,i,val);
            printf("sent sigval %d,val=%d\n",i,j);
        }
    }
    exit(0);
}

static void sig_rt(int signo,siginfo_t *info,void* context)
{
    printf("receiver signal ##%d,code=%d,ival=%d\n",signo,info->si_code,info->si_value.sival_int);
}


