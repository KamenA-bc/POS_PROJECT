#include <ucontext.h>
#include <stdio.h>
#include <malloc.h>

#define FIBER_STACK 1024*64

ucontext_t uctx_fiber_one, uctx_fiber_two, uctx_fiber_three, uctx_fiber_main;

void fiber_one()
{

	printf("Fiber1 started\n");
	printf("Swaping context to fiber2\n");
	swapcontext(&uctx_fiber_one, &uctx_fiber_two);

	printf("Fiber1 just started\n");
	printf("Swaping context to fiber2\n");
	swapcontext(&uctx_fiber_one, &uctx_fiber_two);

}

void fiber_two()
{
	printf("Fiber2 started\n");
	printf("Swaping context to fiber3\n");
	swapcontext(&uctx_fiber_two, &uctx_fiber_three);

	printf("Fiber2 just started again\n");
	printf("Swaping context to fiber3\n");
	swapcontext(&uctx_fiber_two, &uctx_fiber_three);


}

void fiber_three()
{
	printf("Fiber3 started\n");
	printf("Swaping context to fiber1\n");
	swapcontext(&uctx_fiber_three, &uctx_fiber_one);

	printf("Fiber3 just started\n");
	printf("Swapnig context to main\n");
	swapcontext(&uctx_fiber_three, &uctx_fiber_main);
}

int main()
{
	getcontext(&uctx_fiber_one);
	uctx_fiber_one.uc_link = NULL;

	uctx_fiber_one.uc_stack.ss_sp = malloc(FIBER_STACK);
	uctx_fiber_one.uc_stack.ss_size = FIBER_STACK;
	uctx_fiber_one.uc_stack.ss_flags = 0;

	makecontext(&uctx_fiber_one, fiber_one, 0);

	getcontext(&uctx_fiber_two);
	uctx_fiber_two.uc_link = NULL;

	uctx_fiber_two.uc_stack.ss_sp = malloc(FIBER_STACK);
	uctx_fiber_two.uc_stack.ss_size = FIBER_STACK;
	uctx_fiber_two.uc_stack.ss_flags = 0;

	makecontext(&uctx_fiber_two, fiber_two, 0);

	getcontext(&uctx_fiber_three);
	uctx_fiber_three.uc_link = NULL;

	uctx_fiber_three.uc_stack.ss_sp = malloc(FIBER_STACK);
	uctx_fiber_three.uc_stack.ss_size = FIBER_STACK;
	uctx_fiber_three.uc_stack.ss_flags = 0;

	makecontext(&uctx_fiber_three, fiber_three, 0);

	swapcontext(&uctx_fiber_main, &uctx_fiber_one);
	printf("Main received context\n");
	printf("Main exiting\n");
	
	return 0;
}
