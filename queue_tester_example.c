#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

/* Callback function that increments integer items by a certain value (or delete
 * item if item is value 42) */
static int inc_item(queue_t q, void* data, void* arg)
{
	int* a = (int*)data;
	int inc = (int)(long)arg;

	if (*a == 42)
		queue_delete(q, data);
	else
		*a += inc;

	return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(queue_t q, void* data, void* arg)
{
	int* a = (int*)data;
	int match = (int)(long)arg;
	(void)q; //unused

	if (*a == match)
		return 1;

	return 0;
}

/* iterator simple */
void test_iterator(void)
{
	queue_t q;
	int data[] = { 1, 2, 3, 4, 5, 42, 6, 7, 8, 9 };
	size_t i;
	int* ptr;

	/* Initialize the queue and enqueue items */
	q = queue_create();
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
		queue_enqueue(q, &data[i]);

	fprintf(stderr, "*** TEST iterator ***\n");

	/* Add value '1' to every item of the queue, delete item '42' */
	queue_iterate(q, inc_item, (void*)1, NULL);
	TEST_ASSERT(data[0] == 2);
	TEST_ASSERT(queue_length(q) == 9);

	/* Find and get the item which is equal to value '5' */
	ptr = NULL;     // result pointer *must* be reset first
	queue_iterate(q, find_item, (void*)5, (void**)&ptr);
	TEST_ASSERT(ptr != NULL);
	TEST_ASSERT(*ptr == 5);
	TEST_ASSERT(ptr == &data[3]);
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_iterator();

	return 0;
}
