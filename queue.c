#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/*
 * queue nodes. doubly linked lists
 * @prev: previous node
 * @next: next node
 * @data: data pointer
 */
struct queue_node {
	struct queue_node* prev;
	struct queue_node* next;
	void* data;
};
typedef struct queue_node* queue_node_t;

/*
 * queue
 * @head: first node
 * @tail: last node
 * @len: queue length
 */
struct queue {
	queue_node_t head;
	queue_node_t tail;
	int len;
};

// allocate an empty queue
queue_t queue_create(void)
{
	queue_t q = (queue_t)malloc(sizeof(struct queue));
	if (NULL != q)
		memset(q, 0, sizeof(struct queue));
	return q;
}

// deallocate a queue
int queue_destroy(queue_t queue)
{
	if (NULL == queue || 0 != queue->len)
		return -1;
	free(queue);
	return 0;
}

// enqueue data to tail
int queue_enqueue(queue_t queue, void *data)
{
	if (NULL == queue || NULL == data)
		return -1;
	queue_node_t q = (queue_node_t)malloc(sizeof(struct queue_node));
	if (NULL == q)
		return -1;
	memset(q, 0, sizeof(struct queue_node));
	q->data = data;
	if (NULL == queue->head)
	{
		queue->head = q;
	}
	else
	{
		q->prev = queue->tail;
		q->prev->next = q;
	}
	queue->tail = q;
	++queue->len;
	return 0;
}

// dequeue data from head
int queue_dequeue(queue_t queue, void **data)
{
	if (NULL == queue || NULL == data || 0 == queue->len)
		return -1;
	queue_node_t q = queue->head;
	queue_node_t t = q->next;
	queue->head = t;
	if (NULL == t)
		queue->tail = NULL;
	else
		t->prev = NULL;
	--queue->len;
	*data = q->data;
	free(q);
	return 0;
}

// find the first node equal to @data and delete it
int queue_delete(queue_t queue, void *data)
{
	if (NULL == queue || NULL == data || 0 == queue->len)
		return -1;
	queue_node_t q = queue->head;
	while (NULL != q)
	{
		if (q->data != data)
		{
			q = q->next;
			continue;
		}
		--queue->len;
		if (0 == queue->len)
			queue->head = queue->tail = NULL;
		else if (NULL == q->prev)
			queue->head = q->next;
		else
			q->prev->next = q->next;
		free(q);
		return 0;
	}
	return -1;
}

// iterate through a queue
int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if (NULL == queue || NULL == func)
		return -1;
	queue_node_t t, q = queue->head;
	void* ptr;
	while (NULL != q)
	{
		t = q->next;
		ptr = q->data;
		if (func(queue, ptr, arg))
		{
			if (NULL != data)
				*data = ptr;
			break;
		}
		q = t;
	}
	return 0;
}

// queue length
int queue_length(queue_t queue)
{
	if (NULL == queue)
		return -1;
	return queue->len;
}

