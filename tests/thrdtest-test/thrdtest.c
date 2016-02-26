#define THREAD 32
#define QUEUE  256

#include <stdio.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "threadpool.h"

//Link to ThreadPool.lib
#pragma comment(lib, "ThreadPool.lib")


int tasks = 0, done = 0;
CRITICAL_SECTION lock;

void dummy_task(void *arg) {
    Sleep(10000);
	EnterCriticalSection(&lock);
    done++;
    LeaveCriticalSection(&lock);
}

int main(int argc, char **argv)
{
    threadpool_t *pool;

	InitializeCriticalSection(&lock);

    assert((pool = threadpool_create(THREAD, QUEUE, 0)) != NULL);
    fprintf(stderr, "Pool started with %d threads and "
            "queue size of %d\n", THREAD, QUEUE);

    while(threadpool_add(pool, &dummy_task, NULL, 0) == 0) {
		EnterCriticalSection(&lock);
        tasks++;
		LeaveCriticalSection(&lock);
    }

    fprintf(stderr, "Added %d tasks\n", tasks);

    while((tasks / 2) > done) {
        Sleep(10000);
    }
    assert(threadpool_destroy(pool, 0) == 0);
    fprintf(stderr, "Did %d tasks\n", done);

    return 0;
}
