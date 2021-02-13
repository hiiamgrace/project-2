/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * t1
 * t2
 * thread1[1]
 * t3
 * thread2[2]
 * t1 done
 * thread3[3]
 * all done
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread3(void)
{
	//printf("t3\n");
	uthread_yield();
	printf("thread3[%d]\n", uthread_self());
	return 0;
}

int thread2(void)
{
	uthread_create(thread3);
	//printf("t2\n");
	uthread_yield();
	printf("thread2[%d]\n", uthread_self());
	return 0;
}

int thread1(void)
{
	uthread_create(thread2);
	//printf("t1\n");
	uthread_yield();
	printf("thread1[%d]\n", uthread_self());
	uthread_yield();
	//printf("t1 done\n");
	return 0;
}

int main(void)
{
	uthread_start(0);
	uthread_join(uthread_create(thread1), NULL);
	//printf("all done\n");
	uthread_stop();

	return 0;
}
