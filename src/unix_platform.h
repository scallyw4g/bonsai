#ifndef UNIX_PLATFORM_H
#define UNIX_PLATFORM_H

#include <pthread.h>
#include <signal.h>

#define Assert(condition) if (!(condition)) raise(SIGTRAP);

#define THREAD_MAIN_RETURN void*

typedef int thread_id;
typedef int semaphore;

#endif
