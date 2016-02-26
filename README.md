# A simple C thread pool implementation -- works with Win32 API

About
=====
The project is a fork of https://github.com/mbrossard/threadpool with added support for Win32 API. 
	
NOTE
====
The API's interface is similar to original one but the source is not compatible(the parts of Win32). If you need a POSIX implementation
of the library then you should check https://github.com/mbrossard/threadpool

Features
========
Currently, the implementation:
 * Starts all threads on creation of the thread pool.
 * Reserves one task for signaling the queue is full.
 * Stops and joins all worker threads on destroy.

Possible enhancements
=====================
The API contains addtional unused 'flags' parameters that would allow
some additional options:

 * Lazy creation of threads (easy)
 * Reduce number of threads automatically (hard)
 * Unlimited queue size (medium)
 * Kill worker threads on destroy (hard, dangerous)
 * Reduce locking contention (medium/hard)

Usage
=====
You can either build a static library of the source with the supplied Visual Studio solution or just drop-in the Header and Source filein your
code.
	
---
threadpool_t* pool = threadpool_create(NUMOFTHREADS, SIZEOFQUEUE, 0);

//to add a task to queue
threadpool_add( pool, &TASK, (void*)ARGS, 0);	//@note: task should have the signature void (*routine)(void*)
	
//to destroy the pool
threadpool_destroy( pool, FLAG);	//FLAG can be 0 or threadpool_graceful 
