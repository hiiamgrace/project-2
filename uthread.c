#include <assert.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

/*
 * uthread coroutines
 * @id: thread id
 * @ctx: User-level thread context
 * @stack: stack segment
 */
struct uco
{
	uthread_t id;
	uthread_ctx_t ctx;
	void* stack;
};

typedef struct uco* uco_t;

static queue_t uco_queue_ = NULL;
static queue_t all_queue_ = NULL;
static uco_t run_co_ = NULL;
static uthread_t unique_id_ = 0;

// generate a unique ID
uthread_t generate_unique_id()
{
	if (0 == ++unique_id_) unique_id_ = 1;
	return unique_id_;
}

// start the multithreading library
int uthread_start(int preempt)
{
	if (NULL != run_co_)
		return -1;
	uco_queue_ = queue_create();
	all_queue_ = queue_create();
	run_co_ = (uco_t)malloc(sizeof(struct uco));
	if (NULL == run_co_)
		return -1;
	run_co_->id = 0;
	run_co_->stack = NULL;
	queue_enqueue(all_queue_, run_co_);
	if (preempt)
		printf("uthread_start %d\n", preempt);
	return 0;
}

// stop the multithreading library
int uthread_stop(void)
{
	if (queue_destroy(uco_queue_))
		return -1;
	uco_queue_ = NULL;
	run_co_ = NULL;
	uco_t co = NULL;
	while (0 == queue_dequeue(all_queue_, (void**)&co))
	{
		if (NULL != co)
		{
			if (NULL != co->stack)
				uthread_ctx_destroy_stack(co->stack);
			free(co);
		}
	}
	all_queue_ = NULL;
	return 0;
}

// create a new thread
int uthread_create(uthread_func_t func)
{
	uco_t co = (uco_t)malloc(sizeof(struct uco));
	if (NULL == co)
		return -1;
	co->id = generate_unique_id();
	co->stack = uthread_ctx_alloc_stack();
	uthread_ctx_init(&co->ctx, co->stack, func);
	queue_enqueue(uco_queue_, co);
	queue_enqueue(all_queue_, co);
	return co->id;
}

// yield execution
void uthread_yield(void)
{
	uco_t next = NULL;
	if (queue_dequeue(uco_queue_, (void**)&next))
	{
		printf("uthread_yield ERROR [%d]\n", uthread_self());
		return;
	}
	queue_enqueue(uco_queue_, run_co_);
	uco_t prev = run_co_;
	run_co_ = next;
	uthread_ctx_switch(&prev->ctx, &next->ctx);
}

// get thread identifier
uthread_t uthread_self(void)
{
	if (NULL == run_co_)
		return -1;
	return run_co_->id;
}

// exit from currently running thread
void uthread_exit(int retval)
{
	uco_t next = NULL;
	if (queue_dequeue(uco_queue_, (void**)&next))
	{
		printf("uthread_yield ERROR [%d]\n", uthread_self());
		return;
	}
	uco_t prev = run_co_;
	run_co_ = next;
	uthread_ctx_switch(&prev->ctx, &next->ctx);
	printf("uthread_exit [%d] [%d]\n", prev->id, retval);
}

// join a thread
int uthread_join(uthread_t tid, int *retval)
{
	while (1)
	{
		if (0 == queue_length(uco_queue_))
			return 0;
		uthread_yield();
	}
	printf("uthread_join [%d] [%d]\n", tid, *retval);
	return 0;
}

