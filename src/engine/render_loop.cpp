
link_export void
DrainQueue(engine_resources *Engine, work_queue *Queue, bonsai_futex *ExitFutex = 0)
{
  TIMED_FUNCTION();

  Assert(RENDER_THREAD_ThreadLocal_ThreadIndex == ThreadLocal_ThreadIndex);
  UNPACK_ENGINE_RESOURCES(Engine);

  if (ExitFutex) { if (FutexIsSignaled(ExitFutex)) return; }

  thread_local_state *Thread = GetThreadLocalState(ThreadLocal_ThreadIndex);
  while (work_queue_job *Job = PopNextJob(Plat, Queue))
  {
    HandleJob(Job, Thread, &Engine->Stdlib.AppApi);
    RewindArena(GetTranArena());
    if (ExitFutex) { if (FutexIsSignaled(ExitFutex)) return; }
  }
}

link_export THREAD_MAIN_RETURN
RenderThread_Main(void *ThreadStartupParams)
{
  thread_local_state *Thread = Cast(thread_local_state*, ThreadStartupParams);
  WorkerThread_BeforeJobStart(Thread);

  Assert(GetStdlib()->ThreadStates);

  /* Assert(Thread->ThreadIndex > 0); */
  /* SetThreadLocal_ThreadIndex(Thread->ThreadIndex); */

  engine_resources *Engine    = GetEngineResources();
     bonsai_stdlib *Stdlib    = &Engine->Stdlib;
   application_api *AppApi    = &Thread->Stdlib->AppApi;
                os *Os        = &Engine->Stdlib.Os;
          platform *Plat      = &Engine->Stdlib.Plat;
        engine_api *EngineApi = &Engine->EngineApi;
          graphics *Graphics  = &Engine->Graphics;
              auto Ui         = &Stdlib->Ui;

  PlatformMakeRenderContextCurrent(Os, Os->GlContext);

  if (Graphics->Initialized == False)
  {
    Graphics->Initialized = GraphicsInit(Graphics, &Engine->Settings, AllocateArena());

    memory_arena *UiMemory = AllocateArena();
    if (Graphics->Initialized)
    {
      bitmap_block_array Bitmaps = BitmapBlockArray(GetTranArena());
      LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/64x64_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
      LoadBitmapsFromFolderOrdered(CSz("assets/mystic_rpg_icon_pack/Sprites/300%/44x44_sprites"), &Bitmaps, GetTranArena(), GetTranArena());
      Stdlib->Ui.SpriteTextureArray = CreateTextureArrayFromBitmapBlockArray(&Bitmaps, V2i(64,64), CSz("Sprite Textures"));
    }

    // Map immediate GPU buffers for first frame
    MapGpuBuffer(&Graphics->ImmediateGeometry);
    MapGpuBuffer(&Graphics->Transparency.GpuBuffer);

    MapGpuBuffer(&Ui->SolidQuadGeometryBuffer);
    MapGpuBuffer(&Ui->TextGroup->Buf);
  }

  FullBarrier;
  SignalFutex(&Plat->WorkerThreadsReady);

  if (Graphics->Initialized)
  {
    bonsai_futex *WorkerThreadsExitFutex = &Plat->WorkerThreadsExitFutex;

    // Signal to main thread we're ready to start
    WaitOnFutex(&Plat->WorkerThreadsReady, True);

    opengl *GL = GetGL();
    while ( FutexNotSignaled(WorkerThreadsExitFutex) )
    {
      WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();

      AppApi->WorkerBeforeJob(Thread);

      EngineApi->DrainQueue(Engine, &Plat->HiRenderQ, 0);
      EngineApi->DrainQueue(Engine, &Plat->LoRenderQ, &Graphics->RenderGate);

      if (Graphics->FrameFence)
      {
        debug_timed_function Timer("WaitForFrameFence");

        u32 SyncStatus = 0;
        {
          TIMED_NAMED_BLOCK(ClientWaitSync);
          SyncStatus = GL->ClientWaitSync(Graphics->FrameFence, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
        }

        switch(SyncStatus)
        {
          case GL_ALREADY_SIGNALED:
          case GL_CONDITION_SATISFIED:
          {
            TIMED_NAMED_BLOCK(FencePassed);
            /* if (PlatformGetNextVBlank().NextVBlankInNanoseconds < MillisecondsToNanoseconds(1)) */
            {
              BonsaiSwapBuffers(&Engine->Stdlib.Os);

              /* GL->Finish(); */

              HotReloadShaders(GetStdlib());

              {
                TIMED_NAMED_BLOCK(MapBuffers);
                // Map GPU buffers for next frame
                MapGpuBuffer(&Ui->SolidQuadGeometryBuffer);
                MapGpuBuffer(&Ui->TextGroup->Buf);

                triple_buffered_gpu_mapped_element_buffer *GpuMap        = &Graphics->ImmediateGeometry;
                MapGpuBuffer(GpuMap);
                /* MapGpuBuffer(&Graphics->Transparency.GpuBuffer); */
                Assert(GpuMap->Buffer.At == 0);
              }


              {
                TIMED_NAMED_BLOCK(DeleteFrameFence);
                UnsignalFutex(&Graphics->RenderGate, MAIN_THREAD_ThreadLocal_ThreadIndex);
                GL->DeleteSync(Graphics->FrameFence);
                Graphics->FrameFence = 0;
                AssertNoGlErrors;

                /* Graphics->TerrainJobsDispatchedThisFrame = 0; */
                /* Assert(Graphics->TerrainJobsDispatchedThisFrame <= Graphics->MaxTerrainJobsDispatchedThisFrame); */

                /* ContinueTerrainJobs(Graphics); */
              }
            }
          } break;

          case GL_WAIT_FAILED:
          {
            SoftError("Error waiting on gl sync object");
          } break;

          case GL_TIMEOUT_EXPIRED:
          {
          } break;

          InvalidDefaultCase;
        }
      }

      if (FutexIsSignaled(&Plat->WorkerThreadsSuspendFutex))
      {
        // Have to wait for all the worker threads to suspend cause they push
        // entries onto the render queues
        while (Plat->WorkerThreadsSuspendFutex.ThreadsWaiting != GetWorkerThreadCount()-1)
        {
          SleepMs(1);
        }

        // Even though we just called these we have to make sure they're actually
        // flushed; the main thread could have pushed stuff onto the Hi
        // queue while we were draining the Lo queue, for example.  Once we're
        // in this if we know the main thread is waiting for us
        EngineApi->DrainQueue(Engine, &Plat->HiRenderQ, 0);
        EngineApi->DrainQueue(Engine, &Plat->LoRenderQ, &Graphics->RenderGate);

        Assert(QueueIsEmpty(&Engine->Stdlib.Plat.HiRenderQ));
        Assert(QueueIsEmpty(&Engine->Stdlib.Plat.LoRenderQ));

        SleepMs(3);

        Assert(QueueIsEmpty(&Engine->Stdlib.Plat.HiRenderQ));
        Assert(QueueIsEmpty(&Engine->Stdlib.Plat.LoRenderQ));

        WaitOnFutex(&Plat->WorkerThreadsSuspendFutex);
      }

      /* SpinlockNs(100); */
      SleepMs(1);
    }

    Info("Exiting Render Thread (%d)", Thread->ThreadIndex);
    Assert(FutexIsSignaled(WorkerThreadsExitFutex));
    WaitOnFutex(WorkerThreadsExitFutex);

  }
  else
  {
    Error("Render thread initiailization failed.");
  }

  THREAD_MAIN_RETURN Result = ReinterpretCast(THREAD_MAIN_RETURN, Graphics->Initialized);
  return Result;
}

// Owns the secondary GL context, and services the TransferQ, which only ever
// holds RequestTerrainReadback tasks.  This lets the PBO readback overlap with
// rendering on the render thread.
//
link_export THREAD_MAIN_RETURN
TransferThread_Main(void *ThreadStartupParams)
{
  thread_local_state *Thread = Cast(thread_local_state*, ThreadStartupParams);
  WorkerThread_BeforeJobStart(Thread);

  Assert(GetStdlib()->ThreadStates);

  engine_resources *Engine = GetEngineResources();
   application_api *AppApi = &Thread->Stdlib->AppApi;
                os *Os     = &Engine->Stdlib.Os;
          platform *Plat   = &Engine->Stdlib.Plat;

  PlatformMakeRenderContextCurrent(Os, Os->GlContext2);

  // Signal to main thread we're ready to start
  WaitOnFutex(&Plat->WorkerThreadsReady, True);

  bonsai_futex *WorkerThreadsExitFutex    = &Plat->WorkerThreadsExitFutex;
  bonsai_futex *WorkerThreadsSuspendFutex = &Plat->WorkerThreadsSuspendFutex;

  while ( FutexNotSignaled(WorkerThreadsExitFutex) )
  {
    WORKER_THREAD_ADVANCE_DEBUG_SYSTEM();

    // Suspend requests wait on every worker thread, so we have to participate
    if (FutexIsSignaled(WorkerThreadsSuspendFutex))
    {
      WaitOnFutex(WorkerThreadsSuspendFutex);
      continue;
    }

    if (AppApi->WorkerBeforeJob) { AppApi->WorkerBeforeJob(Thread); }

    while (work_queue_job *Job = PopNextJob(Plat, &Plat->TransferQ))
    {
      HandleJob(Job, Thread, AppApi);
      RewindArena(GetTranArena());

      if ( FutexIsSignaled(WorkerThreadsExitFutex)    ) break;
      if ( FutexIsSignaled(WorkerThreadsSuspendFutex) ) break;
    }

    SleepMs(1);
  }

  PlatformReleaseRenderContext(Os);

  Info("Exiting Transfer Thread (%d)", Thread->ThreadIndex);
  Assert(FutexIsSignaled(WorkerThreadsExitFutex));
  WaitOnFutex(WorkerThreadsExitFutex);

  return 0;
}
