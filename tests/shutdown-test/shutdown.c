#include <stdio.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "threadpool.h"

//Link to ThreadPool.lib
#pragma comment(lib, "ThreadPool.lib")

#define THREAD 4
#define SIZE   8192

threadpool_t *pool;
int left;
CRITICAL_SECTION lock;

int error;

void dummy_task(void *arg) {
    Sleep(100);
	EnterCriticalSection(&lock);
    left--;
	LeaveCriticalSection(&lock);
}

int main(int argc, char **argv)
{
    int i;

	InitializeCriticalSection(&lock);

    /* Testing immediate shutdown */
    left = SIZE;
    pool = threadpool_create(THREAD, SIZE, 0);
    for(i = 0; i < SIZE; i++) {
        assert(threadpool_add(pool, &dummy_task, NULL, 0) == 0);
    }
    assert(threadpool_destroy(pool, 0) == 0);
    assert(left > 0);

    /* Testing graceful shutdown */
    left = SIZE;
    pool = threadpool_create(THREAD, SIZE, 0);
    for(i = 0; i < SIZE; i++) {
        assert(threadpool_add(pool, &dummy_task, NULL, 0) == 0);
    }
    assert(threadpool_destroy(pool, threadpool_graceful) == 0);
    assert(left == 0);

	DeleteCriticalSection(&lock);

    return 0;
}
