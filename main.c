#define _GNU_SOURCE

#include <ucontext.h>
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#define FIBER_STACK 1024*64

ucontext_t uctx_fiber_main;


typedef struct task_t
{
    ucontext_t context;
	void (*func)();
    struct task_t *next;
    struct task_t *prev;
}task_t;

task_t *taskQueue = NULL;
task_t *runningTask = NULL;

void push(task_t** head, task_t* newNode);

int create_task(void (*start_routine)()) {

    task_t* newNode = (task_t*)malloc(sizeof(task_t));
    if (newNode == NULL) {
        return -1;
    }

    newNode->func = start_routine;
    newNode->next = NULL;
    newNode->prev = NULL;

    getcontext(&newNode->context);
    
    newNode->context.uc_link = &uctx_fiber_main;
    newNode->context.uc_stack.ss_sp = malloc(FIBER_STACK);
    newNode->context.uc_stack.ss_size = FIBER_STACK;
    newNode->context.uc_stack.ss_flags = 0;
    

    makecontext(&newNode->context, start_routine, 0);

	push(&taskQueue, newNode);
    return 0;
}

void push(task_t** head, task_t* newNode)
{
	if(newNode == NULL)
	{
		return;
	}

	if(*head == NULL)
	{
		*head = newNode;
		return;
	}

	task_t* temp = *head;
	while(temp->next != NULL)
	{
		temp = temp->next;
	}

	temp->next = newNode;
	newNode->prev = temp;
}

task_t* pop(task_t** head)
{
    if (*head == NULL)
    {
        return NULL;
    }

    task_t* temp = *head;

    *head = (*head)->next;

    if (*head != NULL)
    {
        (*head)->prev = NULL;
    }

    temp->next = NULL;
    temp->prev = NULL;

    return temp;
}



void fiber_one()
{
    int local_counter = 0;
	while(true)
	{
        local_counter++;
        printf("+ (counter: %d)\n", local_counter);
		printf("+\n");
        for(int i = 0; i<100000000; i++);
	}
}

void fiber_two()
{
	while(true)
	{
		printf("-\n");
        for(int i = 0; i<100000000; i++);
	}
}

void fiber_three()
{
	while(true)
	{
		printf("!\n");
        for(int i = 0; i<100000000; i++);
	}
}


void sig_handler(int signo)
{
    if(signo == SIGALRM)
    {
        task_t *interruptedTask = runningTask;

        if (runningTask != NULL) {
            push(&taskQueue, runningTask);
        }

        runningTask = pop(&taskQueue);

        if (runningTask != NULL) {
            if (interruptedTask == NULL) {
                swapcontext(&uctx_fiber_main, &runningTask->context);
            } else {
                swapcontext(&interruptedTask->context, &runningTask->context);
            }
        }
    }
}

int main()
{
    struct itimerval timer;

    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

	create_task(&fiber_one);
	create_task(&fiber_two);
	create_task(&fiber_three);

    runningTask = NULL;

    if(signal(SIGALRM, sig_handler)) {
        printf("Can't catch signal\n");
        return 1;
    }
    setitimer(ITIMER_REAL, &timer, NULL);

    while(true) {
        sleep(1);
    }
	return 0;
}
