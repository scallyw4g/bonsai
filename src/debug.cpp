#if BONSAI_INTERNAL

#include <stdio.h>

texture *
MakeTexture_RGBA( v2i Dim, const void* Data, memory_arena *Memory);

void
FramebufferTexture(framebuffer *FBO, texture *Tex);

shader
MakeSimpleTextureShader(texture *Texture, memory_arena *GraphicsMemory);

b32
CheckAndClearFramebuffer();

b32
InitDebugOverlayFramebuffer(debug_text_render_group *RG, memory_arena *DebugArena, const char *DebugFont)
{
  GL_Global->glGenFramebuffers(1, &RG->FBO.ID);
  GL_Global->glBindFramebuffer(GL_FRAMEBUFFER, RG->FBO.ID);

  v2i ScreenDim = V2i(SCR_WIDTH, SCR_HEIGHT);

  RG->FontTexture = LoadDDS(DebugFont);
  RG->CompositedTexture = MakeTexture_RGBA( ScreenDim, 0, DebugArena);

  FramebufferTexture(&RG->FBO, RG->CompositedTexture);

  GL_Global->glGenBuffers(1, &RG->VertexBuffer);
  GL_Global->glGenBuffers(1, &RG->UVBuffer);

  RG->Text2DShader = LoadShaders("TextVertexShader.vertexshader",
                                 "TextVertexShader.fragmentshader", DebugArena);

  RG->TextureUniformID = GL_Global->glGetUniformLocation(RG->Text2DShader.ID, "myTextureSampler");

  RG->DebugTextureShader = MakeSimpleTextureShader(RG->CompositedTexture, DebugArena);

  if (!CheckAndClearFramebuffer())
    return False;

  return True;
}

void
AllocateAndInitGeoBuffer(text_geometry_buffer *Geo, u32 VertCount, memory_arena *DebugArena)
{
  Geo->Verts = PUSH_STRUCT_CHECKED(v3, DebugArena, VertCount);
  Geo->UVs = PUSH_STRUCT_CHECKED(v2, DebugArena, VertCount);
  Geo->Allocated = VertCount;
}

void
InitScopeTree(debug_state *State)
{
  State->NumScopes = 0;
  State->CurrentScope = 0;
  State->WriteScope = &State->RootScopes[State->RootScopeIndex];

  return;
}

void
InitDebugState(platform *Plat)
{
  GlobalDebugState = &Plat->DebugState;
  GlobalDebugState->GetCycleCount = Plat->GetCycleCount;

  InitScopeTree(GlobalDebugState);

  GlobalDebugState->FreeScopeSentinel.Parent = &GlobalDebugState->FreeScopeSentinel;
  GlobalDebugState->FreeScopeSentinel.Child = &GlobalDebugState->FreeScopeSentinel;

  GlobalDebugState->Memory = SubArena(Plat->Memory, Megabytes(32));

  GlobalDebugState->TextRenderGroup = PUSH_STRUCT_CHECKED(debug_text_render_group, Plat->Memory, 1);
  if (!InitDebugOverlayFramebuffer(GlobalDebugState->TextRenderGroup, Plat->Memory, "Holstein.DDS"))
  { Error("Initializing Debug Overlay Framebuffer"); }

  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup->TextGeo, 4096, Plat->Memory);

  GlobalDebugState->Initialized = True;
  return;
}

void
DrawDebugText(debug_text_render_group *RG, text_geometry_buffer *Geo, v2 ViewportDim)
{
  u32 VertCount = Geo->CurrentIndex +1;
  Geo->CurrentIndex = 0;

  GL_Global->glBindFramebuffer(GL_FRAMEBUFFER, 0);

  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->VertexBuffer);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(v3), Geo->Verts, GL_STATIC_DRAW);

  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->UVBuffer);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(v2), Geo->UVs, GL_STATIC_DRAW);

  // Bind Text shader
  GL_Global->glUseProgram(RG->Text2DShader.ID);

  // Bind Font texture
  GL_Global->glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RG->FontTexture.ID);
  GL_Global->glUniform1i(RG->TextureUniformID, 0);

  // 1rst attribute buffer : Verteces
  GL_Global->glEnableVertexAttribArray(0);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->VertexBuffer);
  GL_Global->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  // 2nd attribute buffer : UVs
  GL_Global->glEnableVertexAttribArray(1);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->UVBuffer);
  GL_Global->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_DEPTH_TEST);

  // Draw
  SetViewport(ViewportDim);
  glDrawArrays(GL_TRIANGLES, 0, VertCount);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  GL_Global->glDisableVertexAttribArray(0);
  GL_Global->glDisableVertexAttribArray(1);

  AssertNoGlErrors;
}

rect2
BufferTextAt(debug_text_render_group *RG, text_geometry_buffer *Geo,
    const char *Text, v2 XY, s32 FontSize, v2 ViewportDim)
{
  s32 QuadCount = strlen(Text);

  rect2 Result = { XY, XY };

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    if (Geo->CurrentIndex + 6 > Geo->Allocated)
      DrawDebugText(RG, Geo, ViewportDim);

    v3 vertex_up_left    = V3( (r32)(XY.x+CharIndex*FontSize)         , (r32)(XY.y+FontSize), 0.5f);
    v3 vertex_up_right   = V3( (r32)(XY.x+CharIndex*FontSize+FontSize), (r32)(XY.y+FontSize), 0.5f);
    v3 vertex_down_right = V3( (r32)(XY.x+CharIndex*FontSize+FontSize), (r32)XY.y           , 0.5f);
    v3 vertex_down_left  = V3( (r32)(XY.x+CharIndex*FontSize)         , (r32)XY.y           , 0.5f);

    Result.Max = vertex_up_right.xy;

    char character = Text[CharIndex];
    float uv_x = (character%16)/16.0f;
    float uv_y = (character/16)/16.0f;

    v2 uv_up_left    = V2( uv_x           , uv_y );
    v2 uv_up_right   = V2( uv_x+1.0f/16.0f, uv_y );
    v2 uv_down_right = V2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
    v2 uv_down_left  = V2( uv_x           , (uv_y + 1.0f/16.0f) );


    v3 XYClip = (1.0f / V3(SCR_WIDTH, SCR_HEIGHT, 1));

    Geo->Verts[Geo->CurrentIndex] = (vertex_up_left * XYClip) * 2.0f - 1;
    Geo->UVs[Geo->CurrentIndex++] = uv_up_left;

    Geo->Verts[Geo->CurrentIndex] = (vertex_down_left * XYClip) * 2.0f - 1;
    Geo->UVs[Geo->CurrentIndex++] = uv_down_left;

    Geo->Verts[Geo->CurrentIndex] = (vertex_up_right * XYClip) * 2.0f - 1;
    Geo->UVs[Geo->CurrentIndex++] = uv_up_right;


    Geo->Verts[Geo->CurrentIndex] = (vertex_down_right * XYClip) * 2.0f - 1;
    Geo->UVs[Geo->CurrentIndex++] = uv_down_right;

    Geo->Verts[Geo->CurrentIndex] = (vertex_up_right * XYClip) * 2.0f - 1;
    Geo->UVs[Geo->CurrentIndex++] = uv_up_right;

    Geo->Verts[Geo->CurrentIndex] = (vertex_down_left * XYClip) * 2.0f - 1;
    Geo->UVs[Geo->CurrentIndex++] = uv_down_left;

    continue;
  }

  return Result;
}

debug_global u64 LastFrameCycleCount = 0;
#if 0
inline r32
CalculateFramePercentage(debug_profile_entry *Entry, u64 CycleDelta)
{
  u64 TotalCycles = Entry->CycleCount;
  r32 FramePerc = (r32)((r64)TotalCycles/(r64)CycleDelta)*100;

  return FramePerc;
}
#endif

void
FreeScopes(debug_state *DebugState, debug_profile_scope *ScopeToFree)
{
  if (!ScopeToFree) return;

  FreeScopes(DebugState, ScopeToFree->Child);
  FreeScopes(DebugState, ScopeToFree->Sibling);

  ScopeToFree->Child = 0;
  ScopeToFree->Sibling = 0;
  ScopeToFree->Parent = 0;

  debug_profile_scope *First = DebugState->FreeScopeSentinel.Child;
  debug_profile_scope *Sentinel = &DebugState->FreeScopeSentinel;

  Sentinel->Child = ScopeToFree;
  First->Parent = ScopeToFree;

  ScopeToFree->Parent = Sentinel;
  ScopeToFree->Child = First;

  return;
}

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

void
FreeNextScopeTree(void)
{
  debug_state *State = GetDebugState();

  /* Debug("Scopes Recorded: %lu", State->NumScopes); */
  /* PrintScopeTree(&State->RootScope); */
  /* Debug("------------------------------------------------------------------------------"); */

  State->RootScopeIndex = (State->RootScopeIndex+1) % ROOT_SCOPE_COUNT;
  debug_profile_scope *NextRootScope = State->GetRootScope();
  FreeScopes(State, NextRootScope);

  InitScopeTree(State);

  /* PrintFreeScopes(State); */
  /* Debug("------------------------------------------------------------------------------"); */
  /* PrintScopeTree(State->RootScope); */

  /* debug_profile_scope RootScope; */
  /* debug_profile_scope FreeScopeSentinel; */

  /* Debug("------------------------------------------------------------------------------"); */

}

inline void
BufferText(const char *Text, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  rect2 TextBox = BufferTextAt(RG, &RG->TextGeo, Text, V2(Layout->AtX, Layout->AtY), Layout->FontSize, ViewportDim);
  Layout->AtX = TextBox.Max.x;

  return;
}

inline void
BufferText(r32 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%f", Number);
  BufferText( Buffer, Layout, RG, ViewportDim);
  return;
}

inline void
BufferText(u64 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Number);
  BufferText( Buffer, Layout, RG, ViewportDim);
  return;
}

inline void
BufferText(u32 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Number);
  BufferText( Buffer, Layout, RG, ViewportDim);
  return;
}

inline void
NewLine(layout *Layout)
{
  Layout->AtY -= (Layout->FontSize * 1.3f);
  Layout->AtX = 0;
  return;
}

inline void
AdvanceSpaces(u32 N, layout *Layout)
{
  Layout->AtX += (N*Layout->FontSize);
  return;
}

inline void
BufferCycles(u64 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  u64 OneThousand = 1000;
  r32 Display = (r32)Number;
  char Units = ' ';

  if (Number >= OneThousand)
  {
    Display = Number / (r32)OneThousand;
    Units = 'K';
  }

  char Buffer[32];
  sprintf(Buffer, "%.1f%c", Display, Units);
  {
    s32 Len = strlen(Buffer);
    s32 ColumnWidth = 10;
    s32 Pad = Max(ColumnWidth-Len, 0);
    AdvanceSpaces(Pad, Layout);
  }
  BufferText( Buffer, Layout, RG, ViewportDim);

  return;
}

inline void
BufferSingleDecimal(r32 Perc, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%.1f", Perc);
  {
    s32 Max = 6;
    s32 Len = strlen(Buffer);
    s32 Pad = Max-Len;
    AdvanceSpaces(Pad, Layout);
  }
  BufferText( Buffer, Layout, RG, ViewportDim);
  return;
}

inline void
BufferNumberAsText(r32 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  Layout->AtX += Layout->FontSize;
  BufferText(Number, Layout, RG, ViewportDim);
  Layout->AtX += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(r64 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  Layout->AtX += Layout->FontSize;
  BufferText((r32)Number, Layout, RG, ViewportDim);
  Layout->AtX += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(u64 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  Layout->AtX += Layout->FontSize;
  BufferText(Number, Layout, RG, ViewportDim);
  Layout->AtX += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(u32 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim)
{
  Layout->AtX += Layout->FontSize;
  BufferText(Number, Layout, RG, ViewportDim);
  Layout->AtX += Layout->FontSize;
  return;
}

b32 StringsMatch(const char *S1, const char *S2)
{
  b32 Result = strcmp(S1, S2) == 0;
  return Result;
}

void
BufferScopeTree(debug_profile_scope *Scope, debug_state *State, layout *Layout, v2 ViewportDim, u32 Depth = 0)
{
  if (!Scope)
    return;

  b32 WeAreFirst = True;
  u64 CallCount = 0;
  u64 TotalCycles = 0;

  debug_profile_scope *Next = State->GetRootScope();
  if (Scope->Parent) Next = Scope->Parent->Child;

  while (Next)
  {
    if (Next == Scope) // Find Ourselves
    {
      if (CallCount == 0) // We're first
      {
        // Count duplicates
      }
      else
      {
        WeAreFirst = False;
        break; // We're not first, descend to children/siblings
      }
    }

    if (StringsMatch(Next->Name, Scope->Name))
    {
      ++CallCount;
      TotalCycles += Next->CycleCount;
    }

    Next = Next->Sibling;
  }

  if (WeAreFirst)
  {
    r32 Percentage = 100.0*(r32)TotalCycles/(r32)FrameElapsedCycles;
    u64 AvgCycles = SafeDivide0(TotalCycles, CallCount);
    BufferSingleDecimal(Percentage, Layout, State->TextRenderGroup, ViewportDim);
    BufferCycles(AvgCycles, Layout, State->TextRenderGroup, ViewportDim);
    Layout->AtX += (Depth*2.0f*Layout->FontSize);
    BufferNumberAsText(CallCount, Layout, State->TextRenderGroup, ViewportDim);
    BufferText(Scope->Name, Layout, State->TextRenderGroup, ViewportDim);
    NewLine(Layout);
  }

  if (WeAreFirst)
    BufferScopeTree(Scope->Child, State, Layout, ViewportDim, Depth+1);

  BufferScopeTree(Scope->Sibling, State, Layout, ViewportDim, Depth);

  return;
}

void
DebugFrameEnd(platform *Plat)
{
  debug_state *DebugState = GetDebugState();
  debug_text_render_group *RG = DebugState->TextRenderGroup;
  text_geometry_buffer *TextGeo = &RG->TextGeo;
  r32 dt = Plat->dt;

  static const u32 DtBufferSize = 60;
  static r32 DtBuffer[DtBufferSize] = {};
  static u32 DtBufferIndex = 0;

  DtBuffer[DtBufferIndex] = dt;
  DtBufferIndex = (++DtBufferIndex) % DtBufferSize;

  r32 Accum = 0;
  r32 MinDt = DtBuffer[0];
  r32 MaxDt = DtBuffer[0];
  for ( u32 Index = 0;
        Index < DtBufferSize;
        ++Index)
  {
    r32 Val = DtBuffer[Index];
    Accum += Val;
    MinDt = Min(MinDt, Val);
    MaxDt = Max(MaxDt, Val);
  }

  r32 AverageFrameDt = Accum/(r32)DtBufferSize;


  v2 ViewportDim = V2(Plat->WindowWidth, Plat->WindowHeight);

  /* PrintScopeTree(DebugState->RootScope); */
  /* Log("-------------------------------------------------------------------------------"); */

  layout StatusBarLayout(DEBUG_FONT_SIZE);
  StatusBarLayout.AtY = (r32)SCR_HEIGHT - StatusBarLayout.FontSize;
  {

    AdvanceSpaces(6, &StatusBarLayout);
    BufferSingleDecimal(1000.0*MaxDt, &StatusBarLayout, RG, ViewportDim);
    NewLine(&StatusBarLayout);

    BufferSingleDecimal(1000.0*dt, &StatusBarLayout, RG, ViewportDim);
    BufferSingleDecimal(1000.0*AverageFrameDt, &StatusBarLayout, RG, ViewportDim);
    BufferText("ms", &StatusBarLayout, RG, ViewportDim);

    BufferCycles(FrameElapsedCycles, &StatusBarLayout, RG, ViewportDim);
    NewLine(&StatusBarLayout);

    AdvanceSpaces(6, &StatusBarLayout);
    BufferSingleDecimal(1000.0*MinDt, &StatusBarLayout, RG, ViewportDim);
  }

  {
    layout Layout = StatusBarLayout;
    NewLine(&Layout);
    NewLine(&Layout);
    for (u32 ScopeIndex = 0;
        ScopeIndex < ROOT_SCOPE_COUNT;
        ++ScopeIndex )
    {
      if (ScopeIndex == DebugState->RootScopeIndex)
      {
        BufferSingleDecimal(1000.0*dt, &Layout, RG, ViewportDim);
      }
      else
      {
        BufferText("O", &Layout, RG, ViewportDim);
      }
    }

    NewLine(&Layout);
    NewLine(&Layout);
    Layout.FontSize = 22;
    BufferScopeTree(DebugState->GetRootScope(), DebugState, &Layout, ViewportDim);
    FreeNextScopeTree();
  }


#if 0
  u64 CurrentFrameCycleCount = DebugState->GetCycleCount();
  u64 CycleDelta = CurrentFrameCycleCount - LastFrameCycleCount;
  LastFrameCycleCount = CurrentFrameCycleCount;

  debug_global u64 MaxCycleCount = CycleDelta;
  debug_global u64 MinCycleCount = CycleDelta;

  MaxCycleCount = Max(CycleDelta, MaxCycleCount);
  MinCycleCount = Min(CycleDelta, MinCycleCount);

  s32 LinePadding = 3;

  debug_global r32 MaxX = 0;

  debug_profile_entry SortedEntries[DEBUG_STATE_ENTRY_COUNT];

  MemCopy((u8*)DebugState->Entries,
          (u8*)&SortedEntries,
          sizeof(debug_profile_entry)*DEBUG_STATE_ENTRY_COUNT);

  for (s32 EntryIndex = 0;
      EntryIndex < DEBUG_STATE_ENTRY_COUNT;
      ++EntryIndex)
  {
    debug_profile_entry *Entry = &SortedEntries[EntryIndex];

    for (s32 InnerEntryIndex = 0;
        InnerEntryIndex < DEBUG_STATE_ENTRY_COUNT;
        ++InnerEntryIndex)
    {
      debug_profile_entry *EntryInner = &SortedEntries[InnerEntryIndex];

      if (EntryInner->MaxPerc > Entry->MaxPerc)
      {
        debug_profile_entry Temp = *EntryInner;
        *EntryInner = *Entry;
        *Entry = Temp;
      }
    }
  }

  s32 AtY = 0;

  {
    for (s32 EntryIndex = 0;
        EntryIndex < DEBUG_STATE_ENTRY_COUNT;
        ++EntryIndex)
    {
      debug_profile_entry *Entry = &SortedEntries[EntryIndex];

      if (Entry->HitCount > 0)
      {
        /* char CycleCountBuffer[32]; */
        /* sprintf(CycleCountBuffer, "%" PRIu64, Entry->CycleCount); */

        /* rect2 CCRect = BufferTextAt(Plat, RG, TextGeo, CycleCountBuffer, 0, AtY, FontSize); */
        /* MaxX = max(MaxX, CCRect.Max.x); */
        /* AtY += (FontSize + LinePadding); */
      }
    }


    {
      char CycleCountBuffer[32] = {};
      sprintf(CycleCountBuffer, "%" PRIu64, MinCycleCount);
      BufferTextAt(Plat, RG, TextGeo, CycleCountBuffer, V2(0, AtY), FontSize);
      AtY += (FontSize + LinePadding);
      AtY += (FontSize + LinePadding);
    }

    {
      char CycleCountBuffer[32] = {};
      sprintf(CycleCountBuffer, "%" PRIu64, CycleDelta);
      BufferTextAt(Plat, RG, TextGeo, CycleCountBuffer, V2(0, AtY), FontSize);
      AtY += (FontSize + LinePadding);
      AtY += (FontSize + LinePadding);
    }

    {
      char CycleCountBuffer[32] = {};
      sprintf(CycleCountBuffer, "%" PRIu64, MaxCycleCount);
      BufferTextAt(Plat, RG, TextGeo, CycleCountBuffer, V2(0, AtY), FontSize);
      AtY += (FontSize + LinePadding);
      AtY += (FontSize + LinePadding);
    }

  }

  {
    debug_global s32 HitCountX = 0;

    for (s32 EntryIndex = 0;
        EntryIndex < DEBUG_STATE_ENTRY_COUNT;
        ++EntryIndex)
    {
      debug_profile_entry *Entry = &SortedEntries[EntryIndex];
      if (Entry->FuncName)
      {
        s32 AtX = 0;

        char PercentageBuffer[32] = {};

        r32 FramePerc = CalculateFramePercentage(Entry, CycleDelta);
        sprintf(PercentageBuffer, "%.0f", FramePerc);
        BufferTextAt(Plat, RG, TextGeo, PercentageBuffer, V2(AtX, AtY), FontSize);
        AtX += (FontSize*4);

        sprintf(PercentageBuffer, "%.0f", Entry->MaxPerc);
        BufferTextAt(Plat, RG, TextGeo, PercentageBuffer, V2(AtX, AtY), FontSize);
        AtX += (FontSize*4);

        sprintf(PercentageBuffer, "%.0f", Entry->MinPerc);
        BufferTextAt(Plat, RG, TextGeo, PercentageBuffer, V2(AtX, AtY), FontSize);
        AtX += (FontSize*4);

        /* // Print Hit Count */
        /* char CountBuffer[32]; */
        /* sprintf(CountBuffer, "%" PRIu32, Entry->HitCount); */
        /* rect2 HitCountRect = BufferTextAt(Plat,  RG, CountBuffer, AtX, AtY, FontSize); */
        /* HitCountX = max((s32)HitCountRect.Max.x, HitCountX); */

        BufferTextAt(Plat, RG, TextGeo, Entry->FuncName, V2(AtX, AtY), FontSize);

        AtY += (FontSize + LinePadding);
      }
    }
  }


  // Reset for next frame
  for (s32 EntryIndex = 0;
      EntryIndex < DEBUG_STATE_ENTRY_COUNT;
      ++EntryIndex)
  {
    debug_profile_entry *Entry = &DebugState->Entries[EntryIndex];

    r32 FramePerc = CalculateFramePercentage(Entry, CycleDelta);
    Entry->MaxPerc = Max(Entry->MaxPerc, FramePerc);
    Entry->MinPerc = Min(Entry->MinPerc, FramePerc);

    Entry->HitCount = 0;
    Entry->CycleCount = 0;
  }
#endif

  DrawDebugText(RG, TextGeo, ViewportDim);

  return;
}

void
CleanupText2D(debug_text_render_group *RG)
{
  // Delete buffers
  GL_Global->glDeleteBuffers(1, &RG->VertexBuffer);
  GL_Global->glDeleteBuffers(1, &RG->UVBuffer);

  // Delete texture
  glDeleteTextures(1, &RG->FontTexture.ID);

  // Delete shader
  GL_Global->glDeleteProgram(RG->Text2DShader.ID);

  return;
}

inline void
DoDebugFrameRecord(
    debug_recording_state *State,
    hotkeys *Hotkeys,
    memory_arena *MainMemory)
{
  {
    global_variable b32 Toggled = False;
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
          Log("Recording");
          CopyArena(MainMemory, &State->RecordedMainMemory);
        } break;

        case RecordingMode_Playback:
        {
          Log("Playback");
          CopyArena(&State->RecordedMainMemory, MainMemory);
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
      Assert(State->FramesRecorded < DEBUG_RECORD_INPUT_SIZE);
      Hotkeys->Debug_ToggleLoopedGamePlayback = False;
      State->Inputs[State->FramesRecorded++] = *Hotkeys;
    } break;

    case RecordingMode_Playback:
    {
      *Hotkeys = State->Inputs[State->FramesPlayedBack++];

      if (State->FramesPlayedBack == State->FramesRecorded)
      {
        State->FramesPlayedBack = 0;
        CopyArena(&State->RecordedMainMemory, MainMemory);
      }

    } break;

    InvalidDefaultCase;
  }

  return;
}


#endif // DEBUG
