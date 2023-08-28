
struct game_state;

struct thread_startup_params
{
  engine_api *EngineApi;
  game_api   *GameApi;
  /* bonsai_worker_thread_init_callback InitProc; */
  /* bonsai_worker_thread_callback GameWorkerThreadCallback; */

  engine_resources *EngineResources;

  volatile u32 *HighPriorityWorkerCount;

  bonsai_futex *HighPriorityModeFutex;
  bonsai_futex *WorkerThreadsSuspendFutex;
  bonsai_futex *WorkerThreadsExitFutex;

  work_queue *LowPriority;
  work_queue *HighPriority;

  volatile s32 ThreadIndex;
  /* volatile u32 ThreadId; */
  /* volatile thread_handle ThreadHandle; */
};

