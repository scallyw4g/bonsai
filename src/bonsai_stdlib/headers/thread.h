#define ReadBarrier  asm volatile("" ::: "memory"); _mm_lfence()
#define WriteBarrier asm volatile("" ::: "memory"); _mm_sfence()
#define FullBarrier  asm volatile("" ::: "memory"); _mm_sfence(); _mm_lfence()

global_variable volatile b32 MainThreadBlocksWorkerThreads;
global_variable volatile u32 WorkerThreadsWaiting;

bonsai_function u32 GetWorkerThreadCount();
bonsai_function u32 GetTotalThreadCount();
bonsai_function void SuspendWorkerThreads();

typedef THREAD_MAIN_RETURN (*thread_main_callback_type)(void*);

