#include <stdio.h>
#include <stdlib.h>
#include "task_descriptor.h"

typedef struct Node
{
	TaskDescriptor* task;
	struct Node* prev;
	struct Node* next;
}Node;


Node* createNode(int data)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->task->nID = data;
	newNode->prev = NULL;
	newNode->next = NULL;

	return newNode;
}

void insertAtEnd(Node** head, int data)
{
	Node* newNode = createNode(data);

	if(*head == NULL)
	{
		*head = newNode;
		return;
	}

	Node* temp = *head;
	while(temp->next != NULL)
	{
		temp = temp->next;
	}

	temp->next = newNode;
	newNode->prev = temp;
}


TaskDescriptor* extractFirst(Node** head)
{
	Node* temp = *head;

	if(temp == NULL)
	{
		printf("The queue is empty.\n");
		return NULL;
	}

	*head = (*head)->next;
	if(*head != NULL)
	{
		(*head)->prev = NULL;
	}
	TaskDescriptor* task_ptr = temp->task;
	free(temp);
	return task_ptr;
}


void deleteByDescriptor(Node** head, int data)
{
	Node* toDelete = *head;

	if(*head == NULL)
	{
		printf("The queue is already empty.\n");
		return;
	}

	while(toDelete != NULL)
	{
		if(toDelete->task->nID == data)
		{
			break;
		}

		toDelete = toDelete->next;
	}

	if(toDelete == NULL)
	{
		printf("No such element.\n");
		return;
	}

	if(toDelete->next != NULL)
	{
		toDelete->next->prev = toDelete->prev;
	}
	if(toDelete->prev != NULL)
	{
		toDelete->prev->next = toDelete->next;
	}
	if(toDelete == *head)
	{
		*head = (*head)->next;
	}
	free(toDelete);
}
