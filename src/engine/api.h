#define BONSAI_API_MAIN_THREAD_CALLBACK_NAME          MainThreadCallback
#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME     InitMainThreadCallback
#define BONSAI_API_WORKER_THREAD_CALLBACK_NAME        WorkerThreadCallback
#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME   InitWorkerThreadCallback
#define BONSAI_API_ON_GAME_LIB_LOAD_CALLBACK_NAME     OnGameLibLoad

#define BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS         engine_resources *Resources, thread_local_state *MainThread
#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS    engine_resources *Resources, thread_local_state *MainThread

#define BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS       volatile work_queue_entry* Entry, thread_local_state* Thread
#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS  thread_local_state* AllThreads, s32 ThreadIndex


#define BONSAI_API_MAIN_THREAD_CALLBACK() \
  link_export void BONSAI_API_MAIN_THREAD_CALLBACK_NAME(BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS)

#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK() \
  link_export game_state* BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS)

#define BONSAI_API_WORKER_THREAD_CALLBACK() \
  link_export void BONSAI_API_WORKER_THREAD_CALLBACK_NAME(BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS)

#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK() \
  link_export void BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS)


struct game_state;
struct engine_resources;
struct thread_local_state;

typedef void (*bonsai_main_thread_callback)        (BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS);
typedef void (*bonsai_worker_thread_init_callback) (BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS);

typedef void        (*bonsai_worker_thread_callback)    (BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS);
typedef game_state* (*bonsai_main_thread_init_callback) (BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS);

typedef b32 (*bonsai_engine_callback)(engine_resources*);

struct game_api
{
  bonsai_main_thread_init_callback GameInit;
  bonsai_main_thread_callback GameMain;

  bonsai_worker_thread_init_callback WorkerInit;
  bonsai_worker_thread_callback WorkerMain;

};

struct engine_api
{
  bonsai_engine_callback OnLibraryLoad;     // Anytime the game library is loaded
  bonsai_engine_callback Init;

  bonsai_engine_callback FrameBegin;
  bonsai_engine_callback FrameEnd;
  bonsai_engine_callback Render;
};

#if PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS
link_internal b32
InitializeEngineApi(engine_api *EngineApi, shared_lib GameLib)
{
  EngineApi->OnLibraryLoad = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_OnLibraryLoad) );
  EngineApi->Init          = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_Init) );
  EngineApi->FrameBegin    = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_FrameBegin) );
  EngineApi->FrameEnd      = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_FrameEnd) );
  EngineApi->Render        = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_Render) );

  b32 Result = EngineApi->OnLibraryLoad && EngineApi->Init && EngineApi->FrameBegin && EngineApi->FrameEnd && EngineApi->Render;
  return Result;
}

link_internal b32
InitializeGameApi(game_api *GameApi, shared_lib GameLib)
{
  GameApi->GameInit   = (bonsai_main_thread_init_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME));
  GameApi->GameMain   = (bonsai_main_thread_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_CALLBACK_NAME));
  GameApi->WorkerInit = (bonsai_worker_thread_init_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME));
  GameApi->WorkerMain = (bonsai_worker_thread_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_CALLBACK_NAME) );

  b32 Result = GameApi->GameMain != 0;
  return Result;
}
#endif
