
struct game_state;

struct thread_startup_params
{
  engine_api *EngineApi;
  game_api   *GameApi;

  engine_resources *EngineResources;

  volatile u32 *HighPriorityWorkerCount;

  bonsai_futex *HighPriorityModeFutex;
  bonsai_futex *WorkerThreadsSuspendFutex;
  bonsai_futex *WorkerThreadsExitFutex;

  work_queue *LowPriority;
  work_queue *HighPriority;

  volatile s32 ThreadIndex;
};

