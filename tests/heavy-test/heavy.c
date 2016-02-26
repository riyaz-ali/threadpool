#include <stdio.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "threadpool.h"

//Link against the ThreadPool.lib
#pragma comment(lib, "ThreadPool.lib")


#define THREAD 4
#define SIZE   8192
#define QUEUES 64

/*
 * Warning do not increase THREAD and QUEUES too much on 32-bit
 * platforms: because of each thread (and there will be THREAD *
 * QUEUES of them) will allocate its own stack (8 MB is the default on
 * Linux), you'll quickly run out of virtual space.
 */

threadpool_t *pool[QUEUES];
int tasks[SIZE], left;
CRITICAL_SECTION lock;


int error;

void dummy_task(void *arg) {
    int *pi = (int *)arg;
    *pi += 1;

    if(*pi < QUEUES) {
        assert(threadpool_add(pool[*pi], &dummy_task, arg, 0) == 0);
    } else {
		EnterCriticalSection(&lock);
        left--;
		LeaveCriticalSection(&lock);
    }
}

int main(int argc, char **argv)
{
    int i, copy = 1;

    left = SIZE;
	InitializeCriticalSection(&lock);

    for(i = 0; i < QUEUES; i++) {
        pool[i] = threadpool_create(THREAD, SIZE, 0);
        assert(pool[i] != NULL);
    }

    Sleep(10);

    for(i = 0; i < SIZE; i++) {
        tasks[i] = 0;
        assert(threadpool_add(pool[0], &dummy_task, &(tasks[i]), 0) == 0);
    }

    while(copy > 0) {
        Sleep(10);
		EnterCriticalSection(&lock);
        copy = left;
		LeaveCriticalSection(&lock);
    }

    for(i = 0; i < QUEUES; i++) {
        assert(threadpool_destroy(pool[i], 0) == 0);
    }

	DeleteCriticalSection(&lock);

    return 0;
}
