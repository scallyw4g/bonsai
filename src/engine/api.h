struct engine_api
{
  bonsai_engine_callback OnLibraryLoad; // Anytime the game library is loaded
  bonsai_engine_callback Init;
  /* bonsai_engine_init_callback WorkerInit; */

  bonsai_engine_callback FrameBegin;
  bonsai_engine_callback Simulate;
  bonsai_engine_callback FrameEnd;
  bonsai_engine_callback Render;
};

link_internal b32
InitializeEngineApi(engine_api *EngineApi, shared_lib GameLib)
{
  EngineApi->OnLibraryLoad = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_OnLibraryLoad) );
  EngineApi->Init          = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_Init) );

  EngineApi->Simulate      = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_Simulate) );
  EngineApi->FrameBegin    = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_FrameBegin) );
  EngineApi->FrameEnd      = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_FrameEnd) );
  EngineApi->Render        = (bonsai_engine_callback)GetProcFromLib(GameLib, STRINGIZE(Bonsai_Render) );

  b32 Result = EngineApi->Simulate && EngineApi->OnLibraryLoad && EngineApi->Init && EngineApi->FrameBegin && EngineApi->FrameEnd && EngineApi->Render;
  return Result;
}

link_internal b32
InitializeGameApi(application_api *GameApi, shared_lib GameLib)
{
  GameApi->GameInit      = (bonsai_main_thread_init_callback)  GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME));
  GameApi->GameMain      = (bonsai_main_thread_callback)       GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_MAIN_THREAD_CALLBACK_NAME));
  GameApi->WorkerInit    = (bonsai_worker_thread_init_callback)GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME));
  GameApi->WorkerMain    = (bonsai_worker_thread_callback)     GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_WORKER_THREAD_CALLBACK_NAME) );
  GameApi->OnLibraryLoad = (bonsai_main_thread_callback)       GetProcFromLib(GameLib, STRINGIZE(BONSAI_API_ON_GAME_LIB_LOAD_CALLBACK_NAME) );

  b32 Result = GameApi->GameMain != 0;
  return Result;
}
