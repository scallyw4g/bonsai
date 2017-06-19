#ifndef BONSAI_DEBUG_CPP
#define BONSAI_DEBUG_CPP

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

void
initText2D(const char *TexturePath, debug_text_render_group *RG)
{
  RG->Text2DTextureID = loadDDS(TexturePath);

  GL_Global->glGenBuffers(1, &RG->Text2DVertexBufferID);
  GL_Global->glGenBuffers(1, &RG->Text2DUVBufferID);

  RG->Text2DShaderID = LoadShaders("TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");
  RG->Text2DUniformID = GL_Global->glGetUniformLocation(RG->Text2DShaderID, "myTextureSampler");

  return;
}

rect2
PrintDebugText( debug_text_render_group *RG, const char *Text, int x, int y, int size)
{
  unsigned int length = strlen(Text);
  unsigned int BufferIndex = 0;

  // Fill buffers
  v2 vertices[1024];
  v2 UVs[1024];

  rect2 Result = { V2(x, y), V2(x,y) };

  for ( unsigned int i=0 ; i<length ; i++ )
  {
    v2 vertex_up_left    = V2( x+i*size     , y+size );
    v2 vertex_up_right   = V2( x+i*size+size, y+size );
    v2 vertex_down_right = V2( x+i*size+size, y      );
    v2 vertex_down_left  = V2( x+i*size     , y      );

    Result.Max = vertex_up_right;

    char character = Text[i];
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

  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->Text2DVertexBufferID);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, (BufferIndex+1) * sizeof(v2), &vertices[0], GL_STATIC_DRAW);

  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->Text2DUVBufferID);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, (BufferIndex+1) * sizeof(v2), &UVs[0], GL_STATIC_DRAW);

  // Bind shader
  GL_Global->glUseProgram(RG->Text2DShaderID);

  // Bind texture
  GL_Global->glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RG->Text2DTextureID);

  GL_Global->glUniform1i(RG->Text2DUniformID, 0);

  // 1rst attribute buffer : vertices
  GL_Global->glEnableVertexAttribArray(0);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->Text2DVertexBufferID);
  GL_Global->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  // 2nd attribute buffer : UVs
  GL_Global->glEnableVertexAttribArray(1);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->Text2DUVBufferID);
  GL_Global->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Draw call
  glDrawArrays(GL_TRIANGLES, 0, BufferIndex );

  glDisable(GL_BLEND);

  GL_Global->glDisableVertexAttribArray(0);
  GL_Global->glDisableVertexAttribArray(1);

  AssertNoGlErrors;

  return Result;
}

DEBUG_GLOBAL u64 LastFrameCycleCount = 0;

void
DebugFrameEnd(debug_text_render_group *RG)
{
  debug_state *DebugState = GetDebugState();

  u64 CurrentFrameCycleCount = DebugState->GetCycleCount();
  u64 CycleDelta = CurrentFrameCycleCount - LastFrameCycleCount;

  LastFrameCycleCount = CurrentFrameCycleCount;

  debug_profile_entry NullEntry = {};

  s32 FontSize = DEBUG_FONT_SIZE;
  s32 LinePadding = 3;

  DEBUG_GLOBAL r32 MaxX;

  for (s32 EntryIndex = 0;
      EntryIndex < DEBUG_STATE_ENTRY_COUNT;
      ++EntryIndex)
  {
    debug_profile_entry *Entry = &DebugState->Entries[EntryIndex];

    for (s32 InnerEntryIndex = 0;
        InnerEntryIndex < DEBUG_STATE_ENTRY_COUNT;
        ++InnerEntryIndex)
    {
      debug_profile_entry *EntryInner = &DebugState->Entries[InnerEntryIndex];

      if (EntryInner->CycleCount > Entry->CycleCount)
      {
        debug_profile_entry Temp = *EntryInner;
        *EntryInner = *Entry;
        *Entry = Temp;
      }
    }
  }

  {
    s32 AtY = 0;
    for (s32 EntryIndex = 0;
        EntryIndex < DEBUG_STATE_ENTRY_COUNT;
        ++EntryIndex)
    {

      debug_profile_entry *Entry = &DebugState->Entries[EntryIndex];

      if (Entry->HitCount > 0)
      {
        char CycleCountBuffer[32];
        sprintf(CycleCountBuffer, "%" PRIu64, Entry->CycleCount);

        rect2 CCRect = PrintDebugText( RG, CycleCountBuffer, 0, AtY, FontSize);

        MaxX = max(MaxX, CCRect.Max.x);

        AtY += (FontSize + LinePadding);
      }
    }

    char CycleCountBuffer[32];
    sprintf(CycleCountBuffer, "%" PRIu64, CycleDelta);
    rect2 CCRect = PrintDebugText( RG, CycleCountBuffer, 0, AtY, FontSize);
    AtY += (FontSize + LinePadding);

  }

  {
    s32 AtY = 0;
    DEBUG_GLOBAL s32 PercX = 0;
    DEBUG_GLOBAL s32 HitCountX = 0;
    for (s32 EntryIndex = 0;
        EntryIndex < DEBUG_STATE_ENTRY_COUNT;
        ++EntryIndex)
    {
      debug_profile_entry *Entry = &DebugState->Entries[EntryIndex];
      if (Entry->HitCount > 0)
      {
        char PercentageBuffer[32];
        sprintf(PercentageBuffer, "%.0f", (r32)((r64)Entry->CycleCount/(r64)CycleDelta)*100);

        rect2 PercRect = PrintDebugText( RG, PercentageBuffer, (s32)MaxX, AtY, FontSize);
        PercX = max((s32)PercRect.Max.x, PercX);

        // Print Hit Count
        char CountBuffer[32];
        sprintf(CountBuffer, "%" PRIu32, Entry->HitCount);
        rect2 HitCountRect = PrintDebugText( RG, CountBuffer, PercX, AtY, FontSize);
        HitCountX = max((s32)HitCountRect.Max.x, HitCountX);

        PrintDebugText( RG, Entry->FuncName, HitCountX, AtY, FontSize);

        AtY += (FontSize + LinePadding);
      }

      *Entry = NullEntry;
    }
  }

}

void
CleanupText2D(debug_text_render_group *RG)
{
  // Delete buffers
  GL_Global->glDeleteBuffers(1, &RG->Text2DVertexBufferID);
  GL_Global->glDeleteBuffers(1, &RG->Text2DUVBufferID);

  // Delete texture
  glDeleteTextures(1, &RG->Text2DTextureID);

  // Delete shader
  GL_Global->glDeleteProgram(RG->Text2DShaderID);

  return;
}

#endif
