/*************************************************************************
	> File Name: myprodcons.c
	> Author: Lin
	> Mail: 455477402@qq.com
	> Created Time: Fri Aug 11 22:11:56 2017
 ************************************************************************/

#include"unpipc.h"
#define MAXNITEMS 100000
#define MAXNTHREADS 100

int nitems;
struct{
    pthread_mutex_t mutex;
    int buff[MAXNITEMS];
    int nput;
    int nval;
}shared={
    PTHREAD_MUTEX_INITALIZER
};

void *produce(void*),*consume(void*);

int main(int argc,char** argv)
{
    int i,nthreads,count[MAXNTHREADS];
    pthread_t tid_produce[MAXNTHREADS],tid_consume;
    
    if(argc!=3)
        err_quit("error");
    nitems=min(atoi(argv[1]),MAXNITEMS);
    nthreads=min(atoi(argv[2]),MAXNTHREADS);

    Ser_concurrency(nthreads);

    for(i=0;i<nthreads;i++)
    {
        count[i]=0;
        Pthread_create(&tid_produce[i],NULL,produce,&count[i]);
    }

    for(i=0;i<nthreads;i++)
    {
        Pthread_join(tid_produce[i],NULL);
        printf("count[%d]=%d\n",i,count[i]);
    }

    Pthread_create(&tid_consume,NULL,consume,NULL);
    Pthread_join(tid_consume,NULL);

    exit(0);
    
}

void* produce(void* arg)
{
    for(;;)
    {
        Pthread_mutex_lock(&shared.mutex);
        if(shared.nput>=nitems)
        {
            Pthread_mutex_unlock(&shared.mutex);
            return(NULL);
        }
        shared.buff[shared.nput]=shared.nval;
        shared.nput++;
        shared.nval++;
        Pthread_mutex_unlock(&shared.mutex);
        *((int*)arg)+=1;
    }
}

void* consume(void* arg)
{
    int i;
    for(i=0;i<nitems;i++)
    {
        if(shared.buff[i]!=i)
        {
            printf("buff[%d]=%d\n",i,shared.buff[i]);
        }
    }
    return NULL;
}

