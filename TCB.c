#include <stdio.h>
#include <stdlib.h> 
#include "TCB.h"
#include "task_queue.c"

#define STACK_SIZE 1024*64

int create_task(void (*start_routine)())
{
    ucontext_t uctx_func;
    getcontext(&uctx_func);
    uctx_func.uc_link = &
}

void destroy_task(task_t* task) {
    if (task) {
        free(task); 
    }
}