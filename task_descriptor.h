#pragma once

typedef struct
{
	int nID;
}TaskDescriptor;

TaskDescriptor* create_task(int id);
void destroy_task(TaskDescriptor* task);

