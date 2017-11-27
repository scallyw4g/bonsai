#if DEBUG

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
InitDebugState(platform *Plat)
{
  debug_state *DebugState = GetDebugState();
  DebugState->GetCycleCount = Plat->GetCycleCount;

  DebugState->WriteScope = &DebugState->RootScope.Child;
  DebugState->CurrentScope = &DebugState->RootScope;

  DebugState->Memory = SubArena(Plat->Memory, Megabytes(16));

  DebugState->TextRenderGroup = PUSH_STRUCT_CHECKED(debug_text_render_group, Plat->Memory, 1);
  if (!InitDebugOverlayFramebuffer(DebugState->TextRenderGroup, Plat->Memory, "Holstein.DDS"))
  { Error("Initializing Debug Overlay Framebuffer"); }

  AllocateAndInitGeoBuffer(&DebugState->TextRenderGroup->TextGeo, 32, Plat->Memory);

  return;
}

void
DrawDebugText(debug_text_render_group *RG, text_geometry_buffer *Geo, platform *Plat)
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
  SetViewport(V2(Plat->WindowWidth, Plat->WindowHeight));
  glDrawArrays(GL_TRIANGLES, 0, VertCount);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  GL_Global->glDisableVertexAttribArray(0);
  GL_Global->glDisableVertexAttribArray(1);

  AssertNoGlErrors;
}

rect2
TextOutAt(platform *Plat, debug_text_render_group *RG, text_geometry_buffer *Geo, const char *Text, v2 XY, s32 FontSize)
{
  s32 QuadCount = strlen(Text);

  rect2 Result = { XY, XY };

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    if (Geo->CurrentIndex + 6 > Geo->Allocated)
      DrawDebugText(RG, Geo, Plat);

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

void
DebugFrameEnd(platform *Plat)
{
  /* TIMED_FUNCTION(); */
  debug_state *DebugState = GetDebugState();
  debug_text_render_group *RG = DebugState->TextRenderGroup;
  text_geometry_buffer *TextGeo = &RG->TextGeo;
  s32 FontSize = DEBUG_FONT_SIZE;

  r32 dt = Plat->dt;

  char dtBuffer[32] = {};
  sprintf(dtBuffer, "%f", dt);
  TextOutAt(Plat, RG, TextGeo, dtBuffer, V2(10, 1080-FontSize), FontSize);

#if _BONSAI_SLOW
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

        /* rect2 CCRect = TextOutAt(Plat, RG, TextGeo, CycleCountBuffer, 0, AtY, FontSize); */
        /* MaxX = max(MaxX, CCRect.Max.x); */
        /* AtY += (FontSize + LinePadding); */
      }
    }


    {
      char CycleCountBuffer[32] = {};
      sprintf(CycleCountBuffer, "%" PRIu64, MinCycleCount);
      TextOutAt(Plat, RG, TextGeo, CycleCountBuffer, V2(0, AtY), FontSize);
      AtY += (FontSize + LinePadding);
      AtY += (FontSize + LinePadding);
    }

    {
      char CycleCountBuffer[32] = {};
      sprintf(CycleCountBuffer, "%" PRIu64, CycleDelta);
      TextOutAt(Plat, RG, TextGeo, CycleCountBuffer, V2(0, AtY), FontSize);
      AtY += (FontSize + LinePadding);
      AtY += (FontSize + LinePadding);
    }

    {
      char CycleCountBuffer[32] = {};
      sprintf(CycleCountBuffer, "%" PRIu64, MaxCycleCount);
      TextOutAt(Plat, RG, TextGeo, CycleCountBuffer, V2(0, AtY), FontSize);
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
        TextOutAt(Plat, RG, TextGeo, PercentageBuffer, V2(AtX, AtY), FontSize);
        AtX += (FontSize*4);

        sprintf(PercentageBuffer, "%.0f", Entry->MaxPerc);
        TextOutAt(Plat, RG, TextGeo, PercentageBuffer, V2(AtX, AtY), FontSize);
        AtX += (FontSize*4);

        sprintf(PercentageBuffer, "%.0f", Entry->MinPerc);
        TextOutAt(Plat, RG, TextGeo, PercentageBuffer, V2(AtX, AtY), FontSize);
        AtX += (FontSize*4);

        /* // Print Hit Count */
        /* char CountBuffer[32]; */
        /* sprintf(CountBuffer, "%" PRIu32, Entry->HitCount); */
        /* rect2 HitCountRect = TextOutAt(Plat,  RG, CountBuffer, AtX, AtY, FontSize); */
        /* HitCountX = max((s32)HitCountRect.Max.x, HitCountX); */

        TextOutAt(Plat, RG, TextGeo, Entry->FuncName, V2(AtX, AtY), FontSize);

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

  DrawDebugText(RG, TextGeo, Plat);

  return;
}
#endif

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
