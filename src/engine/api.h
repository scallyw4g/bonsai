
struct engine_api
{
  bonsai_engine_callback OnLibraryLoad; // Anytime the game library is loaded
  bonsai_engine_callback Init;
  /* bonsai_engine_init_callback WorkerInit; */

  bonsai_engine_callback FrameBegin;
  bonsai_engine_callback Simulate;
  bonsai_engine_callback FrameEnd;

  // NOTE(Jesse): This gets loaded such that the render thread shares the same
  // globals as the game lib.
  thread_main_callback_type RenderThread_Main;
  thread_main_callback_type TransferThread_Main;

  bonsai_drain_queue_callback DrainQueue;

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

  EngineApi->Render              = (bonsai_engine_callback)     GetProcFromLib(GameLib, STRINGIZE(Bonsai_Render) );
  EngineApi->RenderThread_Main   = (thread_main_callback_type)  GetProcFromLib(GameLib, STRINGIZE(RenderThread_Main) );
  EngineApi->TransferThread_Main = (thread_main_callback_type)  GetProcFromLib(GameLib, STRINGIZE(TransferThread_Main) );
  EngineApi->DrainQueue          = (bonsai_drain_queue_callback)GetProcFromLib(GameLib, STRINGIZE(DrainQueue) );

  b32 Result = EngineApi->Simulate           &&
               EngineApi->OnLibraryLoad      &&
               EngineApi->Init               &&
               EngineApi->FrameBegin         &&
               EngineApi->FrameEnd           &&
               EngineApi->Render             &&
               EngineApi->RenderThread_Main  &&
               EngineApi->TransferThread_Main  &&
               EngineApi->DrainQueue;

  return Result;
}

