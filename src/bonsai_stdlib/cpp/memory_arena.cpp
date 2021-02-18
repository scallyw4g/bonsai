inline void
RewindArena(memory_arena *Arena, umm RestartBlockSize = Megabytes(1) )
{
  if (Arena->Prev)
  {
    UnprotectArena(Arena->Prev);
    VaporizeArena(Arena->Prev);
    Arena->Prev = 0;
  }

  UnprotectArena(Arena);

  u8* ClearByte = Arena->Start;
  while( ClearByte < Arena->At )
  {
    *ClearByte++ = 0;
  }

  Arena->At = Arena->Start;
  Arena->NextBlockSize = RestartBlockSize;

#if BONSAI_INTERNAL
  Arena->Pushes = 0;
  DEBUG_CLEAR_META_RECORDS_FOR(Arena);
#endif

  return;
}
