#include <sys/time.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
int counter = 0;

void sig_handler(int signo) {
if (signo == SIGALRM)
    {
        counter ++;
        printf("Signal timer :%d\n", counter);
    }
}

int main()
{
    struct itimerval timer;

    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    if(signal(SIGALRM, sig_handler) == SIG_ERR)
        printf("Can't catch signal\n");
    setitimer(ITIMER_REAL, &timer, NULL);

    while(1)
        sleep(1);
        
    return 0;
    
}