/*************************************************************************
	> File Name: mymqcreate.c
	> Author: Lin
	> Mail: 455477402@qq.com
	> Created Time: Wed Jul 26 22:58:05 2017
 ************************************************************************/

#include"unpipc.h"

int main(int argc,char** argv)
{
    int c,flags;
    mqd_t mqd;

    flags=O_RDWD|O_CREAT;
    while((c=Getopt(argc,argv,"e"))!=-1)
    {
        switch(c)
        {
            case 'e':
                flags|=O_EXCL;
                break;
        }
    }
    if(optind!=argc-1)
        err_quit("usage: mqcreate [-e] <name>");
    mqd=Mq_open(argv[optind],flags,FILE_MODE,NULL);

    Mq_close(mqd);
    exit(0);
}

