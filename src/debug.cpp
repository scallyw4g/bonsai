#include <debug.h>

texture *
MakeTexture_RGBA( v2i Dim, const void* Data, memory_arena *Memory);

b32
InitDebugOverlayFramebuffer(debug_text_render_group *RG, memory_arena *DebugArena, const char *DebugFont)
{
  GL_Global->glGenFramebuffers(1, &RG->FBO.ID);
  GL_Global->glBindFramebuffer(GL_FRAMEBUFFER, RG->FBO.ID);

  v2i ScreenDim = V2i(SCR_WIDTH, SCR_HEIGHT);

  RG->FontTexture = LoadDDS(DebugFont);
  RG->CompositedTexture = MakeTexture_RGBA( ScreenDim, 0, DebugArena);

  GL_Global->glGenBuffers(1, &RG->VertexBuffer);
  GL_Global->glGenBuffers(1, &RG->UVBuffer);

  RG->Text2DShader = LoadShaders("TextVertexShader.vertexshader",
                                 "TextVertexShader.fragmentshader", DebugArena);

  RG->TextureUniformID = GL_Global->glGetUniformLocation(RG->Text2DShader.ID, "myTextureSampler");

  return True;
}

void
InitDebugState( debug_state *DebugState, platform *Plat)
{
  DebugState->GetCycleCount = Plat->GetCycleCount;

  DebugState->TextRenderGroup = PUSH_STRUCT_CHECKED(debug_text_render_group, Plat->Memory, 1);
  InitDebugOverlayFramebuffer(DebugState->TextRenderGroup, Plat->Memory, "Holstein.DDS");

  return;
}

rect2
PrintDebugText( debug_text_render_group *RG, const char *Text, v2 XY, s32 FontSize)
{
  GL_Global->glBindFramebuffer(GL_FRAMEBUFFER, 0);
  /* glDepthFunc(GL_ALWAYS); */

  s32 TextLength = strlen(Text);
  s32 BufferIndex = 0;

  // Fill buffers
  v3 vertices[1024];
  v2 UVs[1024];

  rect2 Result = { XY, XY };

  for ( s32 CharIndex = 0;
      CharIndex < TextLength;
      CharIndex++ )
  {
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


    vertices[BufferIndex] = vertex_up_left;
    UVs[BufferIndex++] = uv_up_left;

    vertices[BufferIndex] = vertex_down_left;
    UVs[BufferIndex++] = uv_down_left;

    vertices[BufferIndex] = vertex_up_right;
    UVs[BufferIndex++] = uv_up_right;


    vertices[BufferIndex] = vertex_down_right;
    UVs[BufferIndex++] = uv_down_right;

    vertices[BufferIndex] = vertex_up_right;
    UVs[BufferIndex++] = uv_up_right;

    vertices[BufferIndex] = vertex_down_left;
    UVs[BufferIndex++] = uv_down_left;

    continue;
  }

  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->VertexBuffer);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, (BufferIndex+1) * sizeof(v3), &vertices[0], GL_STATIC_DRAW);

  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->UVBuffer);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, (BufferIndex+1) * sizeof(v2), &UVs[0], GL_STATIC_DRAW);

  // Bind shader
  GL_Global->glUseProgram(RG->Text2DShader.ID);

  // Bind texture
  GL_Global->glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RG->FontTexture.ID);

  GL_Global->glUniform1i(RG->TextureUniformID, 0);

  // 1rst attribute buffer : vertices
  GL_Global->glEnableVertexAttribArray(0);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->VertexBuffer);
  GL_Global->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  // 2nd attribute buffer : UVs
  GL_Global->glEnableVertexAttribArray(1);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->UVBuffer);
  GL_Global->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Draw call
  /* SetViewport( V2(SCR_HEIGHT, SCR_WIDTH) ); */
  glDrawArrays(GL_TRIANGLES, 0, BufferIndex);

  glDisable(GL_BLEND);

  GL_Global->glDisableVertexAttribArray(0);
  GL_Global->glDisableVertexAttribArray(1);

  AssertNoGlErrors;

  glDepthFunc(GL_LEQUAL);
  return Result;
}

debug_global u64 LastFrameCycleCount = 0;

inline r32
CalculateFramePercentage(debug_profile_entry *Entry, u64 CycleDelta)
{
  u64 TotalCycles = Entry->CycleCount;
  r32 FramePerc = (r32)((r64)TotalCycles/(r64)CycleDelta)*100;

  return FramePerc;
}

void
DebugFrameEnd(r32 dt)
{
  /* TIMED_FUNCTION(); */

  debug_state *DebugState = GetDebugState();
  debug_text_render_group *RG = DebugState->TextRenderGroup;
  s32 FontSize = DEBUG_FONT_SIZE;

  char dtBuffer[32] = {};
  sprintf(dtBuffer, "%f", dt);
  PrintDebugText( RG, dtBuffer, V2(10, (SCR_HEIGHT-FontSize-10)/2), FontSize);

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

        /* rect2 CCRect = PrintDebugText( RG, CycleCountBuffer, 0, AtY, FontSize); */
        /* MaxX = max(MaxX, CCRect.Max.x); */
        /* AtY += (FontSize + LinePadding); */
      }
    }


    {
      char CycleCountBuffer[32] = {};
      sprintf(CycleCountBuffer, "%" PRIu64, MinCycleCount);
      PrintDebugText( RG, CycleCountBuffer, V2(0, AtY), FontSize);
      AtY += (FontSize + LinePadding);
      AtY += (FontSize + LinePadding);
    }

    {
      char CycleCountBuffer[32] = {};
      sprintf(CycleCountBuffer, "%" PRIu64, CycleDelta);
      PrintDebugText( RG, CycleCountBuffer, V2(0, AtY), FontSize);
      AtY += (FontSize + LinePadding);
      AtY += (FontSize + LinePadding);
    }

    {
      char CycleCountBuffer[32] = {};
      sprintf(CycleCountBuffer, "%" PRIu64, MaxCycleCount);
      PrintDebugText( RG, CycleCountBuffer, V2(0, AtY), FontSize);
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
        PrintDebugText( RG, PercentageBuffer, V2(AtX, AtY), FontSize);
        AtX += (FontSize*4);

        sprintf(PercentageBuffer, "%.0f", Entry->MaxPerc);
        PrintDebugText( RG, PercentageBuffer, V2(AtX, AtY), FontSize);
        AtX += (FontSize*4);

        sprintf(PercentageBuffer, "%.0f", Entry->MinPerc);
        PrintDebugText( RG, PercentageBuffer, V2(AtX, AtY), FontSize);
        AtX += (FontSize*4);

        /* // Print Hit Count */
        /* char CountBuffer[32]; */
        /* sprintf(CountBuffer, "%" PRIu32, Entry->HitCount); */
        /* rect2 HitCountRect = PrintDebugText( RG, CountBuffer, AtX, AtY, FontSize); */
        /* HitCountX = max((s32)HitCountRect.Max.x, HitCountX); */

        PrintDebugText( RG, Entry->FuncName, V2(AtX, AtY), FontSize);

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
