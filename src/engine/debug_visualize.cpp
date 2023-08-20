link_internal void
DebugVisualize(renderer_2d *Renderer, mesh_freelist *Freelist)
{
#if BONSAI_INTERNAL
  AcquireFutex(&Freelist->DebugFutex);
#endif

  /* v2 Basis = DefaultWindowBasis(Renderer->ScreenDim); */
  /* Info("(%f, %f)", (double)Renderer->ScreenDim.x, (double)Renderer->ScreenDim.y); */

  v2 Basis = {};
  local_persist window_layout VizWindowInstance = WindowLayout("DebugWindow", Basis);
  window_layout *Window = &VizWindowInstance;

  PushWindowStart(Renderer, Window);

  {
    u32 Count = 0;
    PushTableStart(Renderer);
    volatile free_mesh *At = Freelist->FirstFree;
    while (At)
    {
      At = At->Next;
      PushColumn(Renderer, CSz("O"));
      if (++Count > 32)
      {
        PushNewRow(Renderer);
        Count = 0;
      }
    }
    PushTableEnd(Renderer);
  }


  {
    u32 Count = 0;
    PushTableStart(Renderer);
    volatile free_mesh *At = Freelist->Containers;
    while (At)
    {
      At = At->Next;
      PushColumn(Renderer, CSz("F"));
      if (++Count > 32)
      {
        PushNewRow(Renderer);
        Count = 0;
    }
    }
    PushTableEnd(Renderer);
  }

  PushWindowEnd(Renderer, Window);

#if BONSAI_INTERNAL
  ReleaseFutex(&Freelist->DebugFutex);
#endif
}

link_internal void
DebugVisualize(renderer_2d *Renderer, world_chunk **FreeChunks, s32 FreeChunkCount)
{
  /* v2 Basis = DefaultWindowBasis(Renderer->ScreenDim); */
  /* Info("(%f, %f)", (double)Renderer->ScreenDim.x, (double)Renderer->ScreenDim.y); */

  v2 Basis = {};
  local_persist window_layout VizWindowInstance = WindowLayout("DebugWindow", Basis);
  window_layout *Window = &VizWindowInstance;

  PushWindowStart(Renderer, Window);
  PushTableStart(Renderer);
  RangeIterator(Index, FreeChunkCount)
  {
    PushColumn(Renderer, CSz("O"));
    if (Index % 32 == 31) { PushNewRow(Renderer); }
  }
  PushTableEnd(Renderer);
  PushWindowEnd(Renderer, Window);
}

link_internal void
DoEngineDebugMenu()
{
}
