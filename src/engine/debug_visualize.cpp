link_internal void
DebugVisualize(renderer_2d *Ui, mesh_freelist *Freelist)
{
#if BONSAI_INTERNAL
  AcquireFutex(&Freelist->DebugFutex);
#endif

  /* v2 Basis = DefaultWindowBasis(Ui->ScreenDim); */
  /* Info("(%f, %f)", (double)Ui->ScreenDim.x, (double)Ui->ScreenDim.y); */

  v2 Basis = {};
  window_layout *Window = GetOrCreateWindow(Ui, "DebugWindow", Basis);

  PushWindowStart(Ui, Window);

  {
    u32 Count = 0;
    PushTableStart(Ui);
    volatile freelist_entry *At = Freelist->FirstFreeMesh;
    while (At)
    {
      At = At->Next;
      PushColumn(Ui, CSz("O"));
      if (++Count > 32)
      {
        PushNewRow(Ui);
        Count = 0;
      }
    }
    PushTableEnd(Ui);
  }


#if 0
  {
    u32 Count = 0;
    PushTableStart(Ui);
    volatile Free *At = Freelist->Containers;
    while (At)
    {
      At = At->Next;
      PushColumn(Ui, CSz("F"));
      if (++Count > 32)
      {
        PushNewRow(Ui);
        Count = 0;
    }
    }
    PushTableEnd(Ui);
  }
#endif

  PushWindowEnd(Ui, Window);

#if BONSAI_INTERNAL
  ReleaseFutex(&Freelist->DebugFutex);
#endif
}

link_internal void
DebugVisualize(renderer_2d *Ui, world_chunk **FreeChunks, s32 FreeChunkCount)
{
  /* v2 Basis = DefaultWindowBasis(Ui->ScreenDim); */
  /* Info("(%f, %f)", (double)Ui->ScreenDim.x, (double)Ui->ScreenDim.y); */

  v2 Basis = {};
  window_layout *Window = GetOrCreateWindow(Ui, "DebugWindow", Basis);

  PushWindowStart(Ui, Window);
  PushTableStart(Ui);
  RangeIterator(Index, FreeChunkCount)
  {
    PushColumn(Ui, CSz("O"));
    if (Index % 32 == 31) { PushNewRow(Ui); }
  }
  PushTableEnd(Ui);
  PushWindowEnd(Ui, Window);
}

