#if BONSAI_INTERNAL

#include <stdio.h>

debug_global b32 DebugGlobal_RedrawEveryPush = 0;


/****************************  Arena Introspection  **************************/


void
DebugRegisterArena(const char *Name, memory_arena *Arena, debug_state *DebugState)
{
  b32 Registered = False;
  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = &DebugState->RegisteredMemoryArenas[Index];

    const char *CurrentName = Current->Name;
    if (!CurrentName)
    {
      if (AtomicCompareExchange( (volatile char **)&Current->Name, Name, CurrentName ))
      {
        Current->Arena = Arena;
        Registered = True;
        break;
      }
      else
      {
        Debug("Contiue Branch");
        continue;
      }
    }
  }

  if (Registered)
  {
    Info("Registered Arena : %s", Name);
  }
  else
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
  debug_state *DebugState = GetDebugState();
  WriteToMetaTable(InputMeta, MetaTable, PushesMatchExactly);

  return;
}

inline void
ClearMetaRecordsFor(memory_arena *Arena)
{
  debug_state *DebugState = GetDebugState();

  u32 TotalThreadCount = GetWorkerThreadCount() + 1;
  for ( u32 ThreadIndex = 0;
      ThreadIndex < TotalThreadCount;
      ++ThreadIndex)
  {
    for ( u32 MetaIndex = 0;
        MetaIndex < META_TABLE_SIZE;
        ++MetaIndex)
    {
      push_metadata *Meta = GetThreadDebugState(ThreadLocal_ThreadIndex)->MetaTable + MetaIndex;
      if (Meta->ArenaHash == HashArena(Arena))
      {
        Clear(Meta);
      }
    }
  }

  return;
}


/****************************  Memory Allocator  *****************************/


inline void*
Allocate_(memory_arena *Arena, umm StructSize, umm StructCount, const char* Name, s32 Line, const char* File, umm Alignment, b32 MemProtect)
{
  umm PushSize = StructCount * StructSize;
  void* Result = PushStruct( Arena, PushSize, Alignment, MemProtect);

#ifndef BONSAI_NO_PUSH_METADATA
  push_metadata ArenaMetadata = {Name, HashArena(Arena), HashArenaHead(Arena), StructSize, StructCount, 1};
  WritePushMetadata(&ArenaMetadata, GetThreadDebugState(ThreadLocal_ThreadIndex)->MetaTable);
#endif

  if (!Result)
  {
    Error("Pushing %s on Line: %d, in file %s", Name, Line, File);
    Assert(False);
    return False;
  }

  return Result;
}


/**************************  Utility Functions  ******************************/


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
  /* TIMED_FUNCTION(); */ // Seems to behave poorly when timed.

  if (!ScopeToFree) return;

  FreeScopes(ThreadState, ScopeToFree->Child);
  FreeScopes(ThreadState, ScopeToFree->Sibling);

  Clear(ScopeToFree);

  debug_profile_scope *FirstFree = ThreadState->FirstFreeScope;
  ThreadState->FirstFreeScope = ScopeToFree;
  ScopeToFree->Sibling = FirstFree;

  return;
}

// TODO(Jesse): Replace this expression everywhere!!!
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
  ThreadState->CurrentFrame = NextFrameId;

  NextWriteTree->FrameRecorded = NextFrameId;

  return;
}

inline void
WorkerThreadAdvanceDebugSystem()
{
  Assert(ThreadLocal_ThreadIndex != 0);

#if 0
  debug_state *SharedState = GetDebugState();
  if (SharedState->MainThreadBlocksWorkerThreads)
  {
    AtomicIncrement(&SharedState->WorkerThreadsWaiting);
    while(SharedState->MainThreadBlocksWorkerThreads);
    AtomicDecrement(&SharedState->WorkerThreadsWaiting);
  }
#endif

  debug_thread_state *ThreadState = GetThreadDebugState(ThreadLocal_ThreadIndex);
  debug_thread_state *MainThreadState = GetThreadDebugState(0);
  Assert(ThreadState != MainThreadState);

  u32 NextFrameId = MainThreadState->CurrentFrame;  // TODO(Jesse): Should maybe be an atomic read?
  if (NextFrameId != ThreadState->CurrentFrame)
  {
    AdvanceThreadState(ThreadState, NextFrameId);
  }

  return;
}


r32
AdvanceDebugSystem()
{
  TIMED_FUNCTION();
  Assert(ThreadLocal_ThreadIndex == 0);

  local_persist r32 LastMs = (r32)GetHighPrecisionClock();
  r32 CurrentMS = (r32)GetHighPrecisionClock();
  r32 Dt = (r32)((CurrentMS - LastMs)/1000.0f);
  LastMs = CurrentMS;

  debug_state *SharedState = GetDebugState();
  if (!SharedState->DebugDoScopeProfiling) return Dt;

  u64 CurrentCycles = GetCycleCount();

  debug_thread_state *MainThreadState = GetThreadDebugState(0);
  u32 ThisFrameWriteIndex = MainThreadState->CurrentFrame % DEBUG_FRAMES_TRACKED;
  u32 NextFrameWriteIndex = GetNextDebugFrameIndex(ThisFrameWriteIndex);

  AtomicIncrement(&MainThreadState->CurrentFrame);
  AdvanceThreadState(MainThreadState, MainThreadState->CurrentFrame);

  SharedState->ReadScopeIndex = GetNextDebugFrameIndex(SharedState->ReadScopeIndex);

  /* Record frame cycle counts */

  frame_stats *ThisFrame = SharedState->Frames + ThisFrameWriteIndex;
  ThisFrame->FrameMs = Dt * 1000.0;
  ThisFrame->TotalCycles = CurrentCycles - ThisFrame->StartingCycle;

  frame_stats *NextFrame = SharedState->Frames + NextFrameWriteIndex;
  NextFrame->StartingCycle = CurrentCycles;

  return Dt;
}

inline void
DebugResumeWorkerThreads()
{
  TIMED_FUNCTION();
  debug_state *State = GetDebugState();
  State->MainThreadBlocksWorkerThreads = False;
  return;
}

inline void
DebugSuspendWorkerThreads()
{
  TIMED_FUNCTION();
  debug_state *State = GetDebugState();

  State->MainThreadBlocksWorkerThreads = True;

  u32 WorkerThreadCount = GetWorkerThreadCount();
  while (State->WorkerThreadsWaiting < WorkerThreadCount);

  return;
}

void
CleanupText2D(debug_text_render_group *RG)
{
  // Delete buffers
  glDeleteBuffers(1, &RG->VertexBuffer);
  glDeleteBuffers(1, &RG->UVBuffer);

  // Delete texture
  glDeleteTextures(1, &RG->FontTexture.ID);

  // Delete shader
  glDeleteProgram(RG->Text2DShader.ID);

  return;
}

min_max_avg_dt
ComputeMinMaxAvgDt()
{
  TIMED_FUNCTION();

  debug_state *SharedState = GetDebugState();

  min_max_avg_dt Dt = {};
  Dt.Min = FLT_MAX;

    for (u32 FrameIndex = 0;
        FrameIndex < DEBUG_FRAMES_TRACKED;
        ++FrameIndex )
    {
      frame_stats *Frame = SharedState->Frames + FrameIndex;

      Dt.Min = Min(Dt.Min, Frame->FrameMs);
      Dt.Max = Max(Dt.Max, Frame->FrameMs);
      Dt.Avg += Frame->FrameMs;
    }
    Dt.Avg /= (r32)DEBUG_FRAMES_TRACKED;

  return Dt;
}

inline void
DoDebugFrameRecord(
    debug_recording_state *State,
    hotkeys *Hotkeys)
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

inline memory_arena *
GetDebugMemoryAllocator()
{
  debug_state *State = GetDebugState();
  memory_arena *Arena = State->ThreadStates[ThreadLocal_ThreadIndex].Memory;
  return Arena;
}

v2
GetAbsoluteMin(layout *Layout)
{
  v2 Result = Layout->Clip.Min + Layout->Basis;
  return Result;
}

v2
GetAbsoluteMax(layout *Layout)
{
  v2 Result = Layout->Clip.Max + Layout->Basis;
  return Result;
}

v2
GetAbsoluteAt(layout *Layout)
{
  v2 Result = Layout->At + Layout->Basis;
  return Result;
}


/*************************  Profile Scope Trees  *****************************/


debug_profile_scope *
GetProfileScope(debug_thread_state *State)
{
  debug_profile_scope *Result = 0;

  if (State->FirstFreeScope)
  {
    Result = State->FirstFreeScope;
    State->FirstFreeScope = State->FirstFreeScope->Sibling;

    Clear(Result);
  }
  else
  {
    Result = AllocateProtection(debug_profile_scope, GetDebugMemoryAllocator(), 1, False);
  }

  Assert(Result);
  return Result;
}

void
InitScopeTrees(memory_arena *DebugMemory, u32 TotalThreadCount)
{
  u64 CurrentCycles = GetCycleCount();
  for (u32 ThreadIndex = 0;
      ThreadIndex < TotalThreadCount;
      ++ThreadIndex)
  {
    for (u32 TreeIndex = 0;
        TreeIndex < DEBUG_FRAMES_TRACKED;
        ++TreeIndex)
    {
      debug_thread_state *ThreadState = GetThreadDebugState(ThreadIndex);

      ThreadState->ScopeTrees[TreeIndex].Root = GetProfileScope(ThreadState);
      InitScopeTree(ThreadState->ScopeTrees + TreeIndex);
      ThreadState->CurrentFrame = GlobalDebugState->ReadScopeIndex + 1;
    }
  }

  /* debug_state *SharedState = GetDebugState(); */
  /* debug_thread_state *MainThreadState = GetThreadDebugState(0); */
  /* frame_stats *FirstFrame = SharedState->Frames + MainThreadState->WriteIndex; */
  /* FirstFrame->StartingCycle = GetCycleCount(); */

  return;
}

#if 0
void
PrintFreeScopes(debug_state *State)
{
  debug_profile_scope *Sentinel = &State->FreeScopeSentinel;
  debug_profile_scope *Current = Sentinel->Child;

  while(Current != Sentinel)
  {
    Log("%s", Current->Name);
    Current = Current->Child;
  }

  return;
}
#endif


/************************  Debug System Initialization  **********************/


b32
InitDebugOverlayFramebuffer(debug_text_render_group *RG, memory_arena *DebugArena, const char *DebugFont)
{
  glGenFramebuffers(1, &RG->FBO.ID);
  glBindFramebuffer(GL_FRAMEBUFFER, RG->FBO.ID);

  v2i ScreenDim = V2i(SCR_WIDTH, SCR_HEIGHT);

  RG->FontTexture = LoadDDS(DebugFont);
  RG->CompositedTexture = MakeTexture_RGBA( ScreenDim, 0, DebugArena);

  FramebufferTexture(&RG->FBO, RG->CompositedTexture);

  glGenBuffers(1, &RG->SolidUIVertexBuffer);
  glGenBuffers(1, &RG->SolidUIColorBuffer);

  glGenBuffers(1, &RG->VertexBuffer);
  glGenBuffers(1, &RG->UVBuffer);
  glGenBuffers(1, &RG->ColorBuffer);

  RG->Text2DShader = LoadShaders("TextVertexShader.vertexshader",
                                 "TextVertexShader.fragmentshader", DebugArena);

  RG->TextureUniformID = glGetUniformLocation(RG->Text2DShader.ID, "myTextureSampler");

  RG->DebugFontTextureShader = MakeSimpleTextureShader(&RG->FontTexture, DebugArena);
  RG->DebugTextureShader = MakeSimpleTextureShader(RG->CompositedTexture, DebugArena);

  if (!CheckAndClearFramebuffer())
    return False;

  return True;
}

void
AllocateAndInitGeoBuffer(textured_2d_geometry_buffer *Geo, u32 VertCount, memory_arena *DebugArena)
{
  Geo->Verts = Allocate(v3, DebugArena, VertCount);
  Geo->Colors = Allocate(v3, DebugArena, VertCount);
  Geo->UVs = Allocate(v2, DebugArena, VertCount);

  Geo->End = VertCount;
  Geo->At = 0;
}

void
AllocateAndInitGeoBuffer(untextured_2d_geometry_buffer *Geo, u32 VertCount, memory_arena *DebugArena)
{
  Geo->Verts = Allocate(v3, DebugArena, VertCount);
  Geo->Colors = Allocate(v3, DebugArena, VertCount);

  Geo->End = VertCount;
  Geo->At = 0;
  return;
}

shader
MakeSolidUIShader(memory_arena *Memory)
{
  shader SimpleTextureShader = LoadShaders( "SimpleColor.vertexshader",
                                            "SimpleColor.fragmentshader",
                                             Memory);
  return SimpleTextureShader;
}

void
InitDebugMemoryAllocationSystem(debug_state *State)
{
  u32 TotalThreadCount = GetTotalThreadCount();

  // FIXME(Jesse): Can this be allocated in a more sensible way?
  umm Size = TotalThreadCount * sizeof(debug_thread_state);

  // FIXME(Jesse): This should allocate roughly enough space (maybe more than necessary)
  // for the Size parameter, however it seems to be under-allocating, which causes
  // the PushStruct to allocate again.  Bad bad bad.
  memory_arena *BoostrapArena = PlatformAllocateArena(Size);
  DEBUG_REGISTER_ARENA(BoostrapArena, State);
  GlobalDebugState->ThreadStates = (debug_thread_state*)PushStruct(BoostrapArena, Size, 64);
  //

  umm MetaTableSize = META_TABLE_SIZE * sizeof(push_metadata);
  for (u32 ThreadIndex = 0;
      ThreadIndex < TotalThreadCount;
      ++ThreadIndex)
  {
    debug_thread_state *ThreadState = GetThreadDebugState(ThreadIndex);
    Assert((umm)ThreadState % 64 == 0);

    memory_arena *DebugThreadArena = PlatformAllocateArena();
    ThreadState->Memory = DebugThreadArena;
    DEBUG_REGISTER_ARENA(DebugThreadArena, State);

    ThreadState->MetaTable = (push_metadata*)PushStruct(GetDebugMemoryAllocator(), MetaTableSize, 64);
    ThreadState->MutexOps = AllocateAligned(mutex_op_array, DebugThreadArena, DEBUG_FRAMES_TRACKED, 64);
    ThreadState->ScopeTrees = AllocateAligned(debug_scope_tree, DebugThreadArena, DEBUG_FRAMES_TRACKED, 64);
  }

  return;
}

void
InitDebugState(debug_state *DebugState)
{
  GlobalDebugState = DebugState;

  GlobalDebugState->Initialized = True;
  u32 TotalThreadCount = GetTotalThreadCount();

  InitDebugMemoryAllocationSystem(DebugState);

  InitScopeTrees(GetDebugMemoryAllocator(), TotalThreadCount);

  AllocateMesh(&GlobalDebugState->LineMesh, 1024, GetDebugMemoryAllocator());

  if (!InitDebugOverlayFramebuffer(&GlobalDebugState->TextRenderGroup, GetDebugMemoryAllocator(), "Holstein.DDS"))
  { Error("Initializing Debug Overlay Framebuffer"); }

  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup.TextGeo, 1024, GetDebugMemoryAllocator());
  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup.UIGeo, 1024, GetDebugMemoryAllocator());

  GlobalDebugState->TextRenderGroup.SolidUIShader = MakeSolidUIShader(GetDebugMemoryAllocator());

  GlobalDebugState->SelectedArenas = Allocate(selected_arenas, GetDebugMemoryAllocator(), 1);

  return;
}


/*******************************  Clip Rect **********************************/


inline void
AdvanceClip(layout *Layout, v2 TestP)
{
  Layout->Clip.Min = Min(TestP, Layout->Clip.Min);
  Layout->Clip.Max = Max(TestP, Layout->Clip.Max);
  return;
}

inline void
AdvanceClip(layout *Layout)
{
  Layout->Clip.Min = Min(Layout->At, Layout->Clip.Min);
  Layout->Clip.Max = Max(Layout->At, Layout->Clip.Max);
  return;
}

inline void
BeginClipRect(layout *Layout)
{
  Layout->Clip = {V2(FLT_MAX, FLT_MAX), V2(-FLT_MAX, -FLT_MAX)};
  return;
}

v2 BufferQuad(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z);
void BufferColors(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v3 Color);

void
EndClipRect(ui_render_group *Group, layout *Layout, untextured_2d_geometry_buffer *Geo)
{
  v2 MinP = Layout->Clip.Min + Layout->Basis;
  v2 Dim = (Layout->Clip.Max + Layout->Basis) - Layout->Clip.Min;

  BufferQuad(Group, Geo, MinP, Dim, 0.0f);
  BufferColors(Group, Geo, V3(0.2f));
  Geo->At+=6;

  return;
}


/******************************  2D Buffering ********************************/


void
FlushBuffer(debug_text_render_group *RG, untextured_2d_geometry_buffer *Buffer, v2 ScreenDim)
{
  TIMED_FUNCTION();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(ScreenDim);
  UseShader(&RG->SolidUIShader);

  u32 AttributeIndex = 0;
  BufferVertsToCard(RG->SolidUIVertexBuffer, Buffer, &AttributeIndex);
  BufferColorsToCard(RG->SolidUIColorBuffer, Buffer, &AttributeIndex);

  Draw(Buffer->At);
  Buffer->At = 0;

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  AssertNoGlErrors;

  return;
}

void
FlushBuffer(debug_text_render_group *RG, textured_2d_geometry_buffer *Geo, v2 ScreenDim)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(ScreenDim);
  glUseProgram(RG->Text2DShader.ID);

  // Bind Font texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RG->FontTexture.ID);
  glUniform1i(RG->TextureUniformID, 0);

  u32 AttributeIndex = 0;
  BufferVertsToCard(RG->SolidUIVertexBuffer, Geo, &AttributeIndex);
  BufferUVsToCard(RG->UVBuffer, Geo, &AttributeIndex);
  BufferColorsToCard(RG->SolidUIColorBuffer, Geo, &AttributeIndex);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Draw(Geo->At);
  Geo->At = 0;

  glDisable(GL_BLEND);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

  AssertNoGlErrors;
}

void
BufferTextUVs(textured_2d_geometry_buffer *Geo, v2 UV)
{
  v2 uv_up_left    = V2( UV.x           , UV.y );
  v2 uv_up_right   = V2( UV.x+1.0f/16.0f, UV.y );
  v2 uv_down_right = V2( UV.x+1.0f/16.0f, (UV.y + 1.0f/16.0f) );
  v2 uv_down_left  = V2( UV.x           , (UV.y + 1.0f/16.0f) );

  u32 StartingIndex = Geo->At;
  Geo->UVs[StartingIndex++] = uv_up_left;
  Geo->UVs[StartingIndex++] = uv_down_left;
  Geo->UVs[StartingIndex++] = uv_up_right;

  Geo->UVs[StartingIndex++] = uv_down_right;
  Geo->UVs[StartingIndex++] = uv_up_right;
  Geo->UVs[StartingIndex++] = uv_down_left;

  return;
}

void
BufferColors(v3 *Colors, u32 StartingIndex, v3 Color)
{
  Colors[StartingIndex++] = Color;
  Colors[StartingIndex++] = Color;
  Colors[StartingIndex++] = Color;
  Colors[StartingIndex++] = Color;
  Colors[StartingIndex++] = Color;
  Colors[StartingIndex++] = Color;
  return;
}

void
BufferColors(ui_render_group *Group, textured_2d_geometry_buffer *Geo, v3 Color)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  BufferColors(Geo->Colors, Geo->At, Color);
}

void
BufferColors(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v3 Color)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  BufferColors(Geo->Colors, Geo->At, Color);
}

v2
BufferQuadDirect(v3 *Dest, u32 StartingIndex, v2 MinP, v2 Dim, r32 Z, v2 ScreenDim )
{
  v3 vertex_up_left    = V3( MinP.x       , MinP.y       , Z);
  v3 vertex_up_right   = V3( MinP.x+Dim.x , MinP.y       , Z);
  v3 vertex_down_right = V3( MinP.x+Dim.x , MinP.y+Dim.y , Z);
  v3 vertex_down_left  = V3( MinP.x       , MinP.y+Dim.y , Z);

  v3 ToClipSpace = (1.0f / V3(ScreenDim.x, ScreenDim.y, 1.0f));

  // Native OpenGL screen coordinates are {0,0} at the bottom-left corner. This
  // maps the origin to the top-left of the screen.
  //
  // It is also true that Z goes _into_ the screen, which makes 1.0f the bottom
  // of the unit cube.  It makes more sense to think of 1.0f as the top of the
  // cube when stacking UI elements such as text, so invert that too.
  v3 InvertYZ = V3(1.0f, -1.0f, -1.0f);

  Dest[StartingIndex++] = InvertYZ * ((vertex_up_left    * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertYZ * ((vertex_down_left  * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertYZ * ((vertex_up_right   * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertYZ * ((vertex_down_right * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertYZ * ((vertex_up_right   * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertYZ * ((vertex_down_left  * ToClipSpace) * 2.0f - 1);

  v2 Max = vertex_down_right.xy;
  return Max;
}

inline v2
BufferQuad(ui_render_group *Group, textured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z = 0.5f)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  v2 Result = BufferQuadDirect(Geo->Verts, Geo->At, MinP, Dim, Z, Group->ScreenDim);
  return Result;
}

inline v2
BufferQuad(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z = 0.5f)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  v2 MaxP = BufferQuadDirect(Geo->Verts, Geo->At, MinP, Dim, Z, Group->ScreenDim);
  return MaxP;
}

inline r32
BufferChar(ui_render_group *Group, textured_2d_geometry_buffer *Geo, u32 CharIndex, v2 MinP, font *Font, const char *Text, u32 Color)
{
  char Char = Text[CharIndex];
  v2 UV = V2( (Char%16)/16.0f, (Char/16)/16.0f );

  { // Black Drop-shadow
    BufferQuad(Group, Geo, MinP+V2(3), V2(Font->Size), 1.0f);
    BufferTextUVs(Geo, UV);
    BufferColors(Group, Geo, getDefaultPalette()[BLACK].xyz);
    Geo->At += 6;
  }

  v2 MaxP = BufferQuad(Group, Geo, MinP, V2(Font->Size), 1.0f);
  BufferTextUVs(Geo, UV);
  BufferColors(Group, Geo, getDefaultPalette()[Color].xyz);
  Geo->At += 6;

  r32 DeltaX = (MaxP.x - MinP.x);

  return DeltaX;
}

r32
BufferTextAt(ui_render_group *Group, v2 BasisP, font *Font, const char *Text, u32 Color)
{
  textured_2d_geometry_buffer *Geo = &Group->TextGroup->TextGeo;

  s32 QuadCount = (s32)strlen(Text);

  r32 DeltaX = 0;

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = BasisP + V2(Font->Size*CharIndex, 0);
    DeltaX += BufferChar(Group, Geo, CharIndex, MinP, Font, Text, Color);
    continue;
  }

  return DeltaX;
}

r32
BufferText(const char* Text, u32 Color, layout *Layout, font *Font, ui_render_group *Group)
{
  textured_2d_geometry_buffer *Geo = &Group->TextGroup->TextGeo;

  s32 QuadCount = (s32)strlen(Text);

  r32 DeltaX = 0;

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = Layout->Basis + Layout->At + V2(Font->Size*CharIndex, 0);
    DeltaX += BufferChar(Group, Geo, CharIndex, MinP, Font, Text, Color);
    continue;
  }

  Layout->At.x += DeltaX;
  AdvanceClip(Layout);
  return DeltaX;
}

inline void
BufferValue(const char *Text, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  r32 DeltaX = BufferText(Text, ColorIndex, Layout, &Group->Font, Group);
  return;
}

inline void
BufferValue(r32 Number, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%f", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex);
  return;
}

inline void
BufferValue(u64 Number, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex);
  return;
}

inline void
BufferValue(u32 Number, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex);
  return;
}


/*****************************  Text Helpers  ********************************/

char *
FormatString(const char* FormatString, ...)
{
  char *Buffer = AllocateProtection(char, TranArena, 1024, False);

  va_list Arguments;
  va_start(Arguments, FormatString);
  vsnprintf(Buffer, 1023, FormatString, Arguments);
  va_end(Arguments);

  return Buffer;
}

inline void
AdvanceSpaces(u32 N, layout *Layout, font *Font)
{
  Layout->At.x += (N*Font->Size);
  AdvanceClip(Layout);
  return;
}

inline void
NewLine(layout *Layout, font *Font)
{
  Layout->At.y += (Font->LineHeight);
  Layout->At.x = 0;
  AdvanceSpaces(Layout->Depth, Layout, Font);
  AdvanceClip(Layout);
  return;
}

r32
BufferLine(const char* Text, u32 Color, layout *Layout, font *Font, ui_render_group *Group)
{
  r32 xOffset = BufferText(Text, Color, Layout, Font, Group);
  NewLine(Layout, Font);
  return xOffset;
}

inline void
NewRow(table_layout *Table, font *Font)
{
  Table->ColumnIndex = 0;
  NewLine(&Table->Layout, Font);
  return;
}

inline char*
MemorySize(u64 Number)
{
  r64 KB = (r64)Kilobytes(1);
  r64 MB = (r64)Megabytes(1);
  r64 GB = (r64)Gigabytes(1);

  r64 Display = (r64)Number;
  char Units = ' ';

  if (Number >= KB && Number < MB)
  {
    Display = Number / KB;
    Units = 'K';
  }
  else if (Number >= MB && Number < GB)
  {
    Display = Number / MB;
    Units = 'M';
  }
  else if (Number >= GB)
  {
    Display = Number / GB;
    Units = 'G';
  }


  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%.1f%c", (r32)Display, Units);
  return Buffer;
}

inline char*
FormatU32(u32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%u", Number);
  return Buffer;
}

inline char*
FormatMemorySize(u64 Number)
{
  r64 KB = (r64)Kilobytes(1);
  r64 MB = (r64)Megabytes(1);
  r64 GB = (r64)Gigabytes(1);

  r64 Display = (r64)Number;
  char Units = ' ';

  if (Number >= KB && Number < MB)
  {
    Display = Number / KB;
    Units = 'K';
  }
  else if (Number >= MB && Number < GB)
  {
    Display = Number / MB;
    Units = 'M';
  }
  else if (Number >= GB)
  {
    Display = Number / GB;
    Units = 'G';
  }

#if 0
  char *Buffer = AllocateProtection(char, TranArena, Megabytes(1, False));

  for (u32 Index = 0;
      Index < Megabytes(1);
      ++Index)
  {
    Buffer[Index] = 0;
  }

#endif

  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%.1f%c", (r32)Display, Units);

  return Buffer;
}

inline void
BufferMemorySize(u64 Number, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char *Buffer = FormatMemorySize(Number);
  BufferValue( Buffer, Group, Layout, ColorIndex);
  return;
}

inline char*
FormatThousands(u64 Number)
{
  u64 OneThousand = 1000;
  r32 Display = (r32)Number;
  char Units = ' ';

  if (Number >= OneThousand)
  {
    Display = Number / (r32)OneThousand;
    Units = 'K';
  }

  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%.1f%c", Display, Units);

  return Buffer;
}

inline void
BufferThousands(u64 Number, ui_render_group *Group, layout *Layout, u32 ColorIndex, u32 Columns = 10)
{
  char  *Buffer = FormatThousands(Number);

  {
    u32 Len = (u32)strlen(Buffer);
    u32 Pad = Max(Columns-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }

  BufferValue( Buffer, Group, Layout, ColorIndex);

  return;
}

inline void
BufferColumn( s32 Value, u32 ColumnWidth, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%d", Value);
  {
    u32 Len = (u32)strlen(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex);
  return;
}

inline void
BufferColumn( u32 Value, u32 ColumnWidth, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Value);
  {
    u32 Len = (u32)strlen(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex);
  return;
}

inline void
BufferColumn( u64 Value, u32 ColumnWidth, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Value);
  {
    u32 Len = (u32)strlen(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex);
  return;
}

inline void
BufferColumn( r64 Perc, u32 ColumnWidth, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%4.1lf", Perc);
  {
    u32 Len = (u32)strlen(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex);
  return;
}

inline void
BufferColumn( r32 Perc, u32 ColumnWidth, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%.1f", Perc);
  {
    u32 Len = (u32)strlen(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex);
  return;
}

inline void
BufferScopeTreeEntry(ui_render_group *Group, debug_profile_scope *Scope, layout *Layout,
    u32 Color, u64 TotalCycles, u64 TotalFrameCycles, u64 CallCount, u32 Depth)
{
  Assert(TotalFrameCycles);

  r32 Percentage = 100.0f * (r32)SafeDivide0((r64)TotalCycles, (r64)TotalFrameCycles);
  u64 AvgCycles = (u64)SafeDivide0(TotalCycles, CallCount);

  BufferColumn(Percentage, 6, Group, Layout, Color);
  BufferThousands(AvgCycles,  Group, Layout, Color);
  BufferColumn(CallCount, 5,  Group, Layout, Color);

  AdvanceSpaces((Depth*2)+1, Layout, &Group->Font);

  if (Scope->Expanded && Scope->Child)
  {
    BufferValue("-", Group, Layout, Color);
  }
  else if (Scope->Child)
  {
    BufferValue("+", Group, Layout, Color);
  }
  else
  {
    AdvanceSpaces(1, Layout, &Group->Font);
  }

  BufferValue(Scope->Name, Group, Layout, Color);
  NewLine(Layout, &Group->Font);

  return;
}

inline rect2
GetNextLineBounds(layout *Layout, font *Font)
{
  v2 StartingP = GetAbsoluteAt(Layout);

  // FIXME(Jesse): Should line length be systemized somehow?
  v2 EndingP = StartingP + V2(100000.0f, Font->LineHeight);
  rect2 Result = { StartingP, EndingP };
  return Result;
}

inline b32
IsInsideRect(rect2 Rect, v2 P)
{
  b32 Result = (P > Rect.Min && P < Rect.Max);
  return Result;
}

scope_stats
GetStatsFor( debug_profile_scope *Scope, debug_profile_scope *Root)
{
  scope_stats Result = {};

  debug_profile_scope *Next = Root;
  if (Scope->Parent) Next = Scope->Parent->Child; // Selects first sibling

  while (Next)
  {
    if (Next == Scope) // Find Ourselves
    {
      if (Result.Calls == 0) // We're first
      {
        Result.IsFirst = True;
      }
    }

    if (StringsMatch(Next->Name, Scope->Name))
    {
      ++Result.Calls;
      Result.CumulativeCycles += Next->CycleCount;

      if (!Result.MinScope || Next->CycleCount < Result.MinScope->CycleCount)
        Result.MinScope = Next;

      if (!Result.MaxScope || Next->CycleCount > Result.MaxScope->CycleCount)
        Result.MaxScope = Next;
    }

    Next = Next->Sibling;
  }

  return Result;
}

template <typename T> u8
HoverAndClickExpand(ui_render_group *Group, layout *Layout, T *Expandable, u8 Color, u8 HoverColor)
{
  u8 DrawColor = Color;

  {
    rect2 EntryBounds = GetNextLineBounds(Layout, &Group->Font);
    if ( IsInsideRect(EntryBounds, Group->MouseP) )
    {
      DrawColor = HoverColor;
      if (Group->Input->LMB.WasPressed)
        Expandable->Expanded = !Expandable->Expanded;
    }
  }

  return DrawColor;
}

void
BufferFirstCallToEach(ui_render_group *Group, debug_profile_scope *Scope, debug_profile_scope *TreeRoot, memory_arena *Memory, layout *Layout, u64 TotalFrameCycles, u32 Depth)
{
  if (!Scope) return;

  if (Scope->Name)
  {
    if (!Scope->Stats)
    {
      Scope->Stats = AllocateProtection(scope_stats, Memory, 1, False);
      *Scope->Stats = GetStatsFor(Scope, TreeRoot);
    }

    if (Scope->Stats->IsFirst)
    {
      u32 MainColor = HoverAndClickExpand(Group, Layout, Scope, WHITE, TEAL);

      BufferScopeTreeEntry(Group, Scope, Layout, MainColor, Scope->Stats->CumulativeCycles, TotalFrameCycles, Scope->Stats->Calls, Depth);

      if (Scope->Expanded)
        BufferFirstCallToEach(Group, Scope->Stats->MaxScope->Child, TreeRoot, Memory, Layout, TotalFrameCycles, Depth+1);
    }
  }

  BufferFirstCallToEach(Group, Scope->Sibling, TreeRoot, Memory, Layout, TotalFrameCycles, Depth);

  return;
}

inline void
PadBottom(layout *Layout, r32 Pad)
{
  Layout->At.y += Pad;
}

void
SetFontSize(font *Font, r32 FontSize)
{
  Font->Size = FontSize;
  Font->LineHeight = FontSize * 1.3f;
  return;
}

void
Column(const char* ColumnText, ui_render_group *Group, table_layout *Table, u8 Color)
{
  Table->ColumnIndex = (Table->ColumnIndex+1)%MAX_TABLE_COLUMNS;
  table_column *Column = &Table->Columns[Table->ColumnIndex];

  u32 TextLength = (u32)strlen(ColumnText);
  Column->Max = Max(Column->Max, TextLength + 1);

  u32 Pad = Column->Max - TextLength;
  AdvanceSpaces(Pad, &Table->Layout, &Group->Font);

  BufferValue(ColumnText, Group, &Table->Layout, Color);

  return;
}

void
DoTooltip(ui_render_group *Group, const char *Text)
{
  BufferTextAt(Group, Group->MouseP + V2(12, -7), &Group->Font, Text, WHITE);
  return;
}


/****************************  Mutex Introspection  **************************/


r32
GetXOffsetForHorizontalBar(u64 StartCycleOffset, u64 FrameTotalCycles, r32 TotalGraphWidth)
{
  r32 XOffset = ((r32)StartCycleOffset/(r32)FrameTotalCycles)*TotalGraphWidth;
  return XOffset;
}

void
DrawCycleBar( cycle_range *Range, cycle_range *Frame, r32 TotalGraphWidth, const char *Tooltip, v3 Color,
              ui_render_group *Group, untextured_2d_geometry_buffer *Geo, layout *Layout)
{
  Assert(Frame->StartCycle < Range->StartCycle);

  r32 FramePerc = (r32)Range->TotalCycles / (r32)Frame->TotalCycles;

  r32 BarHeight = Group->Font.Size;
  r32 BarWidth = FramePerc*TotalGraphWidth;
  v2 BarDim = V2(BarWidth, BarHeight);

  // Advance to the appropriate starting place along graph
  u64 StartCycleOffset = Range->StartCycle - Frame->StartCycle;
  r32 XOffset = GetXOffsetForHorizontalBar(StartCycleOffset, Frame->TotalCycles, TotalGraphWidth);

  v2 MinP = Layout->At + Layout->Basis + V2(XOffset, 0);
  v2 QuadMaxP = BufferQuad(Group, Geo, MinP, BarDim);
  b32 Hovering = IsInsideRect(RectMinDim(MinP, BarDim), Group->MouseP);

  if (Hovering)
  {
    Color *= 0.5f;
    if (Tooltip) { DoTooltip(Group, Tooltip); }
  }

  BufferColors(Group, Geo, Color);
  Geo->At+=6;

  AdvanceClip(Layout, QuadMaxP);

  return;
}

inline mutex_op_record *
GetMutexOpRecord(mutex *Mutex, mutex_op Op, debug_state *State)
{
  if (!State->DebugDoScopeProfiling) return 0;

  mutex_op_record *Record = 0;
  debug_thread_state *ThreadState = GetThreadDebugState(ThreadLocal_ThreadIndex);
  u32 WriteIndex = ThreadState->CurrentFrame % DEBUG_FRAMES_TRACKED;
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
DebugTimedMutexWaiting(mutex *Mutex)
{
  mutex_op_record *Record = GetMutexOpRecord(Mutex, MutexOp_Waiting, GetDebugState());
  return;
}

inline void
DebugTimedMutexAquired(mutex *Mutex)
{
  mutex_op_record *Record = GetMutexOpRecord(Mutex, MutexOp_Aquired, GetDebugState());
  return;
}

inline void
DebugTimedMutexReleased(mutex *Mutex)
{
  mutex_op_record *Record = GetMutexOpRecord(Mutex, MutexOp_Released, GetDebugState());
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

void
DrawWaitingBar(mutex_op_record *WaitRecord, mutex_op_record *AquiredRecord, mutex_op_record *ReleasedRecord,
               ui_render_group *Group, layout *Layout, font *Font, u64 FrameStartingCycle, u64 FrameTotalCycles, r32 TotalGraphWidth)
{
  Assert(WaitRecord->Op == MutexOp_Waiting);
  Assert(AquiredRecord->Op == MutexOp_Aquired);
  Assert(ReleasedRecord->Op == MutexOp_Released);

  Assert(AquiredRecord->Mutex == WaitRecord->Mutex);
  Assert(ReleasedRecord->Mutex == WaitRecord->Mutex);

  u64 WaitCycleCount = AquiredRecord->Cycle - WaitRecord->Cycle;
  u64 AquiredCycleCount = ReleasedRecord->Cycle - AquiredRecord->Cycle;

  u64 StartCycleOffset = FrameStartingCycle - WaitRecord->Cycle;
  u32 xOffset = GetXOffsetForHorizontalBar(StartCycleOffset, FrameTotalCycles, TotalGraphWidth);

  untextured_2d_geometry_buffer *Geo = &Group->TextGroup->UIGeo;
  cycle_range FrameRange = {FrameStartingCycle, FrameTotalCycles};

  cycle_range WaitRange = {WaitRecord->Cycle, WaitCycleCount};
  DrawCycleBar( &WaitRange, &FrameRange, TotalGraphWidth, 0, V3(1, 0, 0), Group, Geo, Layout);

  cycle_range AquiredRange = {AquiredRecord->Cycle, AquiredCycleCount};
  DrawCycleBar( &AquiredRange, &FrameRange, TotalGraphWidth, 0, V3(0, 1, 0), Group, Geo, Layout);

  return;
}


/************************  Thread Perf Bargraph ******************************/


void
DrawScopeBarsRecursive(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, debug_profile_scope *Scope,
                       layout *Layout, cycle_range *Frame, r32 TotalGraphWidth, random_series *Entropy)
{
  if (!Scope) return;
  Assert(Scope->Name);

  cycle_range Range = {Scope->StartingCycle, Scope->CycleCount};

  DrawCycleBar( &Range, Frame, TotalGraphWidth, Scope->Name, RandomV3(Entropy), Group, Geo, Layout);

  if (Scope->Expanded)
  {
    layout ChildrensLayout = *Layout;
    NewLine(&ChildrensLayout, &Group->Font);
    DrawScopeBarsRecursive(Group, Geo, Scope->Child, &ChildrensLayout, Frame, TotalGraphWidth, Entropy);
    Layout->Clip = ChildrensLayout.Clip;
  }

  DrawScopeBarsRecursive(Group, Geo, Scope->Sibling, Layout, Frame, TotalGraphWidth, Entropy);

  return;
}

void
BufferHorizontalBar(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, layout *Layout,
                    r32 TotalGraphWidth, v3 Color)
{
  v2 MinP = Layout->At + Layout->Basis;
  v2 BarDim = V2(TotalGraphWidth, Group->Font.LineHeight);

  BufferQuad(Group, Geo, MinP, BarDim);
  BufferColors(Group, Geo, Color);
  Geo->At+=6;

  Layout->At.x += TotalGraphWidth;

  return;
}

void
DebugDrawCycleThreadGraph(ui_render_group *Group, debug_state *SharedState, layout *Layout)
{
  random_series Entropy = {};
  r32 TotalGraphWidth = 2000.0f;
  untextured_2d_geometry_buffer *Geo = &Group->TextGroup->UIGeo;

  NewLine(Layout, &Group->Font);
  SetFontSize(&Group->Font, 36);
  NewLine(Layout, &Group->Font);

  r32 MinY = Layout->At.y;

  u32 TotalThreadCount                = GetTotalThreadCount();
  frame_stats *FrameStats             = SharedState->Frames + SharedState->ReadScopeIndex;
  cycle_range FrameCycles             = {FrameStats->StartingCycle, FrameStats->TotalCycles};

  debug_thread_state *MainThreadState = GetThreadDebugState(0);
  debug_scope_tree *MainThreadReadTree    = MainThreadState->ScopeTrees + SharedState->ReadScopeIndex;

  for ( u32 ThreadIndex = 0;
        ThreadIndex < TotalThreadCount;
        ++ThreadIndex)
  {
    BeginClipRect(Layout);

    NewLine(Layout, &Group->Font);
    char *ThreadName = FormatString("Thread %u", ThreadIndex);
    BufferLine(ThreadName, WHITE, Layout, &Group->Font, Group);

    debug_thread_state *ThreadState = GetThreadDebugState(ThreadIndex);
    debug_scope_tree *ReadTree = ThreadState->ScopeTrees + SharedState->ReadScopeIndex;
    if (MainThreadReadTree->FrameRecorded == ReadTree->FrameRecorded)
    {
      DrawScopeBarsRecursive(Group, Geo, ReadTree->Root, Layout, &FrameCycles, TotalGraphWidth, &Entropy);
    }

    Layout->At.y = Layout->Clip.Max.y; // Advance vertical at for next thread

    EndClipRect(Group, Layout, Geo);
  }

  NewLine(Layout, &Group->Font);

  r32 TotalMs = FrameStats->FrameMs;

  if (TotalMs)
  {
    r32 MarkerWidth = 3.0f;

    {
      r32 FramePerc = 16.66666f/TotalMs;
      r32 xOffset = FramePerc*TotalGraphWidth;
      v2 MinP16ms = { xOffset, MinY };
      v2 MaxP16ms = { xOffset+MarkerWidth, Layout->At.y };
      v2 Dim = MaxP16ms - MinP16ms;
      BufferQuad(Group, Geo, MinP16ms, Dim);
      BufferColors(Group, Geo, V3(0,1,0));
      Geo->At+=6;
    }
    {
      r32 FramePerc = 33.333333f/TotalMs;
      r32 xOffset = FramePerc*TotalGraphWidth;
      v2 MinP16ms = { xOffset, MinY };
      v2 MaxP16ms = { xOffset+MarkerWidth, Layout->At.y };
      v2 Dim = MaxP16ms - MinP16ms;
      BufferQuad(Group, Geo, MinP16ms, Dim);
      BufferColors(Group, Geo, V3(1,1,0));
      Geo->At+=6;
    }
  }

  frame_stats *Frame = SharedState->Frames + SharedState->ReadScopeIndex;
  u64 FrameTotalCycles = Frame->TotalCycles;
  u64 FrameStartingCycle = Frame->StartingCycle;

#if 0
  u32 UnclosedMutexRecords = 0;
  u32 TotalMutexRecords = 0;
  TIMED_BLOCK("Mutex Record Collation");
  for ( u32 ThreadIndex = 0;
        ThreadIndex < TotalThreadCount;
        ++ThreadIndex)
  {
    debug_thread_state *ThreadState = GetThreadDebugState(ThreadIndex);
    mutex_op_array *MutexOps = ThreadState->MutexOps + SharedState->ReadScopeIndex;
    mutex_op_record *FinalRecord = MutexOps->Records + MutexOps->NextRecord;

    for (u32 OpRecordIndex = 0;
        OpRecordIndex < MutexOps->NextRecord;
        ++OpRecordIndex)
    {
      mutex_op_record *CurrentRecord = MutexOps->Records + OpRecordIndex;
      if (CurrentRecord->Op == MutexOp_Waiting)
      {
        mutex_op_record *Aquired = FindRecord(CurrentRecord, FinalRecord, MutexOp_Aquired);
        mutex_op_record *Released = FindRecord(CurrentRecord, FinalRecord, MutexOp_Released);
        if (Aquired && Released)
        {
          r32 yOffset = ThreadIndex * Group->Font.LineHeight;
          Layout->At.y += yOffset;
          DrawWaitingBar(CurrentRecord, Aquired, Released, Group, Layout, &Group->Font, FrameStartingCycle, FrameTotalCycles, TotalGraphWidth);
          Layout->At.y -= yOffset;
        }
        else
        {
          Warn("Unclosed Mutex Record at %u on thread %u", OpRecordIndex, ThreadIndex);
        }
      }
    }
  }
  END_BLOCK("Mutex Record Collation");
#endif

  return;
}


/*****************************  Call Graph  **********************************/


inline u32
GetTotalMutexOpsForReadFrame()
{
  debug_state *GlobalDebugState = GetDebugState();
  u32 ReadIndex = GlobalDebugState->ReadScopeIndex;

  u32 OpCount = 0;
  for (u32 ThreadIndex = 0;
      ThreadIndex < GetTotalThreadCount();
      ++ThreadIndex)
  {
    debug_thread_state *ThreadState = GetThreadDebugState(ThreadIndex);
    OpCount += ThreadState->MutexOps[ReadIndex].NextRecord;
  }

  return OpCount;
}

void
DebugDrawCallGraph(ui_render_group *Group, debug_state *DebugState, layout *Layout, r64 MaxMs)
{
  v2 MouseP = Group->MouseP;

  NewLine(Layout, &Group->Font);
  SetFontSize(&Group->Font, 80);

  TIMED_BLOCK("Frame Ticker");
    v2 StartingAt = Layout->At;

    for (u32 FrameIndex = 0;
        FrameIndex < DEBUG_FRAMES_TRACKED;
        ++FrameIndex )
    {
      frame_stats *Frame = DebugState->Frames + FrameIndex;
      r32 Perc = (r32)SafeDivide0(Frame->FrameMs, MaxMs);

      v2 MinP = Layout->At;
      v2 MaxDim = V2(15.0, Group->Font.Size);

      v3 Color = V3(0.5f, 0.5f, 0.5f);

      debug_scope_tree *Tree = &DebugState->ThreadStates[ThreadLocal_ThreadIndex].ScopeTrees[FrameIndex];
      if ( Tree == DebugState->GetWriteScopeTree() )
      {
        Color = V3(0.8f, 0.0f, 0.0f);
        Perc = 0.05f;
      }

      if ( Tree == DebugState->GetReadScopeTree() )
        Color = V3(0.8f, 0.8f, 0.0f);

      v2 QuadDim = V2(15.0, (Group->Font.Size) * Perc);
      v2 Offset = MaxDim - QuadDim;

      v2 DrawDim = BufferQuad(Group, &Group->TextGroup->UIGeo, MinP + Offset, QuadDim);
      Layout->At.x = DrawDim.x + 5.0f;

      if (MouseP > MinP && MouseP < DrawDim)
      {
        debug_thread_state *MainThreadState = GetThreadDebugState(0);
        if (FrameIndex != MainThreadState->CurrentFrame % DEBUG_FRAMES_TRACKED)
        {
          DebugState->ReadScopeIndex = FrameIndex;
          Color = V3(0.8f, 0.8f, 0.0f);
        }
      }

      BufferColors(Group->TextGroup->UIGeo.Colors, Group->TextGroup->UIGeo.At, Color);

      Group->TextGroup->UIGeo.At+=6;
    }


    r32 MaxBarHeight = Group->Font.Size;
    v2 QuadDim = V2(Layout->At.x, 2.0f);
    {
      r32 MsPerc = (r32)SafeDivide0(33.333, MaxMs);
      r32 MinPOffset = MaxBarHeight * MsPerc;
      v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size - MinPOffset };

      BufferQuad(Group, &Group->TextGroup->UIGeo, MinP, QuadDim);
      BufferColors(Group->TextGroup->UIGeo.Colors, Group->TextGroup->UIGeo.At, V3(1,1,0));
      Group->TextGroup->UIGeo.At+=6;
    }

    {
      r32 MsPerc = (r32)SafeDivide0(16.666, MaxMs);
      r32 MinPOffset = MaxBarHeight * MsPerc;
      v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size - MinPOffset };

      BufferQuad(Group, &Group->TextGroup->UIGeo, MinP, QuadDim);
      BufferColors(Group->TextGroup->UIGeo.Colors, Group->TextGroup->UIGeo.At, V3(0,1,0));
      Group->TextGroup->UIGeo.At+=6;
    }

    { // Current ReadTree info
      SetFontSize(&Group->Font, 30);
      frame_stats *Frame = DebugState->Frames + DebugState->ReadScopeIndex;
      BufferColumn(Frame->FrameMs, 4, Group, Layout, WHITE);
      BufferThousands(Frame->TotalCycles, Group, Layout, WHITE);

      u32 TotalMutexOps = GetTotalMutexOpsForReadFrame();
      BufferThousands(TotalMutexOps, Group, Layout, WHITE);
    }
    NewLine(Layout, &Group->Font);

  END_BLOCK("Frame Ticker");

  TIMED_BLOCK("Call Graph");

    u32 TotalThreadCount = GetWorkerThreadCount() + 1;
    u64 TotalCycles = 0;
    for ( u32 ThreadIndex = 0;
        ThreadIndex < TotalThreadCount;
        ++ThreadIndex)
    {
      debug_thread_state *ThreadState = GetThreadDebugState(ThreadIndex);
      debug_scope_tree *ReadTree = ThreadState->ScopeTrees + DebugState->ReadScopeIndex;
      frame_stats *Frame = DebugState->Frames + DebugState->ReadScopeIndex;

      debug_thread_state *MainThreadState = GetThreadDebugState(0);
      debug_scope_tree *MainThreadReadTree    = MainThreadState->ScopeTrees + DebugState->ReadScopeIndex;

      if (MainThreadReadTree->FrameRecorded == ReadTree->FrameRecorded)
      {
        PadBottom(Layout, 15);
        NewLine(Layout, &Group->Font);
        BufferFirstCallToEach(Group, ReadTree->Root, ReadTree->Root, GetDebugMemoryAllocator(), Layout, Frame->TotalCycles, 0);
      }
    }

  END_BLOCK("Call Graph");
}

memory_arena_stats
GetMemoryArenaStats(memory_arena *ArenaIn)
{
  memory_arena_stats Result = {};

  memory_arena *Arena = ArenaIn;
  while (Arena)
  {
    Result.Allocations++;
    Result.Pushes += Arena->Pushes;
    Result.TotalAllocated += TotalSize(Arena);
    Result.Remaining += Remaining(Arena);

    Arena = Arena->Prev;
  }

  return Result;
}

memory_arena_stats
GetTotalMemoryArenaStats()
{
  TIMED_FUNCTION();
  memory_arena_stats TotalStats = {};
  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = &GetDebugState()->RegisteredMemoryArenas[Index];
    if (!Current->Arena) continue;

    memory_arena_stats CurrentStats = GetMemoryArenaStats(Current->Arena);
    TotalStats.Allocations          += CurrentStats.Allocations;
    TotalStats.Pushes               += CurrentStats.Pushes;
    TotalStats.TotalAllocated       += CurrentStats.TotalAllocated;
    TotalStats.Remaining            += CurrentStats.Remaining;
  }

  return TotalStats;
}

void
ColumnLeft(u32 Width, const char *Text, ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  u32 Len = (u32)strlen(Text);
  u32 Pad = Max(Width-Len, 0U);
  BufferValue(Text, Group, Layout, ColorIndex);
  AdvanceSpaces(Pad, Layout, &Group->Font);
}

void
ColumnRight(s32 Width, const char *Text, ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  s32 Len = (s32)strlen(Text);
  s32 Pad = Max(Width-Len, 0);
  AdvanceSpaces(Pad, Layout, &Group->Font);
  BufferValue(Text, Group, Layout, ColorIndex);
}

void
DebugDrawDrawCalls(ui_render_group *Group, layout *Layout)
{
  NewLine(Layout, &Group->Font);
  NewLine(Layout, &Group->Font);

  for( u32 DrawCountIndex = 0;
       DrawCountIndex < Global_DrawCallArrayLength;
       ++ DrawCountIndex)
  {
     debug_draw_call *DrawCall = &Global_DrawCalls[DrawCountIndex];
     if (DrawCall->Caller)
     {
       BufferThousands(DrawCall->Calls, Group, Layout, WHITE);
       BufferThousands(DrawCall->N, Group, Layout, WHITE);
       AdvanceSpaces(2, Layout, &Group->Font);
       BufferValue(DrawCall->Caller, Group, Layout, WHITE);
       NewLine(Layout, &Group->Font);
     }
  }

  return;
}

umm
GetAllocationSize(push_metadata *Meta)
{
  umm AllocationSize = Meta->StructSize*Meta->StructCount*Meta->PushCount;
  return AllocationSize;
}



/*******************************  Arena UI  **********************************/


inline b32
BufferBarGraph(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, layout *Layout, r32 PercFilled, v3 Color)
{
  r32 BarHeight = Group->Font.Size;
  r32 BarWidth = 200.0f;

  v2 MinP = Layout->At + Layout->Basis;
  v2 BarDim = V2(BarWidth, BarHeight);
  v2 PercBarDim = V2(BarWidth, BarHeight) * V2(PercFilled, 1);

  BufferQuad(Group, Geo, MinP, BarDim);
  BufferColors(Group, Geo, V3(0.25f));
  Geo->At+=6;

  rect2 BarRect = { MinP, MinP + BarDim };
  b32 Hovering = IsInsideRect(BarRect, Group->MouseP);

  if (Hovering)
    Color = {{ 1, 0, 1 }};

  BufferQuad(Group, Geo, MinP, PercBarDim);
  BufferColors(Group, Geo, Color);
  Geo->At+=6;

  Layout->At.x += BarDim.x;

  return Hovering;
}

inline b32
BufferArenaBargraph(table_layout *BargraphTable, ui_render_group *Group, umm TotalUsed, r32 TotalPerc, umm Remaining, v3 Color )
{
  Column( FormatMemorySize(TotalUsed), Group, BargraphTable, WHITE);
  b32 Hover = BufferBarGraph(Group, &Group->TextGroup->UIGeo, &BargraphTable->Layout, TotalPerc, Color);
  Column( FormatMemorySize(Remaining), Group, BargraphTable, WHITE);
  NewRow(BargraphTable, &Group->Font);

  b32 Click = (Hover && Group->Input->LMB.WasPressed);
  return Click;
}

v2
BufferMemoryStatsTable(memory_arena_stats MemStats, ui_render_group *Group, table_layout *StatsTable, v2 BasisP)
{
  StatsTable->Layout = {};
  StatsTable->Layout.Basis = BasisP;

  Column("Allocs", Group, StatsTable, WHITE);
  Column(FormatMemorySize(MemStats.Allocations), Group, StatsTable, WHITE);
  NewRow(StatsTable, &Group->Font);

  Column("Pushes", Group, StatsTable, WHITE);
  Column(FormatThousands(MemStats.Pushes), Group, StatsTable, WHITE);
  NewRow(StatsTable, &Group->Font);

  Column("Remaining", Group, StatsTable, WHITE);
  Column(FormatMemorySize(MemStats.Remaining), Group, StatsTable, WHITE);
  NewRow(StatsTable, &Group->Font);

  Column("Total", Group, StatsTable, WHITE);
  Column(FormatMemorySize(MemStats.TotalAllocated), Group, StatsTable, WHITE);
  NewRow(StatsTable, &Group->Font);

  return StatsTable->Layout.Clip.Max;
}

void
BufferMemoryBargraphTable(ui_render_group *Group, selected_arenas *SelectedArenas, memory_arena_stats MemStats, umm TotalUsed, memory_arena *HeadArena, table_layout *BargraphTable, v2 BasisP)
{
  BargraphTable->Layout = {};
  BargraphTable->Layout.Basis = BasisP;
  SetFontSize(&Group->Font, 22);

  NewRow(BargraphTable, &Group->Font);
  v3 DefaultColor = V3(0.5f, 0.5f, 0.0);

  r32 TotalPerc = (r32)SafeDivide0(TotalUsed, MemStats.TotalAllocated);
  b32 TobbleAllArenas = BufferArenaBargraph(BargraphTable, Group, TotalUsed, TotalPerc, MemStats.Remaining, DefaultColor);
  NewRow(BargraphTable, &Group->Font);


  memory_arena *CurrentArena = HeadArena;
  while (CurrentArena)
  {
    v3 Color = DefaultColor;
    for (u32 ArenaIndex = 0;
        ArenaIndex < SelectedArenas->Count;
        ++ArenaIndex)
    {
      selected_memory_arena *Selected = &SelectedArenas->Arenas[ArenaIndex];
      if (Selected->ArenaHash == HashArena(CurrentArena))
      {
        Color = V3(0.85f, 0.85f, 0.0f);
      }
    }

    u64 CurrentUsed = TotalSize(CurrentArena) - Remaining(CurrentArena);
    r32 CurrentPerc = (r32)SafeDivide0(CurrentUsed, TotalSize(CurrentArena));

    b32 GotClicked = BufferArenaBargraph(BargraphTable, Group, CurrentUsed, CurrentPerc, Remaining(CurrentArena), Color);

    if (TobbleAllArenas || GotClicked)
    {
      selected_memory_arena *Found = 0;
      for (u32 ArenaIndex = 0;
          ArenaIndex < SelectedArenas->Count;
          ++ArenaIndex)
      {
        selected_memory_arena *Selected = &SelectedArenas->Arenas[ArenaIndex];
        if (Selected->ArenaHash == HashArena(CurrentArena))
        {
          Found = Selected;
          break;
        }
      }
      if (Found)
      {
        *Found = SelectedArenas->Arenas[--SelectedArenas->Count];
      }
      else
      {
        selected_memory_arena *Selected = &SelectedArenas->Arenas[SelectedArenas->Count++];
        Selected->ArenaHash = HashArena(CurrentArena);
        Selected->HeadArenaHash = HashArenaHead(CurrentArena);
      }

    }

    CurrentArena = CurrentArena->Prev;
  }

  return;
}

layout *
BufferDebugPushMetaData(debug_state *DebugState, ui_render_group *Group, selected_arenas *SelectedArenas, umm CurrentArenaHead, table_layout *Table, v2 Basis)
{
  push_metadata CollatedMetaTable[META_TABLE_SIZE] = {};

  layout *Layout = &Table->Layout;
  Clear(Layout);
  Layout->Basis = Basis;
  BeginClipRect(Layout);

  SetFontSize(&Group->Font, 24);


  Column("Size", Group, Table, WHITE);
  Column("Structs", Group, Table, WHITE);
  Column("Push Count", Group, Table, WHITE);
  Column("Name", Group, Table, WHITE);
  NewLine(Layout, &Group->Font);


  // Pick out relevant metadata and write to collation table
  u32 TotalThreadCount = GetWorkerThreadCount() + 1;


  for ( u32 ThreadIndex = 0;
      ThreadIndex < TotalThreadCount;
      ++ThreadIndex)
  {
    for ( u32 MetaIndex = 0;
        MetaIndex < META_TABLE_SIZE;
        ++MetaIndex)
    {
      push_metadata *Meta = &GetDebugState()->ThreadStates[ThreadIndex].MetaTable[MetaIndex];

      for (u32 ArenaIndex = 0;
          ArenaIndex < SelectedArenas->Count;
          ++ArenaIndex)
      {
        selected_memory_arena *Selected = &SelectedArenas->Arenas[ArenaIndex];
        if (Meta->HeadArenaHash == CurrentArenaHead &&
            Meta->ArenaHash == Selected->ArenaHash )
        {
          CollateMetadata(Meta, CollatedMetaTable);
        }
      }
    }
  }

  // Densely pack collated records
  u32 PackedRecords = 0;
  for ( u32 MetaIndex = 0;
      MetaIndex < META_TABLE_SIZE;
      ++MetaIndex)
  {
    push_metadata *Record = &CollatedMetaTable[MetaIndex];
    if (Record->Name)
    {
      CollatedMetaTable[PackedRecords++] = *Record;
    }
  }

  // Sort collation table
  for ( u32 MetaIndex = 0;
      MetaIndex < PackedRecords;
      ++MetaIndex)
  {
    push_metadata *SortValue = &CollatedMetaTable[MetaIndex];
    for ( u32 TestMetaIndex = 0;
        TestMetaIndex < PackedRecords;
        ++TestMetaIndex)
    {
      push_metadata *TestValue = &CollatedMetaTable[TestMetaIndex];

      if ( GetAllocationSize(SortValue) > GetAllocationSize(TestValue) )
      {
        push_metadata Temp = *SortValue;
        *SortValue = *TestValue;
        *TestValue = Temp;
      }
    }
  }


  // Buffer collation table text
  for ( u32 MetaIndex = 0;
      MetaIndex < PackedRecords;
      ++MetaIndex)
  {
    push_metadata *Collated = &CollatedMetaTable[MetaIndex];
    if (Collated->Name)
    {
      umm AllocationSize = GetAllocationSize(Collated);
      Column( FormatMemorySize(AllocationSize), Group, Table, WHITE);
      Column( FormatThousands(Collated->StructCount), Group, Table, WHITE);
      Column( FormatThousands(Collated->PushCount), Group, Table, WHITE);
      Column(Collated->Name, Group, Table, WHITE);
      NewLine(Layout, &Group->Font);
    }

    continue;
  }


  NewLine(Layout, &Group->Font);
  EndClipRect(Group, Layout, &Group->TextGroup->UIGeo);

  return Layout;
}

void
DebugDrawMemoryHud(ui_render_group *Group, debug_state *DebugState, v2 OriginalBasisP)
{
  local_persist table_layout MemoryHudArenaTable = {};

  MemoryHudArenaTable.Layout.At = {};
  MemoryHudArenaTable.Layout.Basis = OriginalBasisP;

  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = &DebugState->RegisteredMemoryArenas[Index];
    if (!Current->Arena) continue;

    memory_arena_stats MemStats = GetMemoryArenaStats(Current->Arena);
    u64 TotalUsed = MemStats.TotalAllocated - MemStats.Remaining;

    {
      SetFontSize(&Group->Font, 36);
      NewLine(&MemoryHudArenaTable.Layout, &Group->Font);
      u8 Color = HoverAndClickExpand(Group, &MemoryHudArenaTable.Layout, Current, WHITE, TEAL);

      Column(Current->Name, Group, &MemoryHudArenaTable, Color);
      Column(MemorySize(MemStats.TotalAllocated), Group, &MemoryHudArenaTable, Color);
      NewRow(&MemoryHudArenaTable, &Group->Font);
    }


    if (Current->Expanded)
    {
      SetFontSize(&Group->Font, 28);

      table_layout *StatsTable    = &Current->StatsTable;
      table_layout *BargraphTable = &Current->BargraphTable;
      table_layout *MetaTable     = &Current->MetadataTable;

      {
        v2 BasisP = GetAbsoluteAt(&MemoryHudArenaTable.Layout);
        BufferMemoryStatsTable(MemStats, Group, StatsTable, BasisP);
      }

      selected_arenas *SelectedArenas = DebugState->SelectedArenas;
      {
        v2 BasisP = { GetAbsoluteMin(&StatsTable->Layout).x,
                      GetAbsoluteMax(&StatsTable->Layout).y };
        BufferMemoryBargraphTable(Group, SelectedArenas, MemStats, TotalUsed, Current->Arena, BargraphTable, BasisP);
      }


      {
        v2 BasisP = { 100.0f + Max(StatsTable->Layout.Clip.Max.x,
                                   BargraphTable->Layout.Clip.Max.x),
                      GetAbsoluteAt(&MemoryHudArenaTable.Layout).y };

        BufferDebugPushMetaData(DebugState, Group, SelectedArenas, HashArenaHead(Current->Arena), MetaTable, BasisP);
      }

      MemoryHudArenaTable.Layout.At = {};
      MemoryHudArenaTable.Layout.Basis = V2( MemoryHudArenaTable.Layout.Basis.x,
                                             Max( GetAbsoluteMax(&BargraphTable->Layout).y,
                                                  GetAbsoluteMax(&MetaTable->Layout).y ));

      AdvanceClip(&MemoryHudArenaTable.Layout);
    }

    continue;
  }


  return;
}


/*******************************  Network UI  ********************************/


void
DebugDrawNetworkHud(ui_render_group *Group,
    network_connection *Network,
    server_state *ServerState,
    layout *Layout)
{

  BufferValue("Network", Group, Layout, WHITE);
  AdvanceSpaces(2, Layout, &Group->Font);

  if (IsConnected(Network))
  {
    BufferValue("O", Group, Layout, GREEN);

    AdvanceSpaces(2, Layout, &Group->Font);

    if (Network->Client)
    {
      BufferValue("ClientId", Group, Layout, WHITE);
      BufferColumn( Network->Client->Id, 2, Group, Layout, WHITE);
    }

    NewLine(Layout, &Group->Font);
    NewLine(Layout, &Group->Font);

    NewLine(Layout, &Group->Font);

    for (s32 ClientIndex = 0;
        ClientIndex < MAX_CLIENTS;
        ++ClientIndex)
    {
      client_state *Client = &ServerState->Clients[ClientIndex];

      u32 Color = WHITE;

      if (Network->Client->Id == ClientIndex)
        Color = GREEN;

      AdvanceSpaces(1, Layout, &Group->Font);
      BufferValue("Id:", Group, Layout, WHITE);
      BufferColumn( Client->Id, 2, Group, Layout, WHITE);
      AdvanceSpaces(2, Layout, &Group->Font);
      BufferColumn(Client->Counter, 7, Group, Layout, Color);
      NewLine(Layout, &Group->Font);
    }

  }
  else
  {
    BufferValue("X", Group, Layout, RED);
    NewLine(Layout, &Group->Font);
  }

  return;
}


/******************************  Graphics UI  ********************************/


void
DebugDrawGraphicsHud(ui_render_group *Group, debug_state *DebugState, layout *Layout)
{
  BufferValue("Graphics", Group, Layout, WHITE);

  NewLine(Layout, &Group->Font);
  NewLine(Layout, &Group->Font);

  BufferMemorySize(DebugState->BytesBufferedToCard, Group, Layout, WHITE);

  return;
}


/****************************  Frame Start/End  ******************************/


void
DebugFrameEnd(platform *Plat, game_state *GameState)
{
  TIMED_FUNCTION();

  debug_state *DebugState              = GetDebugState();
  debug_text_render_group *RG          = &DebugState->TextRenderGroup;
  textured_2d_geometry_buffer *TextGeo = &RG->TextGeo;

  layout Layout         = {};
  ui_render_group Group = {};
  min_max_avg_dt Dt     = {};

  Group.TextGroup       = RG;
  Group.Input           = &Plat->Input;
  Group.ScreenDim       = V2(Plat->WindowWidth, Plat->WindowHeight);
  Group.MouseP          = V2(Plat->MouseP.x, Plat->MouseP.y);

  SetFontSize(&Group.Font, DEBUG_FONT_SIZE);


  TIMED_BLOCK("Draw Status Bar");
    Dt = ComputeMinMaxAvgDt();
    BufferColumn(Dt.Max, 6, &Group, &Layout, WHITE);
    NewLine(&Layout, &Group.Font);

    BufferColumn(Dt.Avg, 6, &Group, &Layout, WHITE);
    BufferColumn(Plat->dt*1000.0f, 6, &Group, &Layout, WHITE);
    BufferValue("ms", &Group, &Layout, WHITE);

    {
      // Main line
      memory_arena_stats TotalStats = GetTotalMemoryArenaStats();

      BufferThousands(TotalStats.Allocations, &Group, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &Group.Font);
      BufferValue("Allocations", &Group, &Layout, WHITE);

      BufferThousands(TotalStats.Pushes, &Group, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &Group.Font);
      BufferValue("Pushes", &Group, &Layout, WHITE);

      u32 TotalDrawCalls = 0;

      for( u32 DrawCountIndex = 0;
           DrawCountIndex < Global_DrawCallArrayLength;
           ++ DrawCountIndex)
      {
        debug_draw_call *Call = &Global_DrawCalls[DrawCountIndex];
        if (Call->Caller)
        {
          TotalDrawCalls += Call->Calls;
        }
      }

      BufferColumn(TotalDrawCalls, 6, &Group, &Layout, WHITE);
      AdvanceSpaces(1, &Layout, &Group.Font);
      BufferValue("Draw Calls", &Group, &Layout, WHITE);

      NewLine(&Layout, &Group.Font);
    }

    BufferColumn(Dt.Min, 6, &Group, &Layout, WHITE);
  END_BLOCK("Status Bar");

  SetFontSize(&Group.Font, 32);
  NewLine(&Layout, &Group.Font);
  NewLine(&Layout, &Group.Font);

  switch (DebugState->UIType)
  {
    case DebugUIType_None:
    {
    } break;

    case DebugUIType_Graphics:
    {
      DebugDrawGraphicsHud(&Group, DebugState, &Layout);
    } break;

    case DebugUIType_Network:
    {
      DebugDrawNetworkHud(&Group, &Plat->Network, GameState->ServerState, &Layout);
    } break;

    case DebugUIType_CallGraph:
    {
      BufferValue("Call Graphs", &Group, &Layout, WHITE);
      DebugDrawCallGraph(&Group, DebugState, &Layout, Dt.Max);
      DebugDrawCycleThreadGraph(&Group, DebugState, &Layout);
    } break;

    case DebugUIType_Memory:
    {
      BufferValue("Memory Arenas", &Group, &Layout, WHITE);
      NewLine(&Layout, &Group.Font);
      v2 BasisP = Layout.At;
      DebugDrawMemoryHud(&Group, DebugState, BasisP);
    } break;

    case DebugUIType_DrawCalls:
    {
      BufferValue("Draw  Calls", &Group, &Layout, WHITE);
      DebugDrawDrawCalls(&Group, &Layout);
    } break;

    InvalidDefaultCase;
  }

  FlushBuffer(RG, &RG->UIGeo, Group.ScreenDim);
  FlushBuffer(RG, TextGeo, Group.ScreenDim);

  DebugState->BytesBufferedToCard = 0;

  for( u32 DrawCountIndex = 0;
       DrawCountIndex < Global_DrawCallArrayLength;
       ++ DrawCountIndex)
  {
     Global_DrawCalls[DrawCountIndex] = NullDrawCall;
  }

  return;
}

void
DebugFrameBegin(hotkeys *Hotkeys, r32 Dt)
{
  debug_state *State = GetDebugState();

  if ( Hotkeys->Debug_RedrawEveryPush )
  {
    State->Debug_RedrawEveryPush = !State->Debug_RedrawEveryPush;
  }

  if ( Hotkeys->Debug_ToggleGlobalDebugBreak )
  {
    Global_TriggerRuntimeBreak = !Global_TriggerRuntimeBreak;
  }

  if ( Hotkeys->Debug_ToggleProfile )
  {
    Hotkeys->Debug_ToggleProfile = False;
    State->DebugDoScopeProfiling = !State->DebugDoScopeProfiling;
  }

  if ( Hotkeys->Debug_NextUiState )
  {
    Hotkeys->Debug_NextUiState = False;
    State->UIType = (debug_ui_type)(((s32)State->UIType + 1) % (s32)DebugUIType_Count);
  }


  return;
}

#endif // DEBUG


