#include <ucontext.h>
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

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
	while(true)
	{
		printf("+\n");
		fflush(stdout);
		swapcontext(&runningTask->context, &uctx_fiber_main);
	}
}

void fiber_two()
{
	while(true)
	{
		printf("-\n");
		swapcontext(&runningTask->context, &uctx_fiber_main);
	}
}

void fiber_three()
{
	while(true)
	{
		printf("!\n");
		swapcontext(&runningTask->context, &uctx_fiber_main);
	}
}

int main()
{
	create_task(&fiber_one);
	create_task(&fiber_two);
	create_task(&fiber_three);
// Initialize RunningTask to NULL before starting the loop
    runningTask = NULL;

    while(true) {
        // Algorithm Step 1: ТСВ на текущата задача (сочена от RunningTask) се добавя в края на опашката.
        // (If a task was just running, put it at the back of the line)
        if (runningTask != NULL) {
            push(&taskQueue, runningTask);
        }

        // Algorithm Step 2: Извлича се от началото на опашката първия TCB и се указва да се сочи от RunningTask.
        // (Take the next task from the front of the line)
        runningTask = pop(&taskQueue);

        // Algorithm Step 3: Превключва се контекста към задачата, сочена от RunningTask.
        // (Save main's context, and jump to the task's context)
        if (runningTask != NULL) {
            swapcontext(&uctx_fiber_main, &runningTask->context);
        } else {
            // Safety fallback: if the queue is empty, exit the loop
            break; 
        }
    }
	return 0;
}
