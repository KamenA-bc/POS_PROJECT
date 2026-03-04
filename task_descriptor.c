#include <stdlib.h>
#include <stdio.h>
#include "task_descriptor.h"

TaskDescriptor* create_task(int id) {
    TaskDescriptor* new_task = (TaskDescriptor*)malloc(sizeof(TaskDescriptor)); 
    
    new_task->nID = id;
    
    return new_task;
}

void destroy_task(TaskDescriptor* task) {
    if (task != NULL) {
        free(task); 
    }
}
