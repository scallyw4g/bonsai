#if BONSAI_INTERNAL

#include <stdio.h>

debug_global b32 DebugGlobal_RedrawEveryPush = 0;

void
DebugRegisterArena(const char *Name, memory_arena *Arena, debug_state *DebugState)
{
  b32 Registered = False;
  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = &DebugState->RegisteredMemoryArenas[Index];

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
    Error("Too many arenas registered");
    Error("Registering Arena : %s", Name);
  }

  return;
}

b32
AreEqual(push_metadata *First, push_metadata *Second)
{
  b32 Result = (First->Arena == Second->Arena &&
                First->Name == Second->Name);
  return Result;
}

void
WritePushMetadata(push_metadata InputMeta)
{
  debug_state *DebugState = GetDebugState();

  u32 HashValue = (u32)(((u64)InputMeta.Name & (u64)InputMeta.Arena) % META_TABLE_SIZE);
  u32 FirstHashValue = HashValue;

  push_metadata *PickMeta = &DebugState->MetaTable[HashValue];
  while (PickMeta->Name)
  {
    if (AreEqual(PickMeta, &InputMeta))
    {
      PickMeta->PushCount++;
      return;
    }

    PickMeta = &DebugState->MetaTable[(++HashValue)%META_TABLE_SIZE];
    if (HashValue == FirstHashValue)
    {
      Error("DebugState->MetaTable is full");
    }
  }

  *PickMeta = InputMeta;
  PickMeta->PushCount++;

  return;
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

inline void*
PushStructChecked_(memory_arena *Arena, umm Size, const char* Name, s32 Line, const char* File)
{
  void* Result = PushStruct( Arena, Size );

#ifndef BONSAI_NO_PUSH_METADATA
  push_metadata Metadata = {Name, Arena, Size, 0};
  WritePushMetadata(Metadata);
#endif

  if (!Result)
  {
    Error("Pushing %s on Line: %d, in file %s", Name, Line, File);
    Assert(False);
    return False;
  }

  return Result;
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
FreeScopes(debug_state *DebugState, debug_profile_scope *ScopeToFree)
{
  if (!ScopeToFree) return;

  ++DebugState->FreeScopeCount;

  FreeScopes(DebugState, ScopeToFree->Child);
  FreeScopes(DebugState, ScopeToFree->Sibling);

  Clear(ScopeToFree);

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
AdvanceScopeTrees(debug_state *State)
{
  if (!State->DebugDoScopeProfiling) return;

  { // Advance to the next scope and reinitialize
    State->ReadScopeIndex = (State->ReadScopeIndex+1) % ROOT_SCOPE_COUNT;
    debug_scope_tree *WriteScope = State->GetWriteScopeTree();
    if (WriteScope)
    {
      FreeScopes(State, WriteScope->Root);
      InitScopeTree(State, WriteScope);
    }
  }

}

void
InitDebugState(platform *Plat, memory_arena *DebugMemory)
{
  GlobalDebugState = &Plat->DebugState;

  GlobalDebugState->Memory = DebugMemory;

  GlobalDebugState->FreeScopeSentinel.Parent = &GlobalDebugState->FreeScopeSentinel;
  GlobalDebugState->FreeScopeSentinel.Child = &GlobalDebugState->FreeScopeSentinel;

  AdvanceScopeTrees(GlobalDebugState);

  GlobalDebugState->Initialized = True;

  u32 BufferVertices = Kilobytes(1024);
  AllocateMesh(&GlobalDebugState->LineMesh, BufferVertices, DebugMemory);

  if (!InitDebugOverlayFramebuffer(&GlobalDebugState->TextRenderGroup, DebugMemory, "Holstein.DDS"))
  { Error("Initializing Debug Overlay Framebuffer"); }

  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup.TextGeo, 1024, DebugMemory);
  AllocateAndInitGeoBuffer(&GlobalDebugState->TextRenderGroup.UIGeo, 1024, DebugMemory);

  GlobalDebugState->TextRenderGroup.SolidUIShader = MakeSolidUIShader(GlobalDebugState->Memory);

  return;
}

void
UseShader(shader *Shader)
{
  glUseProgram(Shader->ID);
  BindShaderUniforms(Shader);
  return;
}

void
FlushBuffer(debug_text_render_group *RG, untextured_2d_geometry_buffer *Buffer)
{
  TIMED_FUNCTION();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(V2(SCR_WIDTH, SCR_HEIGHT));
  UseShader(&RG->SolidUIShader);

  u32 AttributeIndex = 0;
  BufferVertsToCard(RG->SolidUIVertexBuffer, Buffer, &AttributeIndex);
  BufferColorsToCard(RG->SolidUIColorBuffer, Buffer, &AttributeIndex);

  Draw(Buffer->CurrentIndex);
  Buffer->CurrentIndex = 0;

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  AssertNoGlErrors;

  return;
}

void
FlushBuffer(debug_text_render_group *RG, textured_2d_geometry_buffer *Geo)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(V2(SCR_WIDTH, SCR_HEIGHT));
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

  Draw(Geo->CurrentIndex);
  Geo->CurrentIndex = 0;

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
    FlushBuffer(Group->TextGroup, Geo);

  BufferColors(Geo->Colors, Geo->CurrentIndex, Color);
}

void
BufferColors(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v3 Color)
{
  if (Geo->CurrentIndex + 6 > Geo->Allocated)
    FlushBuffer(Group->TextGroup, Geo);

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
    FlushBuffer(Group->TextGroup, Geo);

  v2 Result = BufferQuadDirect(Geo->Verts, Geo->CurrentIndex, MinP, Dim, Z);
  return Result;
}

v2
BufferQuad(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z = 0.0f)
{
  if (Geo->CurrentIndex + 6 > Geo->Allocated)
    FlushBuffer(Group->TextGroup, Geo);

  v2 Result = BufferQuadDirect(Geo->Verts, Geo->CurrentIndex, MinP, Dim, Z);
  return Result;
}

r32
BufferTextAt(ui_render_group *Group, layout *Layout, const char *Text, u32 Color)
{
  textured_2d_geometry_buffer *TextGeo = &Group->TextGroup->TextGeo;

  s32 QuadCount = (s32)strlen(Text);

  r32 DeltaX = 0;

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = Layout->Basis + Layout->At + V2(Layout->FontSize*CharIndex, 0);
    v2 MaxP = BufferQuad(Group, TextGeo, MinP, V2(Layout->FontSize));

    char character = Text[CharIndex];
    v2 UV = V2( (character%16)/16.0f, (character/16)/16.0f );
    BufferTextUVs(TextGeo, UV);

    BufferColors(Group, TextGeo, getDefaultPalette()[Color].xyz);

    TextGeo->CurrentIndex += 6;

    DeltaX += (MaxP.x - MinP.x);

    continue;
  }

  return DeltaX;
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
AdvanceClip(layout *Layout)
{
  Layout->Clip.Min.x = Min(Layout->At.x, Layout->Clip.Min.x);
  Layout->Clip.Min.y = Min(Layout->At.y, Layout->Clip.Min.y);

  Layout->Clip.Max.x = Max(Layout->At.x, Layout->Clip.Max.x);
  Layout->Clip.Max.y = Max(Layout->At.y, Layout->Clip.Max.y);
}

inline void
BufferValue(const char *Text, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  r32 DeltaX = BufferTextAt(Group, Layout, Text, ColorIndex);
  Layout->At.x += DeltaX;

  AdvanceClip(Layout);

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

inline void
AdvanceSpaces(u32 N, layout *Layout)
{
  Layout->At.x += (N*Layout->FontSize);
  AdvanceClip(Layout);
  return;
}

inline void
NewLine(layout *Layout)
{
  Layout->At.y -= (Layout->LineHeight);
  Layout->At.x = 0;
  AdvanceSpaces(Layout->Depth, Layout);
  AdvanceClip(Layout);
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
BufferMemorySize(u64 Number, ui_render_group *Group, layout *Layout, u32 ColorIndex)
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
  BufferValue( Buffer, Group, Layout, ColorIndex);

  return;
}

inline void
BufferThousands(u64 Number, ui_render_group *Group, layout *Layout, u32 ColorIndex, u32 Columns)
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
    u32 Len = (u32)strlen(Buffer);
    u32 Pad = Max(Columns-Len, 0U);
    AdvanceSpaces(Pad, Layout);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex);

  return;
}

inline void
BufferThousands(u64 Number, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  return BufferThousands(Number, Group, Layout, ColorIndex, 10);
}

inline void
BufferColumn( s32 Value, u32 ColumnWidth, ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%d", Value);
  {
    u32 Len = (u32)strlen(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout);
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
    AdvanceSpaces(Pad, Layout);
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
    AdvanceSpaces(Pad, Layout);
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
    AdvanceSpaces(Pad, Layout);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex);
  return;
}


#if 0
inline void
BufferNumberAsText(r32 Number, ui_render_group *Group, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferValue(Number, Group, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(r64 Number, ui_render_group *Group, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferValue((r32)Number, Group, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(u64 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferValue(Number, Layout, RG, ViewportDim, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(u32 Number, layout *Layout, debug_text_render_group *RG, v2 ViewportDim, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferValue(Number, Layout, RG, ViewportDim, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}
#endif

inline void
BufferScopeTreeEntry(ui_render_group *Group, debug_profile_scope *Scope,
    u32 Color, u64 TotalCycles, u64 TotalFrameCycles, u64 CallCount, u32 Depth)
{
  Assert(TotalFrameCycles);
  r32 Percentage = 100.0f * (r32)SafeDivide0((r64)TotalCycles, (r64)TotalFrameCycles);
  u64 AvgCycles = (u64)SafeDivide0(TotalCycles, CallCount);

  layout *Layout = Group->Layout;

  BufferColumn(Percentage, 6, Group, Layout,  Color);
  BufferThousands(AvgCycles, Group, Layout, Color);
  BufferColumn(CallCount, 5, Group, Layout, Color);

  AdvanceSpaces((Depth*2)+1, Layout);

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
    AdvanceSpaces(1, Layout);
  }

  BufferValue(Scope->Name, Group, Layout, Color);

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

template <typename T> u32
HoverAndClickExpand(ui_render_group *Group, layout *Layout, T *Expandable, u32 Color, u32 HoverColor)
{
  u32 DrawColor = Color;

  {
    rect2 EntryBounds = GetNextLineBounds(Layout);
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
BufferFirstCallToEach(ui_render_group *Group, debug_profile_scope *Scope, debug_state *State, u64 TotalFrameCycles, u32 Depth)
{
  if (!Scope) return;

  if (!Scope->Stats)
  {
#if MEMPROTECT
    State->Memory->MemProtect = False;
#endif
    Scope->Stats = PUSH_STRUCT_CHECKED(scope_stats, State->Memory, 1);
#if MEMPROTECT
    State->Memory->MemProtect = True;
#endif

    *Scope->Stats = GetStatsFor(State, Scope);
  }

  layout *Layout = Group->Layout;

  if (Scope->Stats->IsFirst)
  {
    u32 MainColor = HoverAndClickExpand(Group, Layout, Scope, WHITE, TEAL);

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
    State->DebugDoScopeProfiling = !State->DebugDoScopeProfiling;
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

  AdvanceScopeTrees(State);

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
DebugDrawCallGraph(ui_render_group *Group, debug_state *DebugState, r32 MaxMs)
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
      BufferColumn(ReadTree->FrameMs, 4, Group, Layout, WHITE);
      BufferThousands(ReadTree->TotalCycles, Group, Layout, WHITE);
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

template <typename T> b32
AreEqual(T First, T Second)
{
  b32 Result = True;
  umm TypeSize = sizeof(T);

  u8* FirstPtr = (u8*)&First;
  u8* SecondPtr = (u8*)&Second;

  for (umm Index = 0;
      Index < TypeSize;
      ++Index)
  {
    Result = Result && ( FirstPtr[Index] == SecondPtr[Index]);
  }

  return Result;
}

void
DebugPrintArenaStats(memory_arena *Arena)
{
  Print( Remaining(Arena) );
  Print( TotalSize(Arena) );
  Print( Arena->Pushes );
  Debug("");
}

void
DebugPrintMemStats(memory_arena_stats *Stats)
{
  Print(Stats->Allocations);
  Print(Stats->Pushes);
  Print(Stats->TotalAllocated);
  Print(Stats->Remaining);

  return;
}

memory_arena_stats
GetMemoryArenaStats(memory_arena *Arena)
{
  memory_arena_stats Result = {};

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
BufferBarGraph(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, layout *Layout, r32 PercFilled)
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
ColumnLeft(u32 Width, const char *Text, ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  u32 Len = (u32)strlen(Text);
  u32 Pad = Max(Width-Len, 0U);
  BufferValue(Text, Group, Layout, ColorIndex);
  AdvanceSpaces(Pad, Layout);
}

void
ColumnRight(u32 Width, const char *Text, ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  u32 Len = (u32)strlen(Text);
  u32 Pad = Max(Width-Len, 0U);
  AdvanceSpaces(Pad, Layout);
  BufferValue(Text, Group, Layout, ColorIndex);
}

inline void
BeginClipRect(layout *Layout)
{
  Layout->Clip = { Layout->At, Layout->At };
  return;
}

void
EndClipRect(ui_render_group *Group, layout *Layout, untextured_2d_geometry_buffer *Geo, v2 Basis = V2(0,0))
{
  v2 MinP = Layout->Clip.Min + Basis;
  v2 Dim = (Layout->Clip.Max + Basis) - MinP;

  BufferQuad(Group, Geo, MinP, Dim, 1.0f);
  BufferColors(Group, Geo, V3(0.2f));
  Geo->CurrentIndex+=6;
  return;
}

void
DebugDrawDrawCalls(ui_render_group *Group)
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
       BufferColumn(DrawCall->Count, 4, Group, Layout, WHITE);
       AdvanceSpaces(2, Layout);
       BufferValue(DrawCall->Caller, Group, Layout, WHITE);
       NewLine(Layout);
     }
  }


}

void
BufferDebugPushMetaData(ui_render_group *Group, /* debug_state *State, */ registered_memory_arena *Current, layout *Layout)
{
  BufferValue(Current->Name, Group, Layout, WHITE);
  NewLine(Layout);

  memory_arena *Arena = Current->Arena;
  for ( u32 MetaIndex = 0;
      MetaIndex < META_TABLE_SIZE;
      ++MetaIndex)
  {
    push_metadata *Meta = &GetDebugState()->MetaTable[MetaIndex];
    if (Meta->Arena == Arena)
    {
      BufferThousands(Meta->PushCount, Group, Layout, WHITE, 8);
      AdvanceSpaces(1, Layout);
      BufferValue(Meta->Name, Group, Layout, WHITE);
      NewLine(Layout);
    }
  }

}

void
DebugDrawMemoryHud(ui_render_group *Group, debug_state *DebugState)
{
  layout *Layout = Group->Layout;
  NewLine(Layout);

  /* BufferValue("Free Scopes : ", Layout, RG, ViewportDim, WHITE); */
  /* BufferColumn(DebugState->FreeScopeCount, 4, Layout, RG, ViewportDim, WHITE); */
  /* NewLine(Layout); */

  for ( u32 Index = 0;
        Index < REGISTERED_MEMORY_ARENA_COUNT;
        ++Index )
  {
    registered_memory_arena *Current = &DebugState->RegisteredMemoryArenas[Index];
    if (!Current->Arena) continue;

    memory_arena_stats MemStats = GetMemoryArenaStats(Current->Arena);
    u64 TotalUsed = MemStats.TotalAllocated - MemStats.Remaining;

    BeginClipRect(Layout);

    {
      SetFontSize(Layout, 36);
      NewLine(Layout);
      u32 Color = HoverAndClickExpand(Group, Layout, Current, WHITE, TEAL);
      ColumnLeft(15, Current->Name, Group, Layout, Color);
      BufferValue(MemorySize(MemStats.TotalAllocated), Group, Layout, Color);
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
      BufferValue("Allocs", Group, Layout, WHITE);
      AdvanceSpaces(4, Layout);
      BufferValue(MemStats.Allocations, Group, Layout, WHITE);
      NewLine(Layout);

      BufferValue("Pushes", Group, Layout, WHITE);
      AdvanceSpaces(4, Layout);
      BufferValue(MemStats.Pushes, Group, Layout, WHITE);
      NewLine(Layout);

      BufferValue("Remaining", Group, Layout, WHITE);
      AdvanceSpaces(1, Layout);
      BufferMemorySize(MemStats.Remaining, Group, Layout, WHITE);
      NewLine(Layout);

      BufferValue("Total", Group, Layout, WHITE);
      AdvanceSpaces(5, Layout);
      BufferMemorySize(MemStats.TotalAllocated, Group, Layout, WHITE);
    }

    {
      SetFontSize(Layout, 22);
      NewLine(Layout);

      r32 TotalPerc = (r32)SafeDivide0(TotalUsed, MemStats.TotalAllocated);

      NewLine(Layout);

      ColumnRight(6, MemorySize(TotalUsed), Group, Layout, WHITE);
      BufferBarGraph(Group, &Group->TextGroup->UIGeo, Layout, TotalPerc);
      ColumnRight(6, MemorySize(MemStats.Remaining), Group, Layout, WHITE);

      NewLine(Layout);
      NewLine(Layout);

      memory_arena *CurrentArena = Current->Arena;
      while (CurrentArena)
      {
        u64 CurrentUsed = TotalSize(CurrentArena) - Remaining(CurrentArena);
        r32 CurrentPerc = (r32)SafeDivide0(CurrentUsed, TotalSize(CurrentArena));

        ColumnRight(6, MemorySize(CurrentUsed), Group, Layout, WHITE);
        BufferBarGraph(Group, &Group->TextGroup->UIGeo, Layout, CurrentPerc);
        ColumnRight(6, MemorySize(Remaining(CurrentArena)), Group, Layout, WHITE);
        NewLine(Layout);

        CurrentArena = CurrentArena->Prev;
      }
    }

    --Layout->Depth;

    EndClipRect(Group, Layout, &Group->TextGroup->UIGeo);

    layout MetadataLayout = *Layout;
    MetadataLayout.Basis = { Layout->Clip.Max.x, Layout->Clip.Min.y };

    BeginClipRect(&MetadataLayout);

    BufferDebugPushMetaData(Group, Current, &MetadataLayout);

    EndClipRect(Group, &MetadataLayout, &Group->TextGroup->UIGeo, MetadataLayout.Basis);

    continue;
  }


  return;
}

void
DebugDrawNetworkHud(ui_render_group *Group,
    network_connection *Network,
    server_state *ServerState)
{
  layout *Layout = Group->Layout;

  BufferValue("Network", Group, Layout, WHITE);
  AdvanceSpaces(2, Layout);

  if (IsConnected(Network))
  {
    BufferValue("O", Group, Layout, GREEN);

    AdvanceSpaces(2, Layout);

    if (Network->Client)
    {
      BufferValue("ClientId", Group, Layout, WHITE);
      BufferColumn( Network->Client->Id, 2, Group, Layout, WHITE);
    }

    NewLine(Layout);
    NewLine(Layout);

    NewLine(Layout);

    for (s32 ClientIndex = 0;
        ClientIndex < MAX_CLIENTS;
        ++ClientIndex)
    {
      client_state *Client = &ServerState->Clients[ClientIndex];

      u32 Color = WHITE;

      if (Network->Client->Id == ClientIndex)
        Color = GREEN;

      AdvanceSpaces(1, Layout);
      BufferValue("Id:", Group, Layout, WHITE);
      BufferColumn( Client->Id, 2, Group, Layout, WHITE);
      AdvanceSpaces(2, Layout);
      BufferColumn(Client->Counter, 7, Group, Layout, Color);
      NewLine(Layout);
    }

  }
  else
  {
    BufferValue("X", Group, Layout, RED);
    NewLine(Layout);
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

void
PrintScopeTree(debug_profile_scope *Scope, s32 Depth = 0)
{
  if (!Scope)
    return;

  s32 CurDepth = Depth;

  while (CurDepth--)
  {
    printf("%s", "  ");
  }

  if (Depth > 0)
    printf("%s", " `- ");

  printf("%d %s", Depth, Scope->Name);

  debug_state *DebugState = GetDebugState();
  if (DebugState->WriteScope == &Scope->Child)
    printf(" %s", "<-- Child \n");
  else if (DebugState->WriteScope == &Scope->Sibling)
    printf(" %s", "<-- Sibling \n");
  else
    printf("%s", "\n");


  PrintScopeTree(Scope->Child, Depth+1);
  PrintScopeTree(Scope->Sibling, Depth);

  return;
}

void
DebugDrawGraphicsHud(ui_render_group *Group, debug_state *DebugState)
{
  layout *Layout = Group->Layout;
  BufferValue("Graphics", Group, Layout, WHITE);

  NewLine(Layout);
  NewLine(Layout);

  BufferMemorySize(DebugState->BytesBufferedToCard, Group, Layout, WHITE);

  return;
}

void
DebugFrameEnd(platform *Plat, game_state *GameState)
{
  TIMED_FUNCTION();
  debug_state *DebugState = GetDebugState();
  debug_text_render_group *RG = &DebugState->TextRenderGroup;
  textured_2d_geometry_buffer *TextGeo = &RG->TextGeo;

  /* v2 ViewportDim = V2(Plat->WindowWidth, Plat->WindowHeight); */
  v2 MouseP = V2(Plat->MouseP.x, Plat->WindowHeight - Plat->MouseP.y);

  min_max_avg_dt Dt = ComputeMinMaxAvgDt(DebugState->ScopeTrees);

  layout Layout = {};
  SetFontSize(&Layout, DEBUG_FONT_SIZE);

  ui_render_group Group = {};
  Group.Layout = &Layout;
  Group.TextGroup = RG;
  /* Group.ViewportDim = ViewportDim; */
  Group.MouseP = MouseP;
  Group.Input = &Plat->Input;

  TIMED_BLOCK("Draw Status Bar");
    Layout.At.y = (r32)SCR_HEIGHT - Layout.FontSize;
    BufferColumn(Dt.Max, 6, &Group, &Layout, WHITE);
    NewLine(&Layout);

    BufferColumn(Dt.Avg, 6, &Group, &Layout, WHITE);
    BufferColumn(Plat->dt*1000.0f, 6, &Group, &Layout, WHITE);
    BufferValue("ms", &Group, &Layout, WHITE);

    {
      // Main line
      memory_arena_stats TotalStats = GetTotalMemoryArenaStats();

      BufferThousands(TotalStats.Allocations, &Group, &Layout, WHITE);
      AdvanceSpaces(1, &Layout);
      BufferValue("Allocations", &Group, &Layout, WHITE);

      BufferThousands(TotalStats.Pushes, &Group, &Layout, WHITE);
      AdvanceSpaces(1, &Layout);
      BufferValue("Pushes", &Group, &Layout, WHITE);

      u32 TotalDrawCalls = 0;

      for( u32 DrawCountIndex = 0;
           DrawCountIndex < Global_DrawCallArrayLength;
           ++ DrawCountIndex)
      {
         TotalDrawCalls += Global_DrawCalls[DrawCountIndex].Count;
      }

      BufferColumn(TotalDrawCalls, 6, &Group, &Layout, WHITE);
      AdvanceSpaces(1, &Layout);
      BufferValue("Draw Calls", &Group, &Layout, WHITE);

      NewLine(&Layout);
    }

    BufferColumn(Dt.Min, 6, &Group, &Layout, WHITE);

  END_BLOCK("Status Bar");

  SetFontSize(&Layout, 32);
  NewLine(&Layout);
  NewLine(&Layout);

  switch (DebugState->UIType)
  {
    case DebugUIType_None:
    {
    } break;

    case DebugUIType_Graphics:
    {
      DebugDrawGraphicsHud(&Group, DebugState);
    } break;

    case DebugUIType_Network:
    {
      DebugDrawNetworkHud(&Group, &Plat->Network, GameState->ServerState);
    } break;

    case DebugUIType_CallGraph:
    {
      BufferValue("Call Graphs", &Group, &Layout, WHITE);
      DebugDrawCallGraph(&Group, DebugState, Dt.Max);
    } break;

    case DebugUIType_Memory:
    {
      BufferValue("Memory Arenas", &Group, &Layout, WHITE);
      DebugDrawMemoryHud(&Group, DebugState);
    } break;

    case DebugUIType_DrawCalls:
    {
      BufferValue("Draw  Calls", &Group, &Layout, WHITE);
      DebugDrawDrawCalls(&Group);
    } break;

    InvalidDefaultCase;
  }

  FlushBuffer(RG, &RG->UIGeo);
  FlushBuffer(RG, TextGeo);

  DebugState->BytesBufferedToCard = 0;

  return;
}

#endif // DEBUG
