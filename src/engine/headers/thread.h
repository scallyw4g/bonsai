
struct game_state;

struct thread_startup_params
{
  bonsai_worker_thread_init_callback InitProc;
  bonsai_worker_thread_callback GameWorkerThreadCallback;

  engine_resources *EngineResources;

  volatile u32 *HighPriorityWorkerCount;

  bonsai_futex *WorkerThreadsSuspendFutex;
  bonsai_futex *WorkerThreadsExitFutex;

  work_queue *LowPriority;
  work_queue *HighPriority;

  volatile u32 ThreadIndex;
  volatile u32 ThreadId;
  /* volatile thread_handle ThreadHandle; */
};

struct mesh_freelist;
struct thread_local_state
{
  memory_arena  *PermMemory;
  memory_arena  *TempMemory;
  mesh_freelist *MeshFreelist;
  perlin_noise  Noise;
};

