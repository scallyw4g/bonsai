inline b32
RewindArena(memory_arena *Arena, umm RestartBlockSize = Megabytes(1) )
{
  b32 Result = True;

  // Check for start because when we allocate an arena on the stack it's
  // cleared to zero and treated as a sentinal.
  if (Arena->Prev && Arena->Prev->Start)
  {
    Result &= UnprotectArena(Arena->Prev);
    Result &= VaporizeArena(Arena->Prev);
    Arena->Prev = 0;
  }

  Result &= UnprotectArena(Arena);

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

  return Result;
}
