#if BONSAI_INTERNAL

#include <stdio.h>

void
DebugRegisterArena(const char *Name, memory_arena *Arena)
{

  Debug("Registered Arena %s", Name);
}

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

  GL_Global->glGenBuffers(1, &RG->SolidUIVertexBuffer);
  GL_Global->glGenBuffers(1, &RG->SolidUIColorBuffer);

  GL_Global->glGenBuffers(1, &RG->VertexBuffer);
  GL_Global->glGenBuffers(1, &RG->UVBuffer);
  GL_Global->glGenBuffers(1, &RG->ColorBuffer);

  RG->Text2DShader = LoadShaders("TextVertexShader.vertexshader",
                                 "TextVertexShader.fragmentshader", DebugArena);

  RG->TextureUniformID = GL_Global->glGetUniformLocation(RG->Text2DShader.ID, "myTextureSampler");

  RG->DebugFontTextureShader = MakeSimpleTextureShader(&RG->FontTexture, DebugArena);
  RG->DebugTextureShader = MakeSimpleTextureShader(RG->CompositedTexture, DebugArena);

  if (!CheckAndClearFramebuffer())
    return False;

  return True;
}

void
AllocateAndInitGeoBuffer(textured_2d_geometry_buffer *Geo, u32 VertCount, memory_arena *DebugArena)
{
  Geo->Verts = PUSH_STRUCT_CHECKED(v3, DebugArena, VertCount);
  Geo->Colors = PUSH_STRUCT_CHECKED(v3, DebugArena, VertCount);
  Geo->UVs = PUSH_STRUCT_CHECKED(v2, DebugArena, VertCount);
  Geo->Allocated = VertCount;
}

void
AllocateAndInitGeoBuffer(untextured_2d_geometry_buffer *Geo, u32 VertCount, memory_arena *DebugArena)
{
  Geo->Verts = PUSH_STRUCT_CHECKED(v3, DebugArena, VertCount);
  Geo->Colors = PUSH_STRUCT_CHECKED(v3, DebugArena, VertCount);
  Geo->Allocated = VertCount;
  return;
}

void
InitScopeTree(debug_state *State, debug_scope_tree *WriteScopeTree)
{
  State->NumScopes = 0;
  State->CurrentScope = 0;
  State->WriteScope = &WriteScopeTree->Root;
  return;
}

shader
MakeSolidUIShader(memory_arena *DebugMemory)
{
  shader SimpleTextureShader = LoadShaders( "SimpleColor.vertexshader",
                                            "SimpleColor.fragmentshader",
                                            DebugMemory );
  return SimpleTextureShader;
}

void
InitDebugState(platform *Plat)
{
  GlobalDebugState = &Plat->DebugState;
  GlobalDebugState->GetCycleCount = Plat->GetCycleCount;

  GlobalDebugState->FreeScopeSentinel.Parent = &GlobalDebugState->FreeScopeSentinel;
  GlobalDebugState->FreeScopeSentinel.Child = &GlobalDebugState->FreeScopeSentinel;

  GlobalDebugState->Memory = PUSH_STRUCT_CHECKED(memory_arena, Plat->Memory, 1);
  SubArena(Plat->Memory, GlobalDebugState->Memory, Megabytes(128));

  GlobalDebugState->TextRenderGroup = PUSH_STRUCT_CHECKED(debug_text_render_group, Plat->Memory, 1);
  if (!InitDebugOverlayFramebuffer(GlobalDebugState->TextRenderGroup, Plat->Memory, "Holstein.DDS"))
  { Error("Initializing Debug Overlay Framebuffer"); }

  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup->TextGeo, 512, Plat->Memory);
  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup->UIGeo, 512, Plat->Memory);

  GlobalDebugState->TextRenderGroup->SolidUIShader = MakeSolidUIShader(GlobalDebugState->Memory);

  GlobalDebugState->Initialized = True;
  return;
}

void
FlushTextBuffer(debug_text_render_group *RG, textured_2d_geometry_buffer *Geo, v2 ViewportDim)
{
  u32 VertCount = Geo->CurrentIndex +1;
  Geo->CurrentIndex = 0;

  GL_Global->glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Bind Text shader
  GL_Global->glUseProgram(RG->Text2DShader.ID);

  // Bind Font texture
  GL_Global->glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RG->FontTexture.ID);
  GL_Global->glUniform1i(RG->TextureUniformID, 0);

  // Verteces
  GL_Global->glEnableVertexAttribArray(0);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->VertexBuffer);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(v3), Geo->Verts, GL_STATIC_DRAW);
  GL_Global->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  // UVs
  GL_Global->glEnableVertexAttribArray(1);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->UVBuffer);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(v2), Geo->UVs, GL_STATIC_DRAW);
  GL_Global->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  // Colors
  GL_Global->glEnableVertexAttribArray(2);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->ColorBuffer);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(v3), Geo->Colors, GL_STATIC_DRAW);
  GL_Global->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisable(GL_DEPTH_TEST);

  // Draw
  SetViewport(V2(SCR_WIDTH, SCR_HEIGHT));
  glDrawArrays(GL_TRIANGLES, 0, VertCount);

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  GL_Global->glDisableVertexAttribArray(0);
  GL_Global->glDisableVertexAttribArray(1);
  GL_Global->glDisableVertexAttribArray(2);

  AssertNoGlErrors;
}

void
BufferTextUVs(textured_2d_geometry_buffer *Geo, v2 UV)
{
  v2 uv_up_left    = V2( UV.x           , UV.y );
  v2 uv_up_right   = V2( UV.x+1.0f/16.0f, UV.y );
  v2 uv_down_right = V2( UV.x+1.0f/16.0f, (UV.y + 1.0f/16.0f) );
  v2 uv_down_left  = V2( UV.x           , (UV.y + 1.0f/16.0f) );

  u32 StartingIndex = Geo->CurrentIndex;
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

v2
BufferQuad(v3 *Verts, u32 StartingIndex, v2 MinP, v2 Dim)
{
  v3 vertex_up_left    = V3( MinP.x       , MinP.y+Dim.y , 0.5f);
  v3 vertex_up_right   = V3( MinP.x+Dim.x , MinP.y+Dim.y , 0.5f);
  v3 vertex_down_right = V3( MinP.x+Dim.x , MinP.y       , 0.5f);
  v3 vertex_down_left  = V3( MinP.x       , MinP.y       , 0.5f);

  v3 XYClip = (1.0f / V3(SCR_WIDTH, SCR_HEIGHT, 1));

  Verts[StartingIndex++] = (vertex_up_left * XYClip) * 2.0f - 1;
  Verts[StartingIndex++] = (vertex_down_left * XYClip) * 2.0f - 1;
  Verts[StartingIndex++] = (vertex_up_right * XYClip) * 2.0f - 1;
  Verts[StartingIndex++] = (vertex_down_right * XYClip) * 2.0f - 1;
  Verts[StartingIndex++] = (vertex_up_right * XYClip) * 2.0f - 1;
  Verts[StartingIndex++] = (vertex_down_left * XYClip) * 2.0f - 1;

  v2 Max = vertex_up_right.xy;
  return Max;
}

rect2
BufferTextAt(debug_text_render_group *RG, textured_2d_geometry_buffer *Geo,
    const char *Text, v2 XY, s32 FontSize, v2 ViewportDim, u32 ColorIndex)
{
  s32 QuadCount = strlen(Text);

  rect2 Result = { XY, XY };

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    if (Geo->CurrentIndex + 6 > Geo->Allocated)
      FlushTextBuffer(RG, Geo, ViewportDim);

    char character = Text[CharIndex];
    v2 UV = V2( (character%16)/16.0f, (character/16)/16.0f );
    BufferTextUVs(Geo, UV);

    v2 MinP = V2(XY.x + (FontSize*CharIndex), XY.y);
    Result.Max = BufferQuad(Geo->Verts, Geo->CurrentIndex, MinP, V2(FontSize, FontSize));

    BufferColors(Geo->Colors, Geo->CurrentIndex, default_palette[ColorIndex].xyz);

    Geo->CurrentIndex += 6;

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

debug_profile_scope NullScope = {};

void
FreeScopes(debug_state *DebugState, debug_profile_scope *ScopeToFree)
{
  if (!ScopeToFree) return;

  FreeScopes(DebugState, ScopeToFree->Child);
  FreeScopes(DebugState, ScopeToFree->Sibling);

  *ScopeToFree = NullScope;

  debug_profile_scope *Sentinel = &DebugState->FreeScopeSentinel;
  debug_profile_scope *First = Sentinel->Child;

  Sentinel->Child = ScopeToFree;
  First->Parent = ScopeToFree;

  ScopeToFree->Parent = Sentinel;
  ScopeToFree->Child = First;

  Assert(Sentinel->Parent);

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

inline void
BufferText(const char *Text, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  rect2 TextBox = BufferTextAt(RG, &RG->TextGeo, Text, V2(Layout->At.x, Layout->At.y), Layout->FontSize, ViewportDim, ColorIndex );
  Layout->At.x = TextBox.Max.x;

  return;
}

inline void
BufferText(r32 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%f", Number);
  BufferText( Buffer, Layout, RG, ViewportDim, ColorIndex);
  return;
}

inline void
BufferText(u64 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Number);
  BufferText( Buffer, Layout, RG, ViewportDim, ColorIndex);
  return;
}

inline void
BufferText(u32 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Number);
  BufferText( Buffer, Layout, RG, ViewportDim, ColorIndex);
  return;
}

inline void
NewLine(layout *Layout)
{
  Layout->At.y -= (Layout->LineHeight);
  Layout->At.x = 0;
  return;
}

inline void
AdvanceSpaces(u32 N, layout *Layout)
{
  Layout->At.x += (N*Layout->FontSize);
  return;
}

inline void
BufferCycles(u64 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
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
  BufferText( Buffer, Layout, RG, ViewportDim, ColorIndex);

  return;
}

inline void
BufferColumn( u64 Value, u32 ColumnWidth, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Value);
  {
    s32 Len = strlen(Buffer);
    s32 Pad = Max(ColumnWidth-Len, 0);
    AdvanceSpaces(Pad, Layout);
  }
  BufferText( Buffer, Layout, RG, ViewportDim, ColorIndex);
  return;
}

inline void
BufferColumn( r32 Perc, u32 ColumnWidth, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%.1f", Perc);
  {
    s32 Len = strlen(Buffer);
    s32 Pad = Max(ColumnWidth-Len, 0);
    AdvanceSpaces(Pad, Layout);
  }
  BufferText( Buffer, Layout, RG, ViewportDim, ColorIndex);
  return;
}

inline void
BufferNumberAsText(r32 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferText(Number, Layout, RG, ViewportDim, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(r64 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferText((r32)Number, Layout, RG, ViewportDim, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(u64 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferText(Number, Layout, RG, ViewportDim, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(u32 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferText(Number, Layout, RG, ViewportDim, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}

inline b32
StringsMatch(const char *S1, const char *S2)
{
  b32 Result = strcmp(S1, S2) == 0;
  return Result;
}

inline void
BufferScopeTreeEntry(debug_profile_scope *Scope, layout *Layout, u32 Color, u64 TotalCycles, u64 TotalFrameCycles, u64 CallCount, debug_text_render_group *RG, v2 ViewportDim, u32 Depth)
{
  Assert(TotalFrameCycles);
  r32 Percentage = 100.0f*(r32)((r64)TotalCycles/(r64)TotalFrameCycles);
  u64 AvgCycles = SafeDivide0(TotalCycles, CallCount);
  BufferColumn(Percentage, 6, Layout, RG, ViewportDim, Color);
  BufferCycles(AvgCycles, Layout, RG, ViewportDim, Color);
  BufferColumn(CallCount, 5, Layout, RG, ViewportDim, Color);

  AdvanceSpaces(Depth*2.0f + 1, Layout);

  if (Scope->Expanded && Scope->Child)
  {
    BufferText("-", Layout, RG, ViewportDim, Color);
  }
  else if (Scope->Child)
  {
    BufferText("+", Layout, RG, ViewportDim, Color);
  }
  else
  {
    AdvanceSpaces(1, Layout);
  }

  BufferText(Scope->Name, Layout, RG, ViewportDim, Color);

  NewLine(Layout);
}

inline rect2
GetNextLineBounds(layout *Layout)
{
  v2 StartingP = Layout->At;
  // FIXME(Jesse): Should line length be systemized somehow?
  v2 EndingP = Layout->At + V2(100000.0f, Layout->LineHeight);
  rect2 Result = { StartingP, EndingP };
  return Result;
}

inline b32
IsInsideRect(rect2 Rect, v2 P)
{
  b32 Result = (P > Rect.Min && P < Rect.Max);
  return Result;
}

void
BufferFirstCallToEach(debug_profile_scope *Scope, debug_state *State, layout *Layout,
                v2 ViewportDim, u64 TotalFrameCycles, u32 Depth, input *Input, v2 MouseP);


scope_stats
GetStatsFor(debug_state *State, debug_profile_scope *Scope)
{
  scope_stats Result = {};

  debug_profile_scope *Next = State->GetReadScopeTree()->Root;
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

u32
HoverAndClickExpand(layout *Layout, debug_profile_scope *Scope, v2 MouseP, input *Input, u32 Color, u32 HoverColor)
{
  u32 DrawColor = Color;

  {
    rect2 EntryBounds = GetNextLineBounds(Layout);
    if ( IsInsideRect(EntryBounds, MouseP) )
    {
      DrawColor = HoverColor;
      if (Input->LMB.WasPressed)
        Scope->Expanded = !Scope->Expanded;
    }
  }

  return DrawColor;
}

void
BufferFirstCallToEach(debug_profile_scope *Scope, debug_state *State, layout *Layout,
                      v2 ViewportDim, u64 TotalFrameCycles, u32 Depth, input *Input, v2 MouseP)
{
  if (!Scope) return;

  if (!Scope->Stats)
  {
    Scope->Stats = PUSH_STRUCT_CHECKED(scope_stats, State->Memory, 1);
    *Scope->Stats = GetStatsFor(State, Scope);
  }

  if (Scope->Stats->IsFirst)
  {
    u32 MainColor = HoverAndClickExpand(Layout, Scope, MouseP, Input, WHITE, TEAL);
    BufferScopeTreeEntry(Scope, Layout, MainColor, Scope->Stats->CumulativeCycles, TotalFrameCycles, Scope->Stats->Calls, State->TextRenderGroup, ViewportDim, Depth);

    if (Scope->Expanded)
      BufferFirstCallToEach(Scope->Stats->MaxScope->Child, State, Layout, ViewportDim, TotalFrameCycles, Depth+1, Input, MouseP);

  }

  BufferFirstCallToEach(Scope->Sibling, State, Layout, ViewportDim, TotalFrameCycles, Depth, Input, MouseP);

  return;
}

void
DebugFrameBegin(hotkeys *Hotkeys, r32 Dt, u64 Cycles)
{
  debug_state *State = GetDebugState();

  if ( Hotkeys->Debug_ToggleProfile )
  {
    Hotkeys->Debug_ToggleProfile = False;
    State->DoScopeProfiling = !State->DoScopeProfiling;
  }

  if ( Hotkeys->Debug_NextUiState )
  {
    Hotkeys->Debug_NextUiState = False;
    State->UIType = (debug_ui_type)(((s32)State->UIType + 1) % (s32)DebugUIType_Count);
  }

  { // Record dt/cycles for the frame we're finishing with
    debug_scope_tree *WriteScope = State->GetWriteScopeTree();
    if (WriteScope)
    {
      WriteScope->FrameMs = Dt*1000.0f;
      WriteScope->TotalCycles = Cycles;
    }
  }

  if (!State->DoScopeProfiling) return;

  { // Advance to the next scope and reinitialize
    State->ReadScopeIndex = (State->ReadScopeIndex+1) % ROOT_SCOPE_COUNT;
    debug_scope_tree *WriteScope = State->GetWriteScopeTree();
    if (WriteScope)
    {
      FreeScopes(State, WriteScope->Root);
      InitScopeTree(State, WriteScope);
    }
  }

  return;
}

b32
BufferIsFull(untextured_2d_geometry_buffer *Buffer, u32 VertsToPush)
{
  b32 Result = (Buffer->CurrentIndex + VertsToPush) > Buffer->Allocated;
  return Result;
}

void BindShaderUniforms(shader *Shader);

void
UseShader(shader *Shader)
{
  GL_Global->glUseProgram(Shader->ID);
  BindShaderUniforms(Shader);
  return;
}

void
FlushSolidUIGeo(debug_text_render_group *RG, v2 ViewportDim)
{
  TIMED_FUNCTION();
  untextured_2d_geometry_buffer *Buffer = &RG->UIGeo;

  GL_Global->glBindFramebuffer(GL_FRAMEBUFFER, 0);

  UseShader(&RG->SolidUIShader);

  u32 VertCount = Buffer->CurrentIndex + 1;
  Buffer->CurrentIndex = 0;

  // Vertices
  GL_Global->glEnableVertexAttribArray(0);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->SolidUIVertexBuffer);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(v3), Buffer->Verts, GL_STATIC_DRAW);
  GL_Global->glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );

  // Colors
  GL_Global->glEnableVertexAttribArray(1);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->SolidUIColorBuffer);
  GL_Global->glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(v3), Buffer->Colors, GL_STATIC_DRAW);
  GL_Global->glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glDepthFunc(GL_ALWAYS);

  SetViewport(V2(SCR_WIDTH, SCR_HEIGHT));
  glDrawArrays(GL_TRIANGLES, 0, VertCount);

  glDepthFunc(GL_LEQUAL);

  GL_Global->glDisableVertexAttribArray(0);
  GL_Global->glDisableVertexAttribArray(1);

  AssertNoGlErrors;

  return;
}

inline void
PadBottom(layout *Layout, r32 Pad)
{
  Layout->At.y -= Pad;
}

void
SetFontSize(layout *Layout, r32 FontSize)
{
  Layout->FontSize = FontSize;
  Layout->LineHeight = FontSize * 1.3f;
  return;
}

void
DebugDrawCallGraph(debug_state *DebugState, layout *Layout, debug_text_render_group *RG, textured_2d_geometry_buffer *TextGeo, v2 ViewportDim, v2 MouseP, u32 MaxMs, input *Input)
{
  SetFontSize(Layout, 80);
  NewLine(Layout);

  TIMED_BLOCK("Frame Ticker");
    for (u32 TreeIndex = 0;
        TreeIndex < ROOT_SCOPE_COUNT;
        ++TreeIndex )
    {
      debug_scope_tree *Tree = &DebugState->ScopeTrees[TreeIndex];

      r32 Perc = SafeDivide0(Tree->FrameMs, MaxMs);
      v2 MinP = V2(Layout->At.x, Layout->At.y);
      v2 QuadDim = V2(15.0, (Layout->FontSize) * Perc);

      v2 DrawDim = BufferQuad(RG->UIGeo.Verts, RG->UIGeo.CurrentIndex, MinP, QuadDim);
      Layout->At.x = DrawDim.x + 5.0f;

      v3 Color = V3(0.5f, 0.5f, 0.5f);
      if ( Tree == DebugState->GetWriteScopeTree() )
        Color = V3(0.8f, 0.0f, 0.0f);

      if ( Tree == DebugState->GetReadScopeTree() )
        Color = V3(0.8f, 0.8f, 0.0f);

      v2 AspectCorrectMinP = (MinP); // / ViewportDim ) * V2((r32)SCR_HEIGHT, (r32)SCR_WIDTH);
      v2 AspectCorrectDrawDim = (DrawDim); // / ViewportDim ) * V2((r32)SCR_HEIGHT, (r32)SCR_WIDTH);

      /* Print(MouseP); */
      /* Print(AspectCorrectMinP); */
      /* Print(AspectCorrectDrawDim); */
      /* Print(MinP); */
      /* Print(DrawDim); */
      /* Debug("------------------------------"); */

      if (MouseP > AspectCorrectMinP && MouseP < AspectCorrectDrawDim)
      {
        DebugState->ReadScopeIndex = TreeIndex;
        Color = V3(0.8f, 0.8f, 0.0f);
      }

      BufferColors(RG->UIGeo.Colors, RG->UIGeo.CurrentIndex, Color);

      RG->UIGeo.CurrentIndex+=6;
    }
  END_BLOCK("Frame Ticker");

  TIMED_BLOCK("Call Graph");

    debug_scope_tree *ReadTree = DebugState->GetReadScopeTree();
    layout TreeInfoLayout = *Layout;
    { // Current ReadTree info
      TreeInfoLayout.FontSize = 36;
      TreeInfoLayout.LineHeight = 36 * 1.3f;
      BufferColumn(ReadTree->FrameMs, 4, &TreeInfoLayout, RG, ViewportDim, WHITE);
      BufferCycles(ReadTree->TotalCycles, &TreeInfoLayout, RG, ViewportDim, WHITE);
    }

    layout CallGraphLayout = TreeInfoLayout;
    { // Call Graph
      PadBottom(&CallGraphLayout, 15);
      CallGraphLayout.FontSize = 30;
      CallGraphLayout.LineHeight = CallGraphLayout.FontSize*1.3f;
      NewLine(&CallGraphLayout);
      BufferFirstCallToEach(ReadTree->Root, DebugState, &CallGraphLayout, ViewportDim, ReadTree->TotalCycles, 0, Input, MouseP);
    }
  END_BLOCK("Call Graph");
}

void
DebugDrawMemoryHud(debug_state *DebugState, layout *Layout, debug_text_render_group *RG, textured_2d_geometry_buffer *TextGeo, v2 ViewportDim, v2 MouseP)
{
  SetFontSize(Layout, 40);
  NewLine(Layout);
  BufferText("Memory HUD", Layout, RG, ViewportDim, WHITE);
  return;
}

struct min_max_avg_dt
{
  r32 Min;
  r32 Max;
  r32 Avg;
};

min_max_avg_dt
ComputeMinMaxAvgDt(debug_scope_tree *ScopeTrees)
{
  TIMED_FUNCTION();

  min_max_avg_dt Dt = {};

    for (u32 TreeIndex = 0;
        TreeIndex < ROOT_SCOPE_COUNT;
        ++TreeIndex )
    {
      debug_scope_tree *Tree = &ScopeTrees[TreeIndex];

      Dt.Min = Min(Dt.Min, Tree->FrameMs);
      Dt.Max = Max(Dt.Max, Tree->FrameMs);
      Dt.Avg += Tree->FrameMs;
    }
    Dt.Avg /= (r32)ROOT_SCOPE_COUNT;

  return Dt;
}

void
DebugFrameEnd(platform *Plat, u64 FrameCycles)
{
  TIMED_FUNCTION();
  debug_state *DebugState = GetDebugState();
  debug_text_render_group *RG = DebugState->TextRenderGroup;
  textured_2d_geometry_buffer *TextGeo = &RG->TextGeo;

  v2 ViewportDim = V2(Plat->WindowWidth, Plat->WindowHeight);
  v2 MouseP = V2(Plat->MouseP.x, Plat->WindowHeight - Plat->MouseP.y);

  min_max_avg_dt Dt = ComputeMinMaxAvgDt(DebugState->ScopeTrees);

  layout Layout(DEBUG_FONT_SIZE);
  TIMED_BLOCK("Draw Status Bar");
    Layout.At.y = (r32)SCR_HEIGHT - Layout.FontSize;
    BufferColumn(Dt.Max, 6, &Layout, RG, ViewportDim, WHITE);
    NewLine(&Layout);

    BufferColumn(Dt.Avg, 6, &Layout, RG, ViewportDim, WHITE);
    BufferColumn(Plat->dt*1000.0f, 6, &Layout, RG, ViewportDim, WHITE);
    BufferText("ms", &Layout, RG, ViewportDim, WHITE);
    NewLine(&Layout);

    BufferColumn(Dt.Min, 6, &Layout, RG, ViewportDim, WHITE);
  END_BLOCK("Status Bar");

  switch (DebugState->UIType)
  {
    case DebugUIType_None:
    {
    } break;

    case DebugUIType_CallGraph:
    {
      DebugDrawCallGraph(DebugState, &Layout, RG, TextGeo, ViewportDim, MouseP, Dt.Max, &Plat->Input);
    } break;

    case DebugUIType_MemoryHud:
    {
      DebugDrawMemoryHud(DebugState, &Layout, RG, TextGeo, ViewportDim, MouseP);
    } break;

    InvalidDefaultCase;
  }

  FlushSolidUIGeo(RG, ViewportDim);
  FlushTextBuffer(RG, TextGeo, ViewportDim);

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
