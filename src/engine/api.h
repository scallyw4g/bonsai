#define BONSAI_API_MAIN_THREAD_CALLBACK_NAME          MainThreadCallback
#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME     InitMainThreadCallback
#define BONSAI_API_WORKER_THREAD_CALLBACK_NAME        WorkerThreadCallback
#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME   InitWorkerThreadCallback
#define BONSAI_API_ON_GAME_LIB_LOAD_CALLBACK_NAME     OnGameLibLoad

#define BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS         platform *Plat, game_state *GameState, hotkeys *Hotkeys, thread_local_state *Thread
#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS    platform *Plat, memory_arena *GameMemory
#define BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS       volatile work_queue_entry* Entry, thread_local_state* Thread
#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS  thread_local_state* Thread, game_state* GameState


#define BONSAI_API_MAIN_THREAD_CALLBACK() \
  link_export void BONSAI_API_MAIN_THREAD_CALLBACK_NAME(BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS)

#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK() \
  link_export game_state* BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS)

#define BONSAI_API_WORKER_THREAD_CALLBACK() \
  link_export void BONSAI_API_WORKER_THREAD_CALLBACK_NAME(BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS)

#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK() \
  link_export void BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS)


struct game_state;
struct thread_local_state;

typedef void (*bonsai_main_thread_callback)        (BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS);
typedef void (*bonsai_worker_thread_init_callback) (BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS);

typedef void        (*bonsai_worker_thread_callback)    (BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS);
typedef game_state* (*bonsai_main_thread_init_callback) (BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS);

typedef void (*bonsai_render_callback)(platform *Plat);

typedef b32 (*bonsai_on_load_callback)();


struct game_api
{
  // Exported by user

  bonsai_main_thread_init_callback GameInit;
  bonsai_main_thread_callback GameMain;

  bonsai_worker_thread_init_callback WorkerInit; // Optional
  bonsai_worker_thread_callback WorkerMain;

  // Exported by engine

  bonsai_render_callback Render;
  bonsai_on_load_callback OnLoad;
};

link_export b32
BONSAI_API_ON_GAME_LIB_LOAD_CALLBACK_NAME ()
{
#if PLATFORM_GL_IMPLEMENTATIONS
  b32 Result = InitializeOpenglFunctions();
#else
  b32 Result = True;
#endif
  return Result;
}

#if PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS
link_internal b32
InitializeGameApi(game_api *GameApi, shared_lib GameLib)
{
  GameApi->GameInit = (bonsai_main_thread_init_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME));
  GameApi->GameMain = (bonsai_main_thread_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_CALLBACK_NAME));
  GameApi->WorkerInit = (bonsai_worker_thread_init_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME));
  GameApi->WorkerMain = (bonsai_worker_thread_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_CALLBACK_NAME) );
  GameApi->Render = (bonsai_render_callback)GetProcFromLib(GameLib, STRINGIZE(Renderer_FrameEnd) );

  GameApi->OnLoad = (bonsai_on_load_callback)GetProcFromLib(GameLib, "OnGameLibLoad" );

  b32 Result = (GameApi->GameInit && GameApi->GameMain && GameApi->WorkerMain && GameApi->Render && GameApi->OnLoad);
  if (Result) { Result = GameApi->OnLoad(); }

  return Result;
}
#endif
