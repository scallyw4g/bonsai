#if BONSAI_INTERNAL

#define DEBUG_FRAMES_TRACKED (64)
#define MAX_TABLE_COLUMNS 4
#define MAX_SELECTED_ARENAS 128
#define REGISTERED_MEMORY_ARENA_COUNT 32
#define META_TABLE_SIZE (16 * 1024)
#define DEBUG_RECORD_INPUT_SIZE 3600

#define TIMED_FUNCTION() debug_timed_function FunctionTimer(BONSAI_FUNCTION_NAME)
#define TIMED_BLOCK(BlockName) { debug_timed_function BlockTimer(BlockName)
#define END_BLOCK(BlockName) }

#define DEBUG_FRAME_RECORD(...) DoDebugFrameRecord(__VA_ARGS__)
#define DEBUG_FRAME_END(Plat, GameState) GetDebugState()->FrameEnd(Plat, GameState)
#define DEBUG_FRAME_BEGIN(Hotkeys, dt) GetDebugState()->FrameBegin(Hotkeys, dt)

#ifndef BONSAI_NO_MUTEX_TRACKING
#define TIMED_MUTEX_WAITING(Mut)  GetDebugState()->MutexWait(Mut)
#define TIMED_MUTEX_AQUIRED(Mut)  GetDebugState()->MutexAquired(Mut)
#define TIMED_MUTEX_RELEASED(Mut) GetDebugState()->MutexReleased(Mut)
#else
#define TIMED_MUTEX_WAITING(...)
#define TIMED_MUTEX_AQUIRED(...)
#define TIMED_MUTEX_RELEASED(...)

#endif

#define WORKER_THREAD_ADVANCE_DEBUG_SYSTEM(...) GetDebugState()->WorkerThreadAdvanceDebugSystem()
#define MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(...) GetDebugState()->MainThreadAdvanceDebugSystem()
#define SUSPEND_WORKER_THREADS()  DebugSuspendWorkerThreads()
#define RESUME_WORKER_THREADS()  DebugResumeWorkerThreads()

#define DEBUG_CLEAR_META_RECORDS_FOR(Arena) GetDebugState()->ClearMetaRecordsFor(Arena)

#else

#define TIMED_FUNCTION(...)
#define TIMED_BLOCK(...)
#define END_BLOCK(...)

#define TIMED_MUTEX_WAITING(...)
#define TIMED_MUTEX_AQUIRED(...)
#define TIMED_MUTEX_RELEASED(...)

#define DEBUG_FRAME_RECORD(...)
#define DEBUG_FRAME_END(...)
#define DEBUG_FRAME_BEGIN(...)

#define WORKER_THREAD_WAIT_FOR_DEBUG_SYSTEM(...)
#define MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(...) RealDt
#define SUSPEND_WORKER_THREADS(...)

#endif

