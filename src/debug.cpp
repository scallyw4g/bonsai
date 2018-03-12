#if BONSAI_INTERNAL

#include <stdio.h>

debug_profile_scope NullScope = {};
debug_global b32 DebugGlobal_RedrawEveryPush = 0;

void
DebugRegisterArena(const char *Name, memory_arena *Arena)
{
  /* debug_state *State = GetDebugState(); */
  b32 Registered = False;
  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = &Global_RegisteredMemoryArenas[Index];

    if (!Current->Name)
    {
      Current->Name = Name;
      Current->Arena = Arena;
      Registered = True;
      break;
    }
  }

  if (Registered)
  {
    Info("Registered Arena : %s", Name);
  }
  else
  {
    Error("Registering Arena : %s", Name);
  }

  return;
}

texture *
MakeTexture_RGBA( v2i Dim, const void* Data, memory_arena *Memory);

void
FramebufferTexture(framebuffer *FBO, texture *Tex);

shader
MakeSimpleTextureShader(texture *Texture, memory_arena *GraphicsMemory);

b32
CheckAndClearFramebuffer();

void
BindShaderUniforms(shader *Shader);

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
InitDebugState(platform *Plat, memory_arena *DebugMemory)
{
  GlobalDebugState = &Plat->DebugState;
  GlobalDebugState->Memory = DebugMemory;
  GlobalDebugState->GetCycleCount = Plat->GetCycleCount;


  GlobalDebugState->FreeScopeSentinel.Parent = &GlobalDebugState->FreeScopeSentinel;
  GlobalDebugState->FreeScopeSentinel.Child = &GlobalDebugState->FreeScopeSentinel;

  GlobalDebugState->TextRenderGroup = PUSH_STRUCT_CHECKED(debug_text_render_group, DebugMemory, 1);
  if (!InitDebugOverlayFramebuffer(GlobalDebugState->TextRenderGroup, DebugMemory, "Holstein.DDS"))
  { Error("Initializing Debug Overlay Framebuffer"); }

  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup->TextGeo, 1024, DebugMemory);
  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup->UIGeo, 1024, DebugMemory);

  GlobalDebugState->TextRenderGroup->SolidUIShader = MakeSolidUIShader(GlobalDebugState->Memory);

  GlobalDebugState->Initialized = True;
  return;
}

void
UseShader(shader *Shader)
{
  GL_Global->glUseProgram(Shader->ID);
  BindShaderUniforms(Shader);
  return;
}

void
FlushBuffer(debug_text_render_group *RG, untextured_2d_geometry_buffer *Buffer, v2 ViewportDim)
{
  TIMED_FUNCTION();

  GL_Global->glBindFramebuffer(GL_FRAMEBUFFER, 0);

  UseShader(&RG->SolidUIShader);

  u32 VertCount = Buffer->CurrentIndex;

  BEGIN_CARD_BUFFERING();
    BUFFER_VERTS_TO_CARD(RG->SolidUIVertexBuffer, Buffer);
    BUFFER_COLORS_TO_CARD(RG->SolidUIColorBuffer, Buffer);
  END_CARD_BUFFERING();

  SetViewport(V2(SCR_WIDTH, SCR_HEIGHT));
  Draw(VertCount);

  Buffer->CurrentIndex = 0;

  GL_Global->glDisableVertexAttribArray(0);
  GL_Global->glDisableVertexAttribArray(1);

  AssertNoGlErrors;

  return;
}

void
FlushBuffer(debug_text_render_group *RG, textured_2d_geometry_buffer *Geo, v2 ViewportDim)
{
  u32 VertCount = Geo->CurrentIndex;

  GL_Global->glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Bind Text shader
  GL_Global->glUseProgram(RG->Text2DShader.ID);

  // Bind Font texture
  GL_Global->glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RG->FontTexture.ID);
  GL_Global->glUniform1i(RG->TextureUniformID, 0);

  BEGIN_CARD_BUFFERING();
    BUFFER_VERTS_TO_CARD(RG->SolidUIVertexBuffer, Geo);
    BUFFER_UVS_TO_CARD(RG->UVBuffer, Geo);
    BUFFER_COLORS_TO_CARD(RG->SolidUIColorBuffer, Geo);
  END_CARD_BUFFERING();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Draw
  SetViewport(V2(SCR_WIDTH, SCR_HEIGHT));
  Draw(VertCount);

  glDisable(GL_BLEND);

  Geo->CurrentIndex = 0;

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

void
BufferColors(ui_render_group *Group, textured_2d_geometry_buffer *Geo, v3 Color)
{
  if (Geo->CurrentIndex + 6 > Geo->Allocated)
    FlushBuffer(Group->TextGroup, Geo, Group->ViewportDim);

  BufferColors(Geo->Colors, Geo->CurrentIndex, Color);
}

void
BufferColors(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v3 Color)
{
  if (Geo->CurrentIndex + 6 > Geo->Allocated)
    FlushBuffer(Group->TextGroup, Geo, Group->ViewportDim);

  BufferColors(Geo->Colors, Geo->CurrentIndex, Color);
}

v2
BufferQuadDirect(v3 *Dest, u32 StartingIndex, v2 MinP, v2 Dim, r32 Z)
{
  v3 vertex_up_left    = V3( MinP.x       , MinP.y+Dim.y , Z);
  v3 vertex_up_right   = V3( MinP.x+Dim.x , MinP.y+Dim.y , Z);
  v3 vertex_down_right = V3( MinP.x+Dim.x , MinP.y       , Z);
  v3 vertex_down_left  = V3( MinP.x       , MinP.y       , Z);

  v3 XYClip = (1.0f / V3(SCR_WIDTH, SCR_HEIGHT, 1));

  Dest[StartingIndex++] = (vertex_up_left * XYClip) * 2.0f - 1;
  Dest[StartingIndex++] = (vertex_down_left * XYClip) * 2.0f - 1;
  Dest[StartingIndex++] = (vertex_up_right * XYClip) * 2.0f - 1;
  Dest[StartingIndex++] = (vertex_down_right * XYClip) * 2.0f - 1;
  Dest[StartingIndex++] = (vertex_up_right * XYClip) * 2.0f - 1;
  Dest[StartingIndex++] = (vertex_down_left * XYClip) * 2.0f - 1;

  v2 Max = vertex_up_right.xy;
  return Max;
}

v2
BufferQuad(ui_render_group *Group, textured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z = 0.0f)
{
  if (Geo->CurrentIndex + 6 > Geo->Allocated)
    FlushBuffer(Group->TextGroup, Geo, Group->ViewportDim);

  v2 Result = BufferQuadDirect(Geo->Verts, Geo->CurrentIndex, MinP, Dim, Z);
  return Result;
}

v2
BufferQuad(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z = 0.0f)
{
  if (Geo->CurrentIndex + 6 > Geo->Allocated)
    FlushBuffer(Group->TextGroup, Geo, Group->ViewportDim);

  v2 Result = BufferQuadDirect(Geo->Verts, Geo->CurrentIndex, MinP, Dim, Z);
  return Result;
}

rect2
BufferTextAt(ui_render_group *Group, const char *Text, u32 Color)
{
  textured_2d_geometry_buffer *TextGeo = &Group->TextGroup->TextGeo;
  layout *Layout = Group->Layout;

  s32 QuadCount = strlen(Text);

  rect2 Result = { Layout->At, Layout->At };

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = Layout->At + V2(Layout->FontSize*CharIndex, 0);
    Result.Max = BufferQuad(Group, TextGeo, MinP, V2(Layout->FontSize));

    char character = Text[CharIndex];
    v2 UV = V2( (character%16)/16.0f, (character/16)/16.0f );
    BufferTextUVs(TextGeo, UV);

    BufferColors(Group, TextGeo, default_palette[Color].xyz);

    TextGeo->CurrentIndex += 6;

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

  ++DebugState->FreeScopeCount;

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
BufferText(const char *Text, ui_render_group *Group, u32 ColorIndex)
{
  layout *Layout = Group->Layout;

  rect2 TextBox = BufferTextAt(Group, Text, ColorIndex);
  Layout->At.x = TextBox.Max.x;

  Layout->Clip.Min.x = Min(Layout->At.x, Layout->Clip.Min.x);
  Layout->Clip.Min.y = Min(Layout->At.y, Layout->Clip.Min.y);

  Layout->Clip.Max.x = Max(Layout->At.x, Layout->Clip.Max.x);
  Layout->Clip.Max.y = Max(Layout->At.y, Layout->Clip.Max.y);

  return;
}

inline void
BufferText(r32 Number, ui_render_group *Group, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%f", Number);
  BufferText(Buffer, Group, ColorIndex);
  return;
}

inline void
BufferText(u64 Number, ui_render_group *Group, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Number);
  BufferText(Buffer, Group, ColorIndex);
  return;
}

inline void
BufferText(u32 Number, ui_render_group *Group, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Number);
  BufferText(Buffer, Group, ColorIndex);
  return;
}

inline void
AdvanceSpaces(u32 N, layout *Layout)
{
  Layout->At.x += (N*Layout->FontSize);
  return;
}

inline void
NewLine(layout *Layout)
{
  Layout->At.y -= (Layout->LineHeight);
  Layout->At.x = 0;
  AdvanceSpaces(Layout->Depth, Layout);
  return;
}

static char Global_CharBuffer[32];

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

  sprintf(Global_CharBuffer, "%.1f%c", (r32)Display, Units);
  return Global_CharBuffer;
}

inline void
BufferMemorySize(u64 Number, ui_render_group *Group, u32 ColorIndex)
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

  char Buffer[32];
  sprintf(Buffer, "%.1f%c", (r32)Display, Units);
  BufferText( Buffer, Group, ColorIndex);

  return;
}

inline void
BufferThousands(u64 Number, ui_render_group *Group, u32 ColorIndex, u32 Columns)
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
    s32 Pad = Max(Columns-Len, 0);
    AdvanceSpaces(Pad, Group->Layout);
  }
  BufferText( Buffer, Group, ColorIndex);

  return;
}

inline void
BufferThousands(u64 Number, ui_render_group *Group, u32 ColorIndex)
{
  return BufferThousands(Number, Group, ColorIndex, 10);
}

inline void
BufferColumn( s32 Value, u32 ColumnWidth, ui_render_group *Group, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%d", Value);
  {
    s32 Len = strlen(Buffer);
    s32 Pad = Max(ColumnWidth-Len, 0);
    AdvanceSpaces(Pad, Group->Layout);
  }
  BufferText( Buffer, Group, ColorIndex);
  return;
}

inline void
BufferColumn( u32 Value, u32 ColumnWidth, ui_render_group *Group, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Value);
  {
    s32 Len = strlen(Buffer);
    s32 Pad = Max(ColumnWidth-Len, 0);
    AdvanceSpaces(Pad, Group->Layout);
  }
  BufferText( Buffer, Group, ColorIndex);
  return;
}

inline void
BufferColumn( u64 Value, u32 ColumnWidth, ui_render_group *Group, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Value);
  {
    s32 Len = strlen(Buffer);
    s32 Pad = Max(ColumnWidth-Len, 0);
    AdvanceSpaces(Pad, Group->Layout);
  }
  BufferText( Buffer, Group, ColorIndex);
  return;
}

inline void
BufferColumn( r32 Perc, u32 ColumnWidth, ui_render_group *Group, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%.1f", Perc);
  {
    s32 Len = strlen(Buffer);
    s32 Pad = Max(ColumnWidth-Len, 0);
    AdvanceSpaces(Pad, Group->Layout);
  }
  BufferText( Buffer, Group, ColorIndex);
  return;
}


#if 0
inline void
BufferNumberAsText(r32 Number, ui_render_group *Group, u32 ColorIndex)
{
  Group->Layout->At.x += Group->Layout->FontSize;
  BufferText(Number, Group, ColorIndex);
  Group->Layout->At.x += Group->Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(r64 Number, ui_render_group *Group, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferText((r32)Number, Group, ColorIndex);
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
#endif

inline void
BufferScopeTreeEntry(ui_render_group *Group, debug_profile_scope *Scope,
    u32 Color, u64 TotalCycles, u64 TotalFrameCycles, u64 CallCount, u32 Depth)
{
  Assert(TotalFrameCycles);
  r32 Percentage = 100.0f* SafeDivide0((r64)TotalCycles, (r64)TotalFrameCycles);
  u64 AvgCycles = SafeDivide0(TotalCycles, CallCount);
  BufferColumn(Percentage, 6, Group,  Color);
  BufferThousands(AvgCycles, Group, Color);
  BufferColumn(CallCount, 5, Group, Color);

  AdvanceSpaces(Depth*2.0f + 1, Group->Layout);

  if (Scope->Expanded && Scope->Child)
  {
    BufferText("-", Group, Color);
  }
  else if (Scope->Child)
  {
    BufferText("+", Group, Color);
  }
  else
  {
    AdvanceSpaces(1, Group->Layout);
  }

  BufferText(Scope->Name, Group, Color);

  NewLine(Group->Layout);
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
HoverAndClickExpand(ui_render_group *Group, registered_memory_arena *Arena, u32 Color, u32 HoverColor)
{
  u32 DrawColor = Color;

  {
    rect2 EntryBounds = GetNextLineBounds(Group->Layout);
    if ( IsInsideRect(EntryBounds, Group->MouseP) )
    {
      DrawColor = HoverColor;
      if (Group->Input->LMB.WasPressed)
        Arena->Expanded = !Arena->Expanded;
    }
  }

  return DrawColor;
}

u32
HoverAndClickExpand(ui_render_group *Group, debug_profile_scope *Scope, u32 Color, u32 HoverColor)
{
  u32 DrawColor = Color;

  {
    rect2 EntryBounds = GetNextLineBounds(Group->Layout);
    if ( IsInsideRect(EntryBounds, Group->MouseP) )
    {
      DrawColor = HoverColor;
      if (Group->Input->LMB.WasPressed)
        Scope->Expanded = !Scope->Expanded;
    }
  }

  return DrawColor;
}

void
BufferFirstCallToEach(ui_render_group *Group, debug_profile_scope *Scope, debug_state *State, u64 TotalFrameCycles, u32 Depth)
{
  if (!Scope) return;

  if (!Scope->Stats)
  {
    State->Memory->MemProtect = False;
    Scope->Stats = PUSH_STRUCT_CHECKED(scope_stats, State->Memory, 1);
    State->Memory->MemProtect = True;

    *Scope->Stats = GetStatsFor(State, Scope);
  }

  if (Scope->Stats->IsFirst)
  {
    u32 MainColor = HoverAndClickExpand(Group, Scope, WHITE, TEAL);

    BufferScopeTreeEntry(Group, Scope, MainColor, Scope->Stats->CumulativeCycles, TotalFrameCycles, Scope->Stats->Calls, Depth);

    if (Scope->Expanded)
      BufferFirstCallToEach(Group, Scope->Stats->MaxScope->Child, State, TotalFrameCycles, Depth+1);

  }

  BufferFirstCallToEach(Group, Scope->Sibling, State, TotalFrameCycles, Depth);

  return;
}

void
DebugFrameBegin(hotkeys *Hotkeys, r32 Dt, u64 Cycles)
{
  debug_state *State = GetDebugState();

  if ( Hotkeys->Debug_RedrawEveryPush )
  {
    State->Debug_RedrawEveryPush = !State->Debug_RedrawEveryPush;
  }

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
DebugDrawCallGraph(ui_render_group *Group, debug_state *DebugState, u32 MaxMs)
{
  layout *Layout = Group->Layout;
  v2 MouseP = Group->MouseP;

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

      v2 DrawDim = BufferQuad(Group, &Group->TextGroup->UIGeo, MinP, QuadDim);
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

      BufferColors(Group->TextGroup->UIGeo.Colors, Group->TextGroup->UIGeo.CurrentIndex, Color);

      Group->TextGroup->UIGeo.CurrentIndex+=6;
    }
  END_BLOCK("Frame Ticker");

  TIMED_BLOCK("Call Graph");

    SetFontSize(Layout, 30);
    debug_scope_tree *ReadTree = DebugState->GetReadScopeTree();
    { // Current ReadTree info
      BufferColumn(ReadTree->FrameMs, 4, Group, WHITE);
      BufferThousands(ReadTree->TotalCycles, Group, WHITE);
    }

    { // Call Graph
      PadBottom(Layout, 15);
      NewLine(Layout);
      BufferFirstCallToEach(Group, ReadTree->Root, DebugState, ReadTree->TotalCycles, 0);
    }
  END_BLOCK("Call Graph");
}

struct memory_arena_stats
{
  u64 Allocations;
  u64 Pushes;

  u64 TotalAllocated;
  u64 Remaining;
};

memory_arena_stats
GetMemoryArenaStats(memory_arena *Arena)
{
  TIMED_FUNCTION();
  memory_arena_stats Result = {};

  while (Arena)
  {
    Result.Allocations++;
    Result.Pushes += Arena->Pushes;
    Result.TotalAllocated += Arena->TotalSize;
    Result.Remaining += Arena->Remaining;

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
    registered_memory_arena *Current = &Global_RegisteredMemoryArenas[Index];
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
BufferBarGraph(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, layout *Layout, r32 Width, r32 PercFilled)
{
  r32 BarHeight = Layout->FontSize;
  r32 BarWidth = 200.0f;

  v2 MinP = Layout->At; // + V2(0, BarHeight);
  v2 BarDim = V2(BarWidth, BarHeight);
  v2 PercBarDim = V2(BarWidth, BarHeight) * V2(PercFilled, 1);

  v3 Color = {{ 1, 1, 0 }};

  BufferQuad(Group, Geo, MinP, BarDim);
  BufferColors(Group, Geo, V3(0.25f));
  Geo->CurrentIndex+=6;

  BufferQuad(Group, Geo, MinP, PercBarDim);
  BufferColors(Group, Geo, Color);
  Geo->CurrentIndex+=6;

  Layout->At.x += BarDim.x;

}

void
ColumnLeft(u32 Width, const char *Text, ui_render_group* Group, u32 ColorIndex )
{
  s32 Len = strlen(Text);
  s32 Pad = Max(Width-Len, 0);
  BufferText(Text, Group, ColorIndex);
  AdvanceSpaces(Pad, Group->Layout);
}

void
ColumnRight(u32 Width, const char *Text, ui_render_group* Group, u32 ColorIndex )
{
  s32 Len = strlen(Text);
  s32 Pad = Max(Width-Len, 0);
  AdvanceSpaces(Pad, Group->Layout);
  BufferText(Text, Group, ColorIndex);
}

inline void
BeginClipRect(layout *Layout)
{
  Layout->Clip = { Layout->At, {0,0} };
  return;
}

void
EndClipRect(ui_render_group *Group, layout *Layout, untextured_2d_geometry_buffer *Geo)
{
  v2 Dim = Layout->Clip.Max - Layout->Clip.Min;
  BufferQuad(Group, Geo, Layout->Clip.Min, Dim, 1.0f);
  BufferColors(Group, Geo, V3(0.2f));
  Geo->CurrentIndex+=6;
  return;
}

void
DebugDrawDrawCalls(ui_render_group *Group, debug_state *DebugState)
{
  layout *Layout = Group->Layout;
  NewLine(Layout);
  NewLine(Layout);

  for( u32 DrawCountIndex = 0;
       DrawCountIndex < Global_DrawCallArrayLength;
       ++ DrawCountIndex)
  {
     debug_draw_call *DrawCall = &Global_DrawCalls[DrawCountIndex];
     if (DrawCall->Caller)
     {
       BufferColumn(DrawCall->Count, 4, Group, WHITE);
       AdvanceSpaces(2, Layout);
       BufferText(DrawCall->Caller, Group, WHITE);
       NewLine(Layout);
     }
  }


}

void
DebugDrawMemoryHud(ui_render_group *Group, debug_state *DebugState)
{
  layout *Layout = Group->Layout;
  NewLine(Layout);

  /* BufferText("Free Scopes : ", Layout, RG, ViewportDim, WHITE); */
  /* BufferColumn(DebugState->FreeScopeCount, 4, Layout, RG, ViewportDim, WHITE); */
  /* NewLine(Layout); */

  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = &Global_RegisteredMemoryArenas[Index];
    if (!Current->Arena) continue;

    memory_arena_stats MemStats = GetMemoryArenaStats(Current->Arena);
    u64 TotalUsed = MemStats.TotalAllocated - MemStats.Remaining;

    BeginClipRect(Layout);

    {
      SetFontSize(Layout, 36);
      NewLine(Layout);
      u32 Color = HoverAndClickExpand(Group, Current, WHITE, TEAL);
      ColumnLeft(15, Current->Name, Group, Color);
      BufferText(MemorySize(MemStats.TotalAllocated), Group, Color);
    }

    ++Layout->Depth;
    NewLine(Layout);
    SetFontSize(Layout, 28);

    if (!Current->Expanded)
    {
      --Layout->Depth;
      continue;
    }


    {
      BufferText("Allocs", Group, WHITE);
      AdvanceSpaces(4, Layout);
      BufferText(MemStats.Allocations, Group, WHITE);
      NewLine(Layout);

      BufferText("Pushes", Group, WHITE);
      AdvanceSpaces(4, Layout);
      BufferText(MemStats.Pushes, Group, WHITE);
      NewLine(Layout);

      BufferText("Remaining", Group, WHITE);
      AdvanceSpaces(1, Layout);
      BufferMemorySize(MemStats.Remaining, Group, WHITE);
      NewLine(Layout);

      BufferText("Total", Group, WHITE);
      AdvanceSpaces(5, Layout);
      BufferMemorySize(MemStats.TotalAllocated, Group, WHITE);
    }

    {
      SetFontSize(Layout, 22);
      NewLine(Layout);
      r32 GraphWidth = 350.0f;

      r32 TotalPerc = SafeDivide0(TotalUsed, MemStats.TotalAllocated);

      NewLine(Layout);

      ColumnRight(6, MemorySize(TotalUsed), Group, WHITE);
      BufferBarGraph(Group, &Group->TextGroup->UIGeo, Layout, GraphWidth, TotalPerc);
      ColumnRight(6, MemorySize(MemStats.Remaining), Group, WHITE);

      NewLine(Layout);
      NewLine(Layout);

      memory_arena *CurrentArena = Current->Arena;
      while (CurrentArena)
      {
        u64 CurrentUsed = CurrentArena->TotalSize - CurrentArena->Remaining;
        r32 CurrentPerc = SafeDivide0(CurrentUsed, CurrentArena->TotalSize);

        ColumnRight(6, MemorySize(CurrentUsed), Group, WHITE);
        BufferBarGraph(Group, &Group->TextGroup->UIGeo, Layout, GraphWidth, CurrentPerc);
        ColumnRight(6, MemorySize(CurrentArena->Remaining), Group, WHITE);
        NewLine(Layout);

        CurrentArena = CurrentArena->Prev;
      }
    }

    --Layout->Depth;
    EndClipRect(Group, Layout, &Group->TextGroup->UIGeo);

    continue;
  }


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

  layout Layout = {};
  SetFontSize(&Layout, DEBUG_FONT_SIZE);

  ui_render_group Group = {};
  Group.Layout = &Layout;
  Group.TextGroup = RG;
  Group.ViewportDim = ViewportDim;
  Group.MouseP = MouseP;
  Group.Input = &Plat->Input;

  TIMED_BLOCK("Draw Status Bar");
    Layout.At.y = (r32)SCR_HEIGHT - Layout.FontSize;
    BufferColumn(Dt.Max, 6, &Group, WHITE);
    NewLine(&Layout);

    BufferColumn(Dt.Avg, 6, &Group, WHITE);
    BufferColumn(Plat->dt*1000.0f, 6, &Group, WHITE);
    BufferText("ms", &Group, WHITE);

    {
      // Main line
      memory_arena_stats TotalStats = GetTotalMemoryArenaStats();

      BufferThousands(TotalStats.Allocations, &Group, WHITE);
      AdvanceSpaces(1, &Layout);
      BufferText("Allocations", &Group, WHITE);

      BufferThousands(TotalStats.Pushes, &Group, WHITE);
      AdvanceSpaces(1, &Layout);
      BufferText("Pushes", &Group, WHITE);

      u32 TotalDrawCalls = 0;

      for( u32 DrawCountIndex = 0;
           DrawCountIndex < Global_DrawCallArrayLength;
           ++ DrawCountIndex)
      {
         TotalDrawCalls += Global_DrawCalls[DrawCountIndex].Count;
      }

      BufferColumn(TotalDrawCalls, 6, &Group, WHITE);
      AdvanceSpaces(1, &Layout);
      BufferText("Draw Calls", &Group, WHITE);

      NewLine(&Layout);
    }

    BufferColumn(Dt.Min, 6, &Group, WHITE);

  END_BLOCK("Status Bar");

  SetFontSize(&Layout, 32);
  NewLine(&Layout);
  NewLine(&Layout);

  switch (DebugState->UIType)
  {
    case DebugUIType_None:
    {
    } break;

    case DebugUIType_CallGraph:
    {
      BufferText("Call Graphs", &Group, WHITE);
      DebugDrawCallGraph(&Group, DebugState, Dt.Max);
    } break;

    case DebugUIType_MemoryHud:
    {
      BufferText("Memory Arenas", &Group, WHITE);
      DebugDrawMemoryHud(&Group, DebugState);
    } break;

    case DebugUIType_DrawCalls:
    {
      BufferText("Draw  Calls", &Group, WHITE);
      DebugDrawDrawCalls(&Group, DebugState);
    } break;

    InvalidDefaultCase;
  }

  FlushBuffer(RG, &RG->UIGeo, ViewportDim);
  FlushBuffer(RG, TextGeo, ViewportDim);

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

#endif // DEBUG
