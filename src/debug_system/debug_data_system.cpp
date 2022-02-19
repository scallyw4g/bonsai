#if BONSAI_INTERNAL

debug_global thread_local u32 ThreadLocal_ThreadIndex = 0;
debug_global b32 DebugGlobal_RedrawEveryPush = 0;



/****************************                 ********************************/
/****************************  Picked Chunks  ********************************/
/****************************                 ********************************/



void
PickChunk(world_chunk* Chunk, aabb ChunkAABB)
{
  TIMED_FUNCTION();
  debug_state* DebugState = GetDebugState();

  if (DebugState->DoChunkPicking &&
      DebugState->PickedChunkCount < MAX_PICKED_WORLD_CHUNKS &&
      Intersect(ChunkAABB, DebugState->PickRay) )
  {
    DebugState->PickedChunks[DebugState->PickedChunkCount] = Chunk;
    DebugState->PickedChunkCount++;
  }

  return;
}



/*****************************                   *****************************/
/*****************************  Thread Metadata  *****************************/
/*****************************                   *****************************/



void
RegisterThread(u32 ThreadIndex)
{
  ThreadLocal_ThreadIndex = ThreadIndex;
  return;
}

inline debug_thread_state*
GetThreadLocalStateFor(u32 ThreadIndex)
{
  debug_state *State = GetDebugState();

  // NOTE(Jesse): It's possible that during initialization between the time
  // that "DebugState->Initialized" is set to true and the time the
  // debug_thread_state structs are allocated that we'll call this function,
  // which is why we have to check that ThreadStates is valid.
  //
  // TODO(Jesse): Audit codebase to make sure callers of this function actually
  // check that it returns a valid pointer.
  debug_thread_state *Result = State->ThreadStates ?
                                 State->ThreadStates + ThreadIndex : 0;

  return Result;
}

inline debug_thread_state*
GetThreadLocalState()
{
  debug_thread_state* Result = GetThreadLocalStateFor(ThreadLocal_ThreadIndex);
  return Result;
}



/****************************                       **************************/
/****************************  Arena Introspection  **************************/
/****************************                       **************************/



void
RegisterArena(const char *Name, memory_arena *Arena)
{
  debug_state* DebugState = GetDebugState();
  b32 Registered = False;
  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = &DebugState->RegisteredMemoryArenas[Index];

    const char *CurrentName = Current->Name;
    if (!CurrentName)
    {
      if (AtomicCompareExchange( (volatile void **)&Current->Name, (void*)Name, (void*)CurrentName ))
      {
        Current->Arena = Arena;
        Registered = True;
        break;
      }
      else
      {
        continue;
      }
    }
  }

  if (!Registered)
  {
    Error("Too many arenas registered");
    Error("Registering Arena : %s", Name);
  }

  return;
}

b32
PushesShareHeadArena(push_metadata *First, push_metadata *Second)
{
  b32 Result = (First->HeadArenaHash == Second->HeadArenaHash &&
                First->StructSize    == Second->StructSize    &&
                First->StructCount   == Second->StructCount   &&
                First->Name          == Second->Name);
  return Result;
}

b32
PushesMatchExactly(push_metadata *First, push_metadata *Second)
{
  b32 Result = (First->ArenaHash     == Second->ArenaHash     &&
                First->HeadArenaHash == Second->HeadArenaHash &&
                First->StructSize    == Second->StructSize    &&
                First->StructCount   == Second->StructCount   &&
                First->Name          == Second->Name);
  return Result;
}

inline void
ClearMetaRecordsFor(memory_arena *Arena)
{
  u32 TotalThreadCount = GetWorkerThreadCount() + 1;
  for ( u32 ThreadIndex = 0;
      ThreadIndex < TotalThreadCount;
      ++ThreadIndex)
  {
    for ( u32 MetaIndex = 0;
        MetaIndex < META_TABLE_SIZE;
        ++MetaIndex)
    {
      push_metadata *Meta = GetThreadLocalStateFor(ThreadIndex)->MetaTable + MetaIndex;
      if (Meta->ArenaHash == HashArena(Arena))
      {
        Clear(Meta);
      }
    }
  }

  return;
}



/*****************************                  ******************************/
/*****************************  Arena Metadata  ******************************/
/*****************************                  ******************************/



registered_memory_arena *
GetRegisteredMemoryArena( memory_arena *Arena)
{
  registered_memory_arena *Result = 0;

  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = &GetDebugState()->RegisteredMemoryArenas[Index];
    if (Current->Arena == Arena)
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

void
WriteToMetaTable(push_metadata *Query, push_metadata *Table, meta_comparator Comparator)
{

  u32 HashValue = (u32)(((u64)Query->Name & (u64)Query->ArenaHash) % META_TABLE_SIZE);
  u32 FirstHashValue = HashValue;

  push_metadata *PickMeta = Table + HashValue;
  while (PickMeta->Name)
  {
    if (Comparator(PickMeta, Query))
    {
      break;
    }

    HashValue = (HashValue+1)%META_TABLE_SIZE;
    PickMeta = Table + HashValue;
    if (HashValue == FirstHashValue)
    {
      Error("Meta Table is full");
      return;
    }
  }

  if (PickMeta->Name)
  {
    PickMeta->PushCount += Query->PushCount;
  }
  else
  {
    *PickMeta = *Query;
  }

  return;
}

void
CollateMetadata(push_metadata *InputMeta, push_metadata *MetaTable)
{
  WriteToMetaTable(InputMeta, MetaTable, PushesShareHeadArena);
  return;
}

void
WritePushMetadata(push_metadata *InputMeta, push_metadata *MetaTable)
{
  WriteToMetaTable(InputMeta, MetaTable, PushesMatchExactly);
  return;
}



/****************************                    *****************************/
/****************************  Memory Allocator  *****************************/
/****************************                    *****************************/



void*
DEBUG_Allocate(memory_arena* Arena, umm StructSize, umm StructCount, const char* Name, s32 Line, const char* File, umm Alignment, b32 MemProtect)
{
  umm PushSize = StructCount * StructSize;
  void* Result = PushStruct( Arena, PushSize, Alignment, MemProtect);

  push_metadata ArenaMetadata = {Name, HashArena(Arena), HashArenaHead(Arena), StructSize, StructCount, 1};
  WritePushMetadata(&ArenaMetadata, GetThreadLocalStateFor(ThreadLocal_ThreadIndex)->MetaTable);

  if (!Result)
  {
    Error("Pushing %s on Line: %d, in file %s", Name, Line, File);
    Assert(False);
    return False;
  }

  return Result;
}

memory_arena_stats
GetTotalMemoryArenaStats()
{
  TIMED_FUNCTION();

  memory_arena_stats TotalStats = {};
  debug_state *DebugState = GetDebugState();

  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = DebugState->RegisteredMemoryArenas + Index;
    if (!Current->Arena) continue;

    memory_arena_stats CurrentStats = GetMemoryArenaStats(Current->Arena);
    TotalStats.Allocations          += CurrentStats.Allocations;
    TotalStats.Pushes               += CurrentStats.Pushes;
    TotalStats.TotalAllocated       += CurrentStats.TotalAllocated;
    TotalStats.Remaining            += CurrentStats.Remaining;
  }

  return TotalStats;
}



/**************************                     ******************************/
/**************************  Utility Functions  ******************************/
/**************************                     ******************************/



void
InitScopeTree(debug_scope_tree *Tree)
{
  /* TIMED_FUNCTION(); */ // Cannot be timed because it has to run to initialize the scope tree system

  Clear(Tree);
  Tree->WriteScope   = &Tree->Root;

  return;
}

void
FreeScopes(debug_thread_state *ThreadState, debug_profile_scope *ScopeToFree)
{
  // Behaves poorly when timed because it adds a profile scope for every
  // profile scope present, which has the net effect of adding infinite scopes
  // over the course of the program.
  //
  // Can be timed by wrapping the call site with a TIMED_BLOCK
  //
  /* TIMED_FUNCTION(); */


  debug_profile_scope* Current = ScopeToFree;
  while (Current)
  {
    debug_profile_scope* Next = Current->Sibling;

    FreeScopes(ThreadState, Current->Child);

    Clear(Current);

    Current->Sibling = ThreadState->FirstFreeScope;
    ThreadState->FirstFreeScope = Current;

    Current = Next;
  }

  return;
}

// TODO(Jesse, id: 103, tags: cleanup): Replace this expression everywhere!!!  This might be done.
inline u32
GetNextDebugFrameIndex(u32 Current)
{
  u32 Result = (Current + 1) % DEBUG_FRAMES_TRACKED;
  return Result;
}

inline void
AdvanceThreadState(debug_thread_state *ThreadState, u32 NextFrameId)
{
  /* TIMED_FUNCTION(); */
  // Can this actually be timed if we're freeing scopes?  I think it can,
  // because we're freeing the _next_ frames scopes, however it's behaving
  // badly when turned on

  u32 NextWriteIndex = NextFrameId % DEBUG_FRAMES_TRACKED;

  debug_scope_tree *NextWriteTree = ThreadState->ScopeTrees + NextWriteIndex;

  FreeScopes(ThreadState, NextWriteTree->Root);
  InitScopeTree(NextWriteTree);

  ThreadState->MutexOps[NextWriteIndex].NextRecord = 0;
  ThreadState->WriteIndex = NextFrameId;

  NextWriteTree->FrameRecorded = NextFrameId;

  return;
}

inline void
WorkerThreadAdvanceDebugSystem()
{
  Assert(ThreadLocal_ThreadIndex != 0);

  debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadLocal_ThreadIndex);
  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  Assert(ThreadState != MainThreadState);

  u32 NextFrameId = MainThreadState->WriteIndex;

  // Note(Jesse): WriteIndex must not straddle a cache line!
  Assert(sizeof(MainThreadState->WriteIndex) == 4);
  Assert((umm)(&MainThreadState->WriteIndex)%64 <= 60 );

  if (NextFrameId != ThreadState->WriteIndex)
  {
    AdvanceThreadState(ThreadState, NextFrameId);
  }

  return;
}

global_variable r64 LastMs;

r32
MainThreadAdvanceDebugSystem()
{
  TIMED_FUNCTION();
  Assert(ThreadLocal_ThreadIndex == 0);

  r64 CurrentMS = GetHighPrecisionClock();
  r64 Dt = (CurrentMS - LastMs)/1000.0;
  LastMs = CurrentMS;

  Assert(ThreadLocal_ThreadIndex == 0);
  debug_thread_state *MainThreadState = GetThreadLocalStateFor(ThreadLocal_ThreadIndex);
  debug_state *SharedState = GetDebugState();

  /* DEBUG_VALUE(SharedState->ReadScopeIndex); */
  /* DEBUG_VALUE(MainThreadState->WriteIndex); */
  /* for (u32 ThreadIndex = 1; */
  /*     ThreadIndex < GetTotalThreadCount(); */
  /*     ++ThreadIndex) */
  /* { */
  /*   debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex); */
  /*   DEBUG_VALUE(ThreadState->WriteIndex); */
  /* } */

  if (SharedState->DebugDoScopeProfiling)
  {
    u64 CurrentCycles = GetCycleCount();

    u32 ThisFrameWriteIndex = MainThreadState->WriteIndex % DEBUG_FRAMES_TRACKED;
    u32 NextFrameWriteIndex = GetNextDebugFrameIndex(ThisFrameWriteIndex);

    AtomicIncrement(&MainThreadState->WriteIndex);
    AdvanceThreadState(MainThreadState, MainThreadState->WriteIndex);

    SharedState->ReadScopeIndex = GetNextDebugFrameIndex(SharedState->ReadScopeIndex);

    /* Record frame cycle counts */

    frame_stats *ThisFrame = SharedState->Frames + ThisFrameWriteIndex;
    ThisFrame->FrameMs = Dt * 1000.0;
    ThisFrame->TotalCycles = CurrentCycles - ThisFrame->StartingCycle;

    frame_stats *NextFrame = SharedState->Frames + NextFrameWriteIndex;
    NextFrame->StartingCycle = CurrentCycles;
  }

  return (r32)Dt;
}

min_max_avg_dt
ComputeMinMaxAvgDt()
{
  TIMED_FUNCTION();

  debug_state *SharedState = GetDebugState();

  min_max_avg_dt Dt = {};
  Dt.Min = f32_MAX;

  u32 FrameCount = 0;
  for (u32 FrameIndex = 0;
      FrameIndex < DEBUG_FRAMES_TRACKED;
      ++FrameIndex )
  {
    frame_stats *Frame = SharedState->Frames + FrameIndex;

    Dt.Min = Min(Dt.Min, Frame->FrameMs);
    Dt.Max = Max(Dt.Max, Frame->FrameMs);
    Dt.Avg += Frame->FrameMs;

    if (Frame->FrameMs > 0.0f)
    {
      ++FrameCount;
    }
  }

  if (FrameCount > 0)
  {
    Dt.Avg /= (r32)FrameCount;
  }

  return Dt;
}

#if 0
inline void
DoDebugFrameRecord(debug_recording_state *State, hotkeys *Hotkeys)
{
  {
    static b32 Toggled = False;
    if (Hotkeys->Debug_ToggleLoopedGamePlayback  && !Toggled)
    {
      Toggled = True;
      State->Mode = (debug_recording_mode)((State->Mode + 1) % RecordingMode_Count);

      switch (State->Mode)
      {
        case RecordingMode_Clear:
        {
          Log("Clear");
          State->FramesRecorded = 0;
          State->FramesPlayedBack = 0;
        } break;

        case RecordingMode_Record:
        {
          NotImplemented;
          Log("Recording");
          //CopyArena(MainMemory, &State->RecordedMainMemory);
        } break;

        case RecordingMode_Playback:
        {
          NotImplemented;
          Log("Playback");
          //CopyArena(&State->RecordedMainMemory, MainMemory);
        } break;

        InvalidDefaultCase;
      }

    }
    else if (!Hotkeys->Debug_ToggleLoopedGamePlayback)
    {
      Toggled = False;
    }
  }

  switch (State->Mode)
  {
    case RecordingMode_Clear:
    {
    } break;

    case RecordingMode_Record:
    {
      NotImplemented;
      Assert(State->FramesRecorded < DEBUG_RECORD_INPUT_SIZE);
      Hotkeys->Debug_ToggleLoopedGamePlayback = False;
      State->Inputs[State->FramesRecorded++] = *Hotkeys;
    } break;

    case RecordingMode_Playback:
    {
      NotImplemented;
      *Hotkeys = State->Inputs[State->FramesPlayedBack++];

      if (State->FramesPlayedBack == State->FramesRecorded)
      {
        State->FramesPlayedBack = 0;
        //CopyArena(&State->RecordedMainMemory, MainMemory);
      }

    } break;

    InvalidDefaultCase;
  }

  return;
}
#endif

inline memory_arena *
ThreadsafeDebugMemoryAllocator_debug_profile_scope_only()
{
  debug_state *State = GetDebugState();
  memory_arena *Arena = State->ThreadStates[ThreadLocal_ThreadIndex].MemoryFor_debug_profile_scope;
  return Arena;
}

inline memory_arena *
ThreadsafeDebugMemoryAllocator()
{
  debug_state *State = GetDebugState();
  memory_arena *Arena = State->ThreadStates[ThreadLocal_ThreadIndex].Memory;
  return Arena;
}



/*************************                       *****************************/
/*************************  Profile Scope Trees  *****************************/
/*************************                       *****************************/



debug_profile_scope *
GetProfileScope()
{
  debug_profile_scope *Result = 0;
  debug_thread_state *State = GetThreadLocalState();

  if (State)
  {
    if (State->FirstFreeScope)
    {
      Result = State->FirstFreeScope;
      State->FirstFreeScope = State->FirstFreeScope->Sibling;

      Clear(Result);
    }
    else
    {
      memory_arena *Memory = ThreadsafeDebugMemoryAllocator_debug_profile_scope_only();
      Result = AllocateProtection(debug_profile_scope, Memory, 1, False);
    }
  }

  return Result;
}

void
InitScopeTrees(u32 TotalThreadCount)
{
  Assert(ThreadLocal_ThreadIndex == 0);
  for (u32 ThreadIndex = 0;
      ThreadIndex < TotalThreadCount;
      ++ThreadIndex)
  {
    for (u32 TreeIndex = 0;
        TreeIndex < DEBUG_FRAMES_TRACKED;
        ++TreeIndex)
    {
      debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
      InitScopeTree(ThreadState->ScopeTrees + TreeIndex);
      ThreadState->WriteIndex = GetDebugState()->ReadScopeIndex + 1;
    }
  }

  return;
}



/*******************************              *********************************/
/*******************************  DrawBounds  *********************************/
/*******************************              *********************************/



inline void
AdvanceClip(layout *Layout, v2 TestP)
{
  v2 AbsP = TestP + Layout->Basis;
  while (Layout)
  {
    v2 RelP = AbsP - Layout->Basis;
    Layout->DrawBounds.Min = Min(RelP, Layout->DrawBounds.Min);
    Layout->DrawBounds.Max = Max(RelP, Layout->DrawBounds.Max);

    Layout = Layout->Prev;
  }
}

inline void
AdvanceClip(layout *Layout)
{
  AdvanceClip(Layout, Layout->At);
}

r32
GetXOffsetForHorizontalBar(u64 StartCycleOffset, u64 FrameTotalCycles, r32 TotalGraphWidth)
{
  r32 XOffset = ((r32)StartCycleOffset/(r32)FrameTotalCycles)*TotalGraphWidth;
  return XOffset;
}

inline mutex_op_record *
ReserveMutexOpRecord(mutex *Mutex, mutex_op Op, debug_state *State)
{
  if (!State->DebugDoScopeProfiling) return 0;

  mutex_op_record *Record = 0;
  debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadLocal_ThreadIndex);
  u32 WriteIndex = ThreadState->WriteIndex % DEBUG_FRAMES_TRACKED;
  mutex_op_array *MutexOps = &ThreadState->MutexOps[WriteIndex];

  if (MutexOps->NextRecord < TOTAL_MUTEX_OP_RECORDS)
  {
    Record = MutexOps->Records + MutexOps->NextRecord++;
    Record->Cycle = GetCycleCount();
    Record->Op = Op;
    Record->Mutex = Mutex;
  }
  else
  {
    Warn("Total debug mutex operations of %u exceeded, discarding record info.", TOTAL_MUTEX_OP_RECORDS);
  }

  return Record;
}

inline void
MutexWait(mutex *Mutex)
{
  /* mutex_op_record *Record = */ ReserveMutexOpRecord(Mutex, MutexOp_Waiting, GetDebugState());
  return;
}

inline void
MutexAquired(mutex *Mutex)
{
  /* mutex_op_record *Record = */ ReserveMutexOpRecord(Mutex, MutexOp_Aquired, GetDebugState());
  return;
}

inline void
MutexReleased(mutex *Mutex)
{
  /* mutex_op_record *Record = */ ReserveMutexOpRecord(Mutex, MutexOp_Released, GetDebugState());
  return;
}

mutex_op_record *
FindRecord(mutex_op_record *WaitRecord, mutex_op_record *FinalRecord, mutex_op SearchOp)
{
  Assert(WaitRecord->Op == MutexOp_Waiting);

  mutex_op_record *Result = 0;;
  mutex_op_record *SearchRecord = WaitRecord;

  while (SearchRecord < FinalRecord)
  {
    if (SearchRecord->Op == SearchOp &&
        SearchRecord->Mutex == WaitRecord->Mutex)
    {
      Result = SearchRecord;
      break;
    }

    ++SearchRecord;
  }

  return Result;
}



/*****************************              **********************************/
/*****************************  Call Graph  **********************************/
/*****************************              **********************************/


inline u32
GetTotalMutexOpsForReadFrame()
{
  u32 ReadIndex = GetDebugState()->ReadScopeIndex;

  u32 OpCount = 0;
  for (u32 ThreadIndex = 0;
      ThreadIndex < GetTotalThreadCount();
      ++ThreadIndex)
  {
    debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
    OpCount += ThreadState->MutexOps[ReadIndex].NextRecord;
  }

  return OpCount;
}

umm
GetAllocationSize(push_metadata *Meta)
{
  umm AllocationSize = Meta->StructSize*Meta->StructCount*Meta->PushCount;
  return AllocationSize;
}



/********************                               **************************/
/********************  Debug System Initialization  **************************/
/********************                               **************************/



void
InitDebugMemoryAllocationSystem(debug_state *State)
{
  u32 TotalThreadCount = GetTotalThreadCount();

  // FIXME(Jesse): Can this be allocated in a more sensible way?
  umm Size = TotalThreadCount * sizeof(debug_thread_state);

  // FIXME(Jesse): This should allocate roughly enough space (maybe more than necessary)
  // for the Size parameter, however it seems to be under-allocating, which causes
  // the PushStruct to allocate again.  Bad bad bad.
  memory_arena *BoostrapArena = AllocateArena(Size);
  DEBUG_REGISTER_ARENA(BoostrapArena);
  State->ThreadStates = (debug_thread_state*)PushStruct(BoostrapArena, Size, CACHE_LINE_SIZE);

#if BONSAI_INTERNAL

  /* The WriteIndex member gets read from multiple threads, and on x86_64 reads
   * are atomic that don't span a cache-line boundary.  This ensures we never
   * hit that case.
   */
  CAssert(sizeof(debug_thread_state) == CACHE_LINE_SIZE);
  for (u32 ThreadIndex = 0;
      ThreadIndex < TotalThreadCount;
      ++ThreadIndex)
  {
    debug_thread_state* ThreadState = GetThreadLocalStateFor(ThreadIndex);
    Assert( (umm)(&ThreadState->WriteIndex) % CACHE_LINE_SIZE < (CACHE_LINE_SIZE)-sizeof(ThreadState->WriteIndex) );
  }
#endif

  umm MetaTableSize = META_TABLE_SIZE * sizeof(push_metadata);
  for (u32 ThreadIndex = 0;
      ThreadIndex < TotalThreadCount;
      ++ThreadIndex)
  {
    debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
    Assert((umm)ThreadState % CACHE_LINE_SIZE == 0);

    memory_arena *DebugThreadArena = AllocateArena();
    ThreadState->Memory = DebugThreadArena;
    DEBUG_REGISTER_ARENA(DebugThreadArena);

    memory_arena *DebugThreadArenaFor_debug_profile_scope = AllocateArena();
    ThreadState->MemoryFor_debug_profile_scope = DebugThreadArenaFor_debug_profile_scope;
    DEBUG_REGISTER_ARENA(DebugThreadArenaFor_debug_profile_scope);

    ThreadState->MetaTable = (push_metadata*)PushStruct(ThreadsafeDebugMemoryAllocator(), MetaTableSize, CACHE_LINE_SIZE);
    ThreadState->MutexOps = AllocateAligned(mutex_op_array, DebugThreadArena, DEBUG_FRAMES_TRACKED, CACHE_LINE_SIZE);
    ThreadState->ScopeTrees = AllocateAligned(debug_scope_tree, DebugThreadArena, DEBUG_FRAMES_TRACKED, CACHE_LINE_SIZE);
  }

  return;
}


void
InitDebugDataSystem(debug_state *DebugState)
{
  InitDebugMemoryAllocationSystem(DebugState);

  u32 TotalThreadCount = GetTotalThreadCount();
  InitScopeTrees(TotalThreadCount);

  return;
}

#endif
