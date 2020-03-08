struct thread
{
  u32 ThreadIndex;
  thread_id ID;
};

struct mesh_freelist;
struct thread_local_state
{
  memory_arena*  PermMemory;
  memory_arena*  TempMemory;
  mesh_freelist* MeshFreelist;
  perlin_noise*  Noise;
};

struct game_state;

#define EXPAND(E) #E
#define TO_STRING(Text) EXPAND(Text)

#define BONSAI_API_MAIN_THREAD_CALLBACK_NAME          MainThreadCallback
#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME     InitMainThreadCallback
#define BONSAI_API_WORKER_THREAD_CALLBACK_NAME        WorkerThreadCallback
#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME   InitWorkerThreadCallback

#define BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS         platform *Plat, game_state *GameState, hotkeys *Hotkeys
#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS    platform *Plat, memory_arena *GameMemory, get_debug_state_proc GetDebugState_in
#define BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS       work_queue_entry* Entry, thread_local_state* Thread
#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS  thread_local_state* Thread, game_state* GameState


#define BONSAI_API_MAIN_THREAD_CALLBACK() \
  exported_function void BONSAI_API_MAIN_THREAD_CALLBACK_NAME(BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS)

#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK() \
  exported_function game_state* BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS)

#define BONSAI_API_WORKER_THREAD_CALLBACK() \
  exported_function void BONSAI_API_WORKER_THREAD_CALLBACK_NAME(BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS)

#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK() \
  exported_function void BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS)

typedef void (*GameCallback)(void*);

typedef void (*bonsai_main_thread_callback)        (BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS);
typedef void (*bonsai_worker_thread_init_callback) (BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS);

typedef void        (*bonsai_worker_thread_callback)    (BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS);
typedef game_state* (*bonsai_main_thread_init_callback) (BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS);

struct thread_startup_params
{
  bonsai_worker_thread_init_callback InitProc;
  game_state* GameState;
  work_queue* LowPriority;
  work_queue* HighPriority;
  thread Self;
};


