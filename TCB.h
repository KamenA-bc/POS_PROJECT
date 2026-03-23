#pragma once
#include <ucontext.h>

typedef struct task_t
{
    ucontext_t context;
    void (*func);
    struct task_t *next;
    struct task_t *prev;
}task_t;

int create_task(void (*start_routine)());
void destroy_task(task_t* task);