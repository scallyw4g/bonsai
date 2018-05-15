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
PushesMatch(push_metadata *First, push_metadata *Second)
{
  b32 Result = (First->Arena == Second->Arena &&
                First->StructSize == Second->StructSize &&
                First->StructCount == Second->StructCount &&
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
    if (PushesMatch(PickMeta, &InputMeta))
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
PushStructChecked_(memory_arena *Arena, umm StructSize, umm StructCount, const char* Name, s32 Line, const char* File)
{
  umm PushSize = StructCount * StructSize;

  void* Result = PushStruct( Arena, PushSize );

#ifndef BONSAI_NO_PUSH_METADATA
  push_metadata Metadata = {Name, Arena, StructSize, StructCount, 0};
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

  Geo->End = VertCount + 1;
  Geo->At = 0;
}

void
AllocateAndInitGeoBuffer(untextured_2d_geometry_buffer *Geo, u32 VertCount, memory_arena *DebugArena)
{
  Geo->Verts = PUSH_STRUCT_CHECKED(v3, DebugArena, VertCount);
  Geo->Colors = PUSH_STRUCT_CHECKED(v3, DebugArena, VertCount);

  Geo->End = VertCount + 1;
  Geo->At = 0;
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

  AllocateMesh(&GlobalDebugState->LineMesh, 1024, DebugMemory);

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
  v3 InvertY = V3(1.0f, -1.0f, 1.0f);

  Dest[StartingIndex++] = InvertY * ((vertex_up_left    * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertY * ((vertex_down_left  * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertY * ((vertex_up_right   * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertY * ((vertex_down_right * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertY * ((vertex_up_right   * ToClipSpace) * 2.0f - 1);
  Dest[StartingIndex++] = InvertY * ((vertex_down_left  * ToClipSpace) * 2.0f - 1);

  v2 Max = vertex_down_right.xy;
  return Max;
}

v2
BufferQuad(ui_render_group *Group, textured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z = 0.0f)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  v2 Result = BufferQuadDirect(Geo->Verts, Geo->At, MinP, Dim, Z, Group->ScreenDim);
  return Result;
}

v2
BufferQuad(ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z = 0.0f)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  v2 Result = BufferQuadDirect(Geo->Verts, Geo->At, MinP, Dim, Z, Group->ScreenDim);
  return Result;
}

r32
BufferTextAt(ui_render_group *Group, layout *Layout, font *Font, const char *Text, u32 Color)
{
  textured_2d_geometry_buffer *TextGeo = &Group->TextGroup->TextGeo;

  s32 QuadCount = (s32)strlen(Text);

  r32 DeltaX = 0;

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = Layout->Basis + Layout->At + V2(Font->Size*CharIndex, 0);
    v2 MaxP = BufferQuad(Group, TextGeo, MinP, V2(Font->Size));

    char character = Text[CharIndex];
    v2 UV = V2( (character%16)/16.0f, (character/16)/16.0f );
    BufferTextUVs(TextGeo, UV);

    BufferColors(Group, TextGeo, getDefaultPalette()[Color].xyz);

    TextGeo->At += 6;

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
  r32 DeltaX = BufferTextAt(Group, Layout, &Group->Font, Text, ColorIndex);
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


  char *Buffer = PUSH_STRUCT_CHECKED(char, TranArena, 32);
  sprintf(Buffer, "%.1f%c", (r32)Display, Units);
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
  char *Buffer = PUSH_STRUCT_CHECKED(char, TranArena, Megabytes(1));

  for (u32 Index = 0;
      Index < Megabytes(1);
      ++Index)
  {
    Buffer[Index] = 0;
  }

#endif

  char *Buffer = PUSH_STRUCT_CHECKED(char, TranArena, 32);
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

  char *Buffer = PUSH_STRUCT_CHECKED(char, TranArena, 32);
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
BufferNumberAsText(u64 Number, layout *Layout, debug_text_render_group *RG, v2 ScreenDim, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferValue(Number, Layout, RG, ScreenDim, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}

inline void
BufferNumberAsText(u32 Number, layout *Layout, debug_text_render_group *RG, v2 ScreenDim, u32 ColorIndex)
{
  Layout->At.x += Layout->FontSize;
  BufferValue(Number, Layout, RG, ScreenDim, ColorIndex);
  Layout->At.x += Layout->FontSize;
  return;
}
#endif

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
  v2 StartingP = Layout->At;

  // FIXME(Jesse): Should line length be systemized somehow?
  v2 EndingP = Layout->At + V2(100000.0f, Font->LineHeight);
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
BufferFirstCallToEach(ui_render_group *Group, debug_profile_scope *Scope, debug_state *State, layout *Layout, u64 TotalFrameCycles, u32 Depth)
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

  if (Scope->Stats->IsFirst)
  {
    u32 MainColor = HoverAndClickExpand(Group, Layout, Scope, WHITE, TEAL);

    BufferScopeTreeEntry(Group, Scope, Layout, MainColor, Scope->Stats->CumulativeCycles, TotalFrameCycles, Scope->Stats->Calls, Depth);

    if (Scope->Expanded)
      BufferFirstCallToEach(Group, Scope->Stats->MaxScope->Child, State, Layout, TotalFrameCycles, Depth+1);

  }

  BufferFirstCallToEach(Group, Scope->Sibling, State, Layout, TotalFrameCycles, Depth);

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
DebugDrawCallGraph(ui_render_group *Group, debug_state *DebugState, layout *Layout, r32 MaxMs)
{
  v2 MouseP = Group->MouseP;

  NewLine(Layout, &Group->Font);
  SetFontSize(&Group->Font, 80);

  TIMED_BLOCK("Frame Ticker");
    v2 StartingAt = Layout->At;

    for (u32 TreeIndex = 0;
        TreeIndex < ROOT_SCOPE_COUNT;
        ++TreeIndex )
    {
      debug_scope_tree *Tree = &DebugState->ScopeTrees[TreeIndex];

      r32 Perc = SafeDivide0(Tree->FrameMs, MaxMs);

      v2 MinP = Layout->At;
      v2 MaxDim = V2(15.0, Group->Font.Size);
      v2 QuadDim = V2(15.0, (Group->Font.Size) * Perc);
      v2 Offset = MaxDim - QuadDim;

      v2 DrawDim = BufferQuad(Group, &Group->TextGroup->UIGeo, MinP + Offset, QuadDim);
      Layout->At.x = DrawDim.x + 5.0f;

      v3 Color = V3(0.5f, 0.5f, 0.5f);
      if ( Tree == DebugState->GetWriteScopeTree() )
        Color = V3(0.8f, 0.0f, 0.0f);

      if ( Tree == DebugState->GetReadScopeTree() )
        Color = V3(0.8f, 0.8f, 0.0f);

      if (MouseP > MinP && MouseP < DrawDim)
      {
        DebugState->ReadScopeIndex = TreeIndex;
        Color = V3(0.8f, 0.8f, 0.0f);
      }

      BufferColors(Group->TextGroup->UIGeo.Colors, Group->TextGroup->UIGeo.At, Color);

      Group->TextGroup->UIGeo.At+=6;
    }


    r32 MaxBarHeight = Group->Font.Size;
    v2 QuadDim = V2(Layout->At.x, 2.0f);
    {
      r32 MsPerc = SafeDivide0(33.333f, MaxMs);
      r32 MinPOffset = MaxBarHeight * MsPerc;
      v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size - MinPOffset };

      BufferQuad(Group, &Group->TextGroup->UIGeo, MinP, QuadDim, 1.0f);
      BufferColors(Group->TextGroup->UIGeo.Colors, Group->TextGroup->UIGeo.At, V3(1,1,0));
      Group->TextGroup->UIGeo.At+=6;
    }

    {
      r32 MsPerc = SafeDivide0(16.666f, MaxMs);
      r32 MinPOffset = MaxBarHeight * MsPerc;
      v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size - MinPOffset };

      BufferQuad(Group, &Group->TextGroup->UIGeo, MinP, QuadDim, 1.0f);
      BufferColors(Group->TextGroup->UIGeo.Colors, Group->TextGroup->UIGeo.At, V3(0,1,0));
      Group->TextGroup->UIGeo.At+=6;
    }

    { // Current ReadTree info
      SetFontSize(&Group->Font, 30);
      debug_scope_tree *ReadTree = DebugState->GetReadScopeTree();
      BufferColumn(ReadTree->FrameMs, 4, Group, Layout, WHITE);
      BufferThousands(ReadTree->TotalCycles, Group, Layout, WHITE);
    }
    NewLine(Layout, &Group->Font);

  END_BLOCK("Frame Ticker");

  TIMED_BLOCK("Call Graph");
    PadBottom(Layout, 15);
    NewLine(Layout, &Group->Font);
    debug_scope_tree *ReadTree = DebugState->GetReadScopeTree();
    BufferFirstCallToEach(Group, ReadTree->Root, DebugState, Layout, ReadTree->TotalCycles, 0);
  END_BLOCK("Call Graph");
}

struct memory_arena_stats
{
  u64 Allocations;
  u64 Pushes;

  u64 TotalAllocated;
  u64 Remaining;
};

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
  r32 BarHeight = Group->Font.Size;
  r32 BarWidth = 200.0f;

  v2 MinP = Layout->At; // + V2(0, BarHeight);
  v2 BarDim = V2(BarWidth, BarHeight);
  v2 PercBarDim = V2(BarWidth, BarHeight) * V2(PercFilled, 1);

  v3 Color = {{ 1, 1, 0 }};

  BufferQuad(Group, Geo, MinP, BarDim);
  BufferColors(Group, Geo, V3(0.25f));
  Geo->At+=6;

  BufferQuad(Group, Geo, MinP, PercBarDim);
  BufferColors(Group, Geo, Color);
  Geo->At+=6;

  Layout->At.x += BarDim.x;

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

inline void
BeginClipRect(layout *Layout)
{
  Layout->Clip = {V2(FLT_MAX, FLT_MAX), V2(-FLT_MAX, -FLT_MAX)};
  return;
}

void
EndClipRect(ui_render_group *Group, layout *Layout, untextured_2d_geometry_buffer *Geo, v2 Basis = V2(0,0))
{

  v2 MinP = Layout->Clip.Min + Basis;
  v2 Dim = (Layout->Clip.Max + Basis) - MinP;

  BufferQuad(Group, Geo, MinP, Dim, 1.0f);
  BufferColors(Group, Geo, V3(0.2f));
  Geo->At+=6;

  return;
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

layout *
BufferDebugPushMetaData(ui_render_group *Group, registered_memory_arena *Current, v2 Basis)
{
  table_layout *Table = &Current->MetadataTable;
  layout *Layout = &Table->Layout;
  Clear(Layout);

  Layout->Basis = Basis;
  SetFontSize(&Group->Font, 24);

  BeginClipRect(Layout);

  Column("Size", Group, Table, WHITE);
  Column("Structs", Group, Table, WHITE);
  Column("Push Count", Group, Table, WHITE);
  Column("Name", Group, Table, WHITE);
  NewLine(Layout, &Group->Font);

  memory_arena *Arena = Current->Arena;
  for ( u32 MetaIndex = 0;
      MetaIndex < META_TABLE_SIZE;
      ++MetaIndex)
  {
    push_metadata *Meta = &GetDebugState()->MetaTable[MetaIndex];
    if (Meta->Arena == Arena)
    {

      umm AllocationSize = Meta->StructSize*Meta->StructCount*Meta->PushCount;
      Column( FormatMemorySize(AllocationSize), Group, Table, WHITE);
      Column( FormatThousands(Meta->StructCount), Group, Table, WHITE);
      Column( FormatThousands(Meta->PushCount), Group, Table, WHITE);
      Column(Meta->Name, Group, Table, WHITE);
      NewLine(Layout, &Group->Font);
    }
  }

  NewLine(Layout, &Group->Font);
  EndClipRect(Group, Layout, &Group->TextGroup->UIGeo, Layout->Basis);

  return Layout;
}

void
BufferArenaBargraph(table_layout *BargraphTable, ui_render_group *Group, umm TotalUsed, r32 TotalPerc, umm Remaining)
{
  Column( FormatMemorySize(TotalUsed), Group, BargraphTable, WHITE);
  BufferBarGraph(Group, &Group->TextGroup->UIGeo, &BargraphTable->Layout, TotalPerc);
  Column( FormatMemorySize(Remaining), Group, BargraphTable, WHITE);
  NewRow(BargraphTable, &Group->Font);

  return;
}

void
DebugDrawMemoryHud(ui_render_group *Group, debug_state *DebugState, layout *BasisLayout)
{
  local_persist table_layout MemoryHudTable = {};

  MemoryHudTable.Layout = *BasisLayout;

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
      NewLine(&MemoryHudTable.Layout, &Group->Font);
      u8 Color = HoverAndClickExpand(Group, &MemoryHudTable.Layout, Current, WHITE, TEAL);

      Column(Current->Name, Group, &MemoryHudTable, Color);
      Column(MemorySize(MemStats.TotalAllocated), Group, &MemoryHudTable, Color);
      NewRow(&MemoryHudTable, &Group->Font);
    }


    BeginClipRect(&MemoryHudTable.Layout);

    ++MemoryHudTable.Layout.Depth;
    SetFontSize(&Group->Font, 28);


    if (!Current->Expanded)
    {
      --MemoryHudTable.Layout.Depth;
      continue;
    }


    {
      table_layout *StatsTable = &Current->StatsTable;
      StatsTable->Layout = MemoryHudTable.Layout;

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

      MemoryHudTable.Layout = StatsTable->Layout;
    }

    {
      table_layout *BargraphTable = &Current->BargraphTable;
      BargraphTable->Layout = MemoryHudTable.Layout;
      SetFontSize(&Group->Font, 22);

      NewRow(BargraphTable, &Group->Font);

      r32 TotalPerc = (r32)SafeDivide0(TotalUsed, MemStats.TotalAllocated);
      BufferArenaBargraph(BargraphTable, Group, TotalUsed, TotalPerc, MemStats.Remaining);
      NewRow(BargraphTable, &Group->Font);

      memory_arena *CurrentArena = Current->Arena;
      while (CurrentArena)
      {
        u64 CurrentUsed = TotalSize(CurrentArena) - Remaining(CurrentArena);
        r32 CurrentPerc = (r32)SafeDivide0(CurrentUsed, TotalSize(CurrentArena));

        BufferArenaBargraph(BargraphTable, Group, CurrentUsed, CurrentPerc, Remaining(CurrentArena));

        CurrentArena = CurrentArena->Prev;
      }

      MemoryHudTable.Layout = BargraphTable->Layout;
    }

    --MemoryHudTable.Layout.Depth;

    v2 BasisP =  { MemoryHudTable.Layout.Clip.Max.x + 100.0f, MemoryHudTable.Layout.Clip.Min.y};

    layout *DebugMetaLayout = BufferDebugPushMetaData(Group, Current, BasisP);
    MemoryHudTable.Layout.At = Max(MemoryHudTable.Layout.Clip.Max, BasisP + DebugMetaLayout->Clip.Max);
    AdvanceClip(&MemoryHudTable.Layout);

    EndClipRect(Group, &MemoryHudTable.Layout, &Group->TextGroup->UIGeo);

    continue;
  }


  return;
}

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
DebugDrawGraphicsHud(ui_render_group *Group, debug_state *DebugState, layout *Layout)
{
  BufferValue("Graphics", Group, Layout, WHITE);

  NewLine(Layout, &Group->Font);
  NewLine(Layout, &Group->Font);

  BufferMemorySize(DebugState->BytesBufferedToCard, Group, Layout, WHITE);

  return;
}

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
    Dt = ComputeMinMaxAvgDt(DebugState->ScopeTrees);
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
    } break;

    case DebugUIType_Memory:
    {
      BufferValue("Memory Arenas", &Group, &Layout, WHITE);
      NewLine(&Layout, &Group.Font);
      DebugDrawMemoryHud(&Group, DebugState, &Layout);
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

  return;
}

#endif // DEBUG
