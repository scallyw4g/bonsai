struct game_state;
struct engine_resources;
struct thread_local_state;

typedef void (*bonsai_main_thread_callback)        (BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS);
typedef void (*bonsai_worker_thread_init_callback) (BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS);

typedef bool        (*bonsai_worker_thread_callback)    (BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS);
typedef game_state* (*bonsai_main_thread_init_callback) (BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS);

typedef b32 (*bonsai_engine_callback)(engine_resources*);
typedef b32 (*bonsai_engine_init_callback)(engine_resources*, thread_startup_params*);

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
  bonsai_engine_init_callback WorkerInit;

  bonsai_engine_callback FrameBegin;
  bonsai_engine_callback SimulateAndBufferGeometry;
  bonsai_engine_callback FrameEnd;
  bonsai_engine_callback Render;
};

link_internal b32
InitializeEngineApi(engine_api *EngineApi, shared_lib GameLib)
{
  EngineApi->OnLibraryLoad             = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_OnLibraryLoad) );
  EngineApi->Init                      = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_Init) );
  EngineApi->WorkerInit                = (bonsai_engine_init_callback)GetProcFromLib(GameLib, STRINGIZE(WorkerInit) );

  EngineApi->SimulateAndBufferGeometry = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_SimulateAndBufferGeometry) );
  EngineApi->FrameBegin                = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_FrameBegin) );
  EngineApi->FrameEnd                  = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_FrameEnd) );
  EngineApi->Render                    = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_Render) );

  b32 Result = EngineApi->SimulateAndBufferGeometry && EngineApi->WorkerInit && EngineApi->OnLibraryLoad && EngineApi->Init && EngineApi->FrameBegin && EngineApi->FrameEnd && EngineApi->Render;
  return Result;
}

link_internal b32
InitializeGameApi(game_api *GameApi, shared_lib GameLib)
{
  GameApi->GameInit   = (bonsai_main_thread_init_callback)  GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME));
  GameApi->GameMain   = (bonsai_main_thread_callback)       GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_CALLBACK_NAME));
  GameApi->WorkerInit = (bonsai_worker_thread_init_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME));
  GameApi->WorkerMain = (bonsai_worker_thread_callback)     GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_CALLBACK_NAME) );

  b32 Result = GameApi->GameMain != 0;
  return Result;
}
