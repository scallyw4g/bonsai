#include <texture.cpp>
#include <stream.cpp>
#include <shader.cpp>
#include <render_init.cpp>

#include <render.h>
#include <render_utils.cpp>
#include <bonsai_mesh.cpp>
#include <gpu_mapped_buffer.cpp>

debug_global clip_rect NullClipRect = {};

#if 0
void
CleanupText2D(debug_text_render_group *RG)
{
  // Delete buffers
  glDeleteBuffers(1, &RG->VertexBuffer);
  glDeleteBuffers(1, &RG->UVBuffer);

  // Delete texture
  glDeleteTextures(1, &RG->FontTexture->ID);

  // Delete shader
  glDeleteProgram(RG->Text2DShader.ID);

  return;
}
#endif

b32
InitDebugOverlayFramebuffer(debug_text_render_group *RG, memory_arena *DebugArena, const char *DebugFont)
{
  RG->FontTexture = LoadBitmap(DebugFont, DebugArena, DebugTextureArraySlice_Count);

  glGenBuffers(1, &RG->SolidUIVertexBuffer);
  glGenBuffers(1, &RG->SolidUIColorBuffer);
  glGenBuffers(1, &RG->SolidUIUVBuffer);

  RG->Text2DShader = LoadShaders("TextVertexShader.vertexshader",
                                 "TextVertexShader.fragmentshader", DebugArena);

  RG->TextTextureUniform = glGetUniformLocation(RG->Text2DShader.ID, "TextTextureSampler");

  return True;
}

void
AllocateAndInitGeoBuffer(textured_2d_geometry_buffer *Geo, u32 VertCount, memory_arena *DebugArena)
{
  Geo->Verts  = Allocate(v3, DebugArena, VertCount);
  Geo->Colors = Allocate(v3, DebugArena, VertCount);
  Geo->UVs    = Allocate(v3, DebugArena, VertCount);

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

shader
MakeRenderToTextureShader(memory_arena *Memory, m4 *ViewProjection)
{
  shader Shader = LoadShaders( "RenderToTexture.vertexshader",
                               "RenderToTexture.fragmentshader",
                                Memory);

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(Memory, &Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;

  return Shader;
}


/******************************                *******************************/
/******************************  2D Buffering  *******************************/
/******************************                *******************************/


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
  glBindTexture(GL_TEXTURE_3D, RG->FontTexture->ID);
  //

  glUniform1i(RG->TextTextureUniform, 0); // Assign texture unit 0 to the TextTexureUniform

  u32 AttributeIndex = 0;
  BufferVertsToCard(  RG->SolidUIVertexBuffer, Geo, &AttributeIndex);
  BufferUVsToCard(    RG->SolidUIUVBuffer, Geo, &AttributeIndex);
  BufferColorsToCard( RG->SolidUIColorBuffer, Geo, &AttributeIndex);

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
BufferUVForQuad(textured_2d_geometry_buffer* Geo, u32 Slice)
{
  v3 uv_up_left    = V3(0, 1, Slice);
  v3 uv_up_right   = V3(1, 1, Slice);
  v3 uv_down_right = V3(1, 0, Slice);
  v3 uv_down_left  = V3(0, 0, Slice);

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
BufferTextUVs(textured_2d_geometry_buffer *Geo, v2 UV, debug_texture_array_slice Slice)
{
  r32 OneOverSixteen = 1.0f/16.0f;

  v3 uv_up_left    = V3( UV.x                , UV.y+OneOverSixteen , (r32)Slice);
  v3 uv_up_right   = V3( UV.x+OneOverSixteen , UV.y+OneOverSixteen , (r32)Slice);
  v3 uv_down_right = V3( UV.x+OneOverSixteen , UV.y                , (r32)Slice);
  v3 uv_down_left  = V3( UV.x                , UV.y                , (r32)Slice);

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
BufferColors(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo, v3 Color)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  BufferColors(Geo->Colors, Geo->At, Color);
}

void
BufferColors(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v3 Color)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  BufferColors(Geo->Colors, Geo->At, Color);
}

#define TO_NDC(P) ((P * ToNDC) - 1.0f)

v2
BufferQuadDirect(v3 *Dest, u32 StartingIndex, v2 MinP, v2 Dim, r32 Z, v2 ScreenDim, v2 MaxClip)
{
  // Note(Jesse): Z==0 is farthest back, Z==1 is closest to the camera
  Assert(Z >= 0.0f && Z <= 1.0f);

  v3 vertex_up_left    = V3( MinP.x       , MinP.y      , Z);
  v3 vertex_up_right   = V3( MinP.x+Dim.x , MinP.y      , Z);
  v3 vertex_down_right = V3( MinP.x+Dim.x , MinP.y+Dim.y, Z);
  v3 vertex_down_left  = V3( MinP.x       , MinP.y+Dim.y, Z);

  if (LengthSq(MaxClip) > 0.0f)
  {
    Print(MaxClip);

    if (vertex_up_right.x > MaxClip.x)
    {
      vertex_up_right.x = MaxClip.x;
      // TODO(Jesse): Remap UVs somehow??
    }

    if (vertex_down_right.x > MaxClip.x)
    {
      vertex_down_right.x = MaxClip.x;
      // TODO(Jesse): Remap UVs somehow??
    }

    if (vertex_down_right.y > MaxClip.y)
    {
      vertex_down_right.y = MaxClip.y;
      // TODO(Jesse): Remap UVs somehow??
    }

    if (vertex_down_left.y > MaxClip.y)
    {
      vertex_down_left.y = MaxClip.y;
      // TODO(Jesse): Remap UVs somehow??
    }
  }

  v3 ToNDC = 2.0f/V3(ScreenDim.x, ScreenDim.y, 1.0f);

  // Native OpenGL screen coordinates are {0,0} at the bottom-left corner. This
  // maps the origin to the top-left of the screen.
  v3 InvertYZ = V3(1.0f, -1.0f, -1.0f);

  Dest[StartingIndex++] = InvertYZ * TO_NDC(vertex_up_left);
  Dest[StartingIndex++] = InvertYZ * TO_NDC(vertex_down_left);
  Dest[StartingIndex++] = InvertYZ * TO_NDC(vertex_up_right);
  Dest[StartingIndex++] = InvertYZ * TO_NDC(vertex_down_right);
  Dest[StartingIndex++] = InvertYZ * TO_NDC(vertex_up_right);
  Dest[StartingIndex++] = InvertYZ * TO_NDC(vertex_down_left);

  return vertex_down_right.xy;
}

inline v2
BufferQuad(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z = 0.5f, v2 MaxClip = V2(0))
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  v2 Result = BufferQuadDirect(Geo->Verts, Geo->At, MinP, Dim, Z, Group->ScreenDim, MaxClip);
  return Result;
}

inline v2
BufferQuad(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v2 MinP, v2 Dim, r32 Z = 0.5f, v2 MaxClip = V2(0))
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  v2 MaxP = BufferQuadDirect(Geo->Verts, Geo->At, MinP, Dim, Z, Group->ScreenDim, MaxClip);
  return MaxP;
}

inline r32
BufferChar(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo, u32 CharIndex, v2 MinP, font *Font, const char *Text, u32 Color, v2 MaxClip = V2(0))
{
  r32 DeltaX = 0;

  char Char = Text[CharIndex];
  v2 UV = GetUVForCharCode(Char);

  { // Black Drop-shadow
    v2 ShadowOffset = 0.1f*V2(Font->Size);
    BufferQuad(Group, Geo, MinP+ShadowOffset, V2(Font->Size), 0.99f, MaxClip);
    BufferTextUVs(Geo, UV, DebugTextureArraySlice_Font);
    BufferColors(Group, Geo, GetColorData(BLACK).xyz);
    Geo->At += 6;
  }

  v2 MaxP = BufferQuad(Group, Geo, MinP, V2(Font->Size), 1.0f, MaxClip);
  BufferTextUVs(Geo, UV, DebugTextureArraySlice_Font);
  BufferColors(Group, Geo, GetColorData(Color).xyz);
  Geo->At += 6;

  DeltaX = (MaxP.x - MinP.x);

  return DeltaX;
}

r32
BufferValue(const char* Text, debug_ui_render_group *Group, layout *Layout, u32 Color, v2 MaxClip = V2(0))
{
  textured_2d_geometry_buffer *Geo = &Group->TextGroup->TextGeo;

  s32 QuadCount = (s32)strlen(Text);

  r32 DeltaX = 0;

  if (LengthSq(MaxClip) > 0.0f) MaxClip+=Layout->Basis;

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = Layout->Basis + Layout->At + V2(Group->Font.Size*CharIndex, 0);
    DeltaX += BufferChar(Group, Geo, CharIndex, MinP, &Group->Font, Text, Color, MaxClip);
    continue;
  }

  Layout->At.x += DeltaX;
  AdvanceClip(Layout);
  return DeltaX;
}

inline void
BufferValue(r32 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%f", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex);
  return;
}

inline void
BufferValue(u64 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex);
  return;
}

inline void
BufferValue(u32 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex);
  return;
}

void
EndClipRect(debug_ui_render_group *Group, layout *Layout, untextured_2d_geometry_buffer *Geo)
{
  v2 MinP = Layout->Clip.Min + Layout->Basis;
  v2 Dim = Layout->Clip.Max - Layout->Clip.Min;

  BufferQuad(Group, Geo, MinP, Dim, 0.0f);
  BufferColors(Group, Geo, V3(0.2f));
  Geo->At+=6;

  return;
}


/*****************************                ********************************/
/*****************************  Text Helpers  ********************************/
/*****************************                ********************************/

inline void
AdvanceSpaces(u32 N, layout *Layout, font *Font)
{
  Layout->At.x += (N*Font->Size);
  AdvanceClip(Layout);
  return;
}

inline rect2
NewLine(layout *Layout, font *Font)
{
  v2 Min = { Layout->Basis.x, Layout->Basis.y + Layout->At.y };
  Layout->At.y += (Font->LineHeight);
  v2 Max = Layout->Basis + Layout->At;

  rect2 Bounds = RectMinMax(Min, Max);
  Layout->At.x = 0;
  AdvanceClip(Layout);
  return Bounds;
}

r32
BufferLine(const char* Text, u32 Color, layout *Layout, font *Font, debug_ui_render_group *Group)
{
  r32 xOffset = BufferValue(Text, Group, Layout, Color);
  NewLine(Layout, Font);
  return xOffset;
}

inline rect2
NewRow(table_layout *Table, font *Font)
{
  Table->ColumnIndex = 0;
  rect2 Bounds = NewLine(&Table->Layout, Font);
  return Bounds;
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
ToString(u64 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%lu", Number);
  return Buffer;
}

inline char*
ToString(s32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%i", Number);
  return Buffer;
}

inline char*
ToString(u32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%u", Number);
  return Buffer;
}

inline char*
ToString(r32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%.2f", Number);
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

  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%.1f%c", (r32)Display, Units);

  return Buffer;
}

inline void
BufferMemorySize(u64 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
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
BufferThousands(u64 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, u32 Columns = 10)
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
BufferColumn( s32 Value, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
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
BufferColumn( u32 Value, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
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
BufferColumn( u64 Value, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
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
BufferColumn( r64 Perc, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
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
BufferColumn( r32 Perc, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
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

function v2
GetTextBounds(u32 TextLength, font* Font)
{
  v2 Result = {};
  Result.x = TextLength * Font->Size;
  Result.y = Font->LineHeight;
  return Result;
}

function rect2
Column(const char* ColumnText, debug_ui_render_group *Group, table_layout *Table, u8 Color, u8 HoverColor = TEAL, v2 MaxClip = V2(0))
{
  Table->ColumnIndex = (Table->ColumnIndex+1)%MAX_TABLE_COLUMNS;
  table_column *Column = Table->Columns + Table->ColumnIndex;

  u32 TextLength = (u32)strlen(ColumnText);
  Column->Max = Max(Column->Max, TextLength + 1);

  u32 Pad = Column->Max - TextLength;
  AdvanceSpaces(Pad, &Table->Layout, &Group->Font);

  v2 Min = Table->Layout.Basis + Table->Layout.At;
  v2 Max = Min + GetTextBounds(TextLength, &Group->Font);

  u8 UseColor = Color;
  rect2 Bounds = RectMinMax(Min, Max);
  if (IsInsideRect(Bounds, Group->MouseP))
  {
    UseColor = HoverColor;
  }

  BufferValue(ColumnText, Group, &Table->Layout, UseColor, MaxClip);

  return Bounds;
}

/* function rect2 */
/* Column(const char* ColumnText, debug_ui_render_group *Group, window_layout *Window, u8 Color, u8 HoverColor = TEAL) */
/* { */
/*   rect2 Bounds = Column(ColumnText, Group, &Window->Table, Color, HoverColor, Window->MaxClip); */
/*   return Bounds; */
/* } */

function b32
Button(const char* ColumnText, debug_ui_render_group *Group, table_layout *Table, u8 Color)
{
  b32 Result = False;
  rect2 Bounds = Column(ColumnText, Group, Table, Color);
  if (IsInsideRect(Bounds, Group->MouseP) && Group->Input->LMB.WasPressed)
  {
    Result = True;
  }

  return Result;
}

inline void
BufferScopeTreeEntry(debug_ui_render_group *Group, debug_profile_scope *Scope, table_layout *Layout,
    u8 Color, u64 TotalCycles, u64 TotalFrameCycles, u64 CallCount, u32 Depth)
{
  Assert(TotalFrameCycles);

  r32 Percentage = 100.0f * (r32)SafeDivide0((r64)TotalCycles, (r64)TotalFrameCycles);
  u64 AvgCycles = (u64)SafeDivide0(TotalCycles, CallCount);

  Column(ToString(Percentage), Group, Layout, Color);
  Column(ToString(AvgCycles),  Group, Layout, Color);
  Column(ToString(CallCount),  Group, Layout, Color);

  AdvanceSpaces((Depth*2)+1, &Layout->Layout, &Group->Font);

  if (Scope->Expanded && Scope->Child)
  {
    BufferValue("-", Group, &Layout->Layout, Color);
  }
  else if (Scope->Child)
  {
    BufferValue("+", Group, &Layout->Layout, Color);
  }
  else
  {
    AdvanceSpaces(1, &Layout->Layout, &Group->Font);
  }

  BufferValue(Scope->Name, Group, &Layout->Layout, Color);
  NewRow(Layout, &Group->Font);

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

#if 0
scope_stats
GetStatsFor( debug_profile_scope *Target, debug_profile_scope *Root)
{
  scope_stats Result = {};

  debug_profile_scope *Current = Root;
  if (Target->Parent) Current = Target->Parent->Child; // Selects first sibling

  while (Current)
  {
    if (Current == Target) // Find Ourselves
    {
      if (Result.Calls == 0) // We're first
      {
        Result.IsFirst = True;
      }
      else
      {
        break;
      }
    }

    // These are compile-time string constants, so we can compare pointers to
    // find equality
    if (Current->Name == Target->Name)
    {
      ++Result.Calls;
      Result.CumulativeCycles += Current->CycleCount;

      if (!Result.MinScope || Current->CycleCount < Result.MinScope->CycleCount)
        Result.MinScope = Current;

      if (!Result.MaxScope || Current->CycleCount > Result.MaxScope->CycleCount)
        Result.MaxScope = Current;
    }

    Current = Current->Sibling;
  }

  return Result;
}
#endif

template <typename T> u8
HoverAndClickExpand(debug_ui_render_group *Group, layout *Layout, T *Expandable, u8 Color, u8 HoverColor)
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

r32
BufferTextAt(debug_ui_render_group *Group, v2 BasisP, const char *Text, u32 Color)
{
  textured_2d_geometry_buffer *Geo = &Group->TextGroup->TextGeo;

  s32 QuadCount = (s32)strlen(Text);

  r32 DeltaX = 0;

  for ( s32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = BasisP + V2(Group->Font.Size*CharIndex, 0);
    DeltaX += BufferChar(Group, Geo, CharIndex, MinP, &Group->Font, Text, Color);
    continue;
  }

  return DeltaX;
}

void
DoTooltip(debug_ui_render_group *Group, const char *Text)
{
  BufferTextAt(Group, Group->MouseP+V2(12, -7), Text, WHITE);
  return;
}


/****************************                       **************************/
/****************************  Mutex Introspection  **************************/
/****************************                       **************************/


b32
DrawCycleBar( cycle_range *Range, cycle_range *Frame, r32 TotalGraphWidth, const char *Tooltip, v3 Color,
              debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, layout *Layout)
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

  AdvanceClip(Layout, QuadMaxP - Layout->Basis);

  return Hovering;
}

void
DrawWaitingBar(mutex_op_record *WaitRecord, mutex_op_record *AquiredRecord, mutex_op_record *ReleasedRecord,
               debug_ui_render_group *Group, layout *Layout, u64 FrameStartingCycle, u64 FrameTotalCycles, r32 TotalGraphWidth)
{
  Assert(WaitRecord->Op == MutexOp_Waiting);
  Assert(AquiredRecord->Op == MutexOp_Aquired);
  Assert(ReleasedRecord->Op == MutexOp_Released);

  Assert(AquiredRecord->Mutex == WaitRecord->Mutex);
  Assert(ReleasedRecord->Mutex == WaitRecord->Mutex);

  u64 WaitCycleCount = AquiredRecord->Cycle - WaitRecord->Cycle;
  u64 AquiredCycleCount = ReleasedRecord->Cycle - AquiredRecord->Cycle;

  untextured_2d_geometry_buffer *Geo = &Group->TextGroup->UIGeo;
  cycle_range FrameRange = {FrameStartingCycle, FrameTotalCycles};

  cycle_range WaitRange = {WaitRecord->Cycle, WaitCycleCount};
  DrawCycleBar( &WaitRange, &FrameRange, TotalGraphWidth, 0, V3(1, 0, 0), Group, Geo, Layout);

  cycle_range AquiredRange = {AquiredRecord->Cycle, AquiredCycleCount};
  DrawCycleBar( &AquiredRange, &FrameRange, TotalGraphWidth, 0, V3(0, 1, 0), Group, Geo, Layout);

  return;
}


/****************************                 ********************************/
/****************************  Picked Chunks  ********************************/
/****************************                 ********************************/


function v2
DrawTexturedQuadAt(debug_ui_render_group* Group, textured_2d_geometry_buffer* Geo, v2 MinP, v2 Dim)
{
  u8 Color = WHITE;

  v2 MaxP = BufferQuad(Group, Geo, MinP, Dim);
  BufferUVForQuad(Geo, DebugTextureArraySlice_Viewport);
  BufferColors(Group, Geo, GetColorData(Color).xyz);
  Geo->At += 6;

  return MaxP;
}

function void
BufferRectangleAt(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo,
                v2 MinP, v2 Dim, v3 Color, r32 Z = 0.5f)
{
  BufferQuad(Group, Geo, MinP, Dim, Z);
  BufferColors(Group, Geo, Color);
  Geo->At+=6;

  return;
}

function void
DrawPickedChunks(debug_ui_render_group* Group, v2 LayoutBasis)
{
  debug_state* DebugState = GetDebugState();

  local_persist table_layout PickerTable_ = {};
  table_layout *PickerTable = &PickerTable_;

  PickerTable->Layout.At = V2(0,0);
  PickerTable->Layout.Clip = NullClipRect;
  PickerTable->Layout.Basis = LayoutBasis;

  world_chunk** PickedChunks = DebugState->PickedChunks;
  u32* PickedChunkCount = DebugState->PickedChunkCount;

  MapGpuElementBuffer(&DebugState->GameGeo);

  BeginClipRect(&PickerTable->Layout);
  for (u32 ChunkIndex = 0;
      ChunkIndex < *PickedChunkCount;
      ++ChunkIndex)
  {
    world_chunk *Chunk = PickedChunks[ChunkIndex];

    u8 MainColor = Chunk == DebugState->HotChunk ? PINK : WHITE;

    interactable PickerListInteraction = StartInteractable(PickerTable, (umm)PickerTable);
      Column(ToString(Chunk->WorldP.x), Group, PickerTable, MainColor, MainColor);
      Column(ToString(Chunk->WorldP.y), Group, PickerTable, MainColor, MainColor);
      Column(ToString(Chunk->WorldP.z), Group, PickerTable, MainColor, MainColor);
    EndInteractable(Group, PickerTable, &PickerListInteraction);

    if (Clicked(Group, &PickerListInteraction))
    {
      DebugState->HotChunk = Chunk;
    }

    if (Button("X", Group, PickerTable, RED))
    {
      PickedChunks[ChunkIndex] = PickedChunks[*PickedChunkCount-1];
      *PickedChunkCount = *PickedChunkCount-1;
    }

    NewRow(PickerTable, &Group->Font);
  }

  if (DebugState->HotChunk)
  {
    v3 Basis = -0.5f*V3(WORLD_CHUNK_DIM);
    untextured_3d_geometry_buffer* Src = DebugState->HotChunk->LodMesh;
    untextured_3d_geometry_buffer* Dest = &Group->GameGeo->Buffer;
    BufferVertsChecked(Src, Dest, Basis, V3(1.0f));
  }

  { // Draw hotchunk to the GameGeo FBO
    glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID);
    FlushBuffersToCard(&DebugState->GameGeo);

    DebugState->ViewProjection =
      ProjectionMatrix(&DebugState->Camera, DEBUG_TEXTURE_DIM, DEBUG_TEXTURE_DIM) *
      ViewMatrix(WORLD_CHUNK_DIM, &DebugState->Camera);

    glUseProgram(Group->GameGeoShader->ID);

    SetViewport(V2(DEBUG_TEXTURE_DIM, DEBUG_TEXTURE_DIM));

    BindShaderUniforms(Group->GameGeoShader);

    Draw(DebugState->GameGeo.Buffer.At);
    DebugState->GameGeo.Buffer.At = 0;
  }

  local_persist window_layout PickerWindow = WindowLayout(V2(430.0f, 137.0f), V2(400.0f, 350.0f));

  Clear(&PickerWindow.Layout.At);
  Clear(&PickerWindow.Layout.Clip);

  if (DebugState->HotChunk)
  {
    v2 QuadDim = PickerWindow.MaxClip - PickerWindow.Layout.At;
    DrawTexturedQuadAt( Group, &Group->TextGroup->TextGeo,
                        GetAbsoluteAt(&PickerWindow.Layout),
                        QuadDim);

    PickerWindow.Layout.At += QuadDim;
    AdvanceClip(&PickerWindow.Layout);
  }

  untextured_2d_geometry_buffer *Geo = &Group->TextGroup->UIGeo;
  EndClipRect(Group, &PickerWindow.Layout, Geo);

  UpdateGameCamera( -0.005f*Group->MouseDP,
                    Group->Input,
                    Canonical_Position(0),
                    &DebugState->Camera);

  if (Length(PickerWindow.MaxClip) == 0.0f)
  {
    PickerWindow.MaxClip = PickerWindow.Layout.Clip.Max;
  }

  v2 Dim = V2(8);
  r32 Z = 0.6f;
  {
    v2 MinP = PickerWindow.Layout.Basis + PickerWindow.MaxClip;
    v3 Color = V3(0.5f, 0.5f, 0.0f);

    interactable Interaction = Interactable(MinP, MinP+Dim, (umm)"PickerTableResizeWindowWidget");
    if (Pressed(Group, &Interaction))
    {
      PickerWindow.MaxClip = Group->MouseP - PickerWindow.Layout.Basis;
      PickerWindow.MaxClip = Max(PickerWindow.MaxClip, V2(0,0));

      Color *= 2.0f;
    }

    if (Hover(Group, &Interaction))
    {
      Color *= 2.0f;
    }

    BufferRectangleAt(Group, &Group->TextGroup->UIGeo, MinP, Dim, Color, Z);
  }

  {
    v2 MinP = PickerWindow.Layout.Basis - (Dim/2.0f);
    v3 Color = V3(0.0f, 0.0f, 0.5f);

    interactable Interaction = Interactable(MinP, MinP+Dim, (umm)"PickerTableChangeBasisWindowWidget");
    if (Pressed(Group, &Interaction))
    {
      PickerWindow.Layout.Basis = Group->MouseP;
      Color *= 2.0f;
    }

    if (Hover(Group, &Interaction))
    {
      Color *= 2.0f;
    }

    BufferRectangleAt(Group, &Group->TextGroup->UIGeo, MinP, Dim, Color, Z);
  }

  return;
}



/************************                        *****************************/
/************************  Thread Perf Bargraph  *****************************/
/************************                        *****************************/


void
DrawScopeBarsRecursive(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, debug_profile_scope *Scope,
                       layout *Layout, cycle_range *Frame, r32 TotalGraphWidth, random_series *Entropy)
{
  while (Scope)
  {
    Assert(Scope->Name);

    cycle_range Range = {Scope->StartingCycle, Scope->CycleCount};
    v3 Color = RandomV3(Entropy);

    b32 Hovering = DrawCycleBar( &Range, Frame, TotalGraphWidth, Scope->Name, Color, Group, Geo, Layout);
    if (Hovering && Group->Input->LMB.WasPressed)
      Scope->Expanded = !Scope->Expanded;

    if (Scope->Expanded)
    {
      layout ChildrensLayout = *Layout;
      NewLine(&ChildrensLayout, &Group->Font);
      DrawScopeBarsRecursive(Group, Geo, Scope->Child, &ChildrensLayout, Frame, TotalGraphWidth, Entropy);
      AdvanceClip(&ChildrensLayout);
      Layout->Clip.Max = Max(ChildrensLayout.Clip.Max, Layout->Clip.Max);
      Layout->Clip.Min = Max(ChildrensLayout.Clip.Min, Layout->Clip.Min);
    }

    Scope = Scope->Sibling;
  }

  return;
}

void
DebugDrawCycleThreadGraph(debug_ui_render_group *Group, debug_state *SharedState, v2 BasisP)
{
  random_series Entropy = {};
  r32 TotalGraphWidth = 1500.0f;
  untextured_2d_geometry_buffer *Geo = &Group->TextGroup->UIGeo;


  local_persist table_layout CycleGraphTable;
  CycleGraphTable.Layout.At = V2(0,0);
  CycleGraphTable.Layout.Clip = NullClipRect;
  CycleGraphTable.Layout.Basis = BasisP;

  layout* Layout = &CycleGraphTable.Layout;

  SetFontSize(&Group->Font, 30);

  r32 MinY = Layout->Basis.y + Layout->At.y;

  u32 TotalThreadCount                = GetTotalThreadCount();
  frame_stats *FrameStats             = SharedState->Frames + SharedState->ReadScopeIndex;
  cycle_range FrameCycles             = {FrameStats->StartingCycle, FrameStats->TotalCycles};

  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree    = MainThreadState->ScopeTrees + SharedState->ReadScopeIndex;

  for ( u32 ThreadIndex = 0;
        ThreadIndex < TotalThreadCount;
        ++ThreadIndex)
  {
    BeginClipRect(Layout);

    char *ThreadName = FormatString(TranArena, "Thread %u", ThreadIndex);
    Column(ThreadName, Group, &CycleGraphTable, WHITE);
    NewRow(&CycleGraphTable, &Group->Font);

    debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
    debug_scope_tree *ReadTree = ThreadState->ScopeTrees + SharedState->ReadScopeIndex;
    if (MainThreadReadTree->FrameRecorded == ReadTree->FrameRecorded)
    {
      DrawScopeBarsRecursive(Group, Geo, ReadTree->Root, Layout, &FrameCycles, TotalGraphWidth, &Entropy);
    }

    Layout->At.y = Layout->Clip.Max.y + 25; // Advance vertical at for next thread

    EndClipRect(Group, Layout, Geo);
    NewRow(&CycleGraphTable, &Group->Font);
  }

  NewLine(Layout, &Group->Font);

  r32 TotalMs = (r32)FrameStats->FrameMs;

  if (TotalMs > 0.0f)
  {
    r32 MarkerWidth = 3.0f;

    {
      r32 FramePerc = 16.66666f/TotalMs;
      r32 xOffset = FramePerc*TotalGraphWidth;
      v2 MinP16ms = Layout->Basis + V2( xOffset, MinY );
      v2 MaxP16ms = Layout->Basis + V2( xOffset+MarkerWidth, Layout->At.y );
      v2 Dim = MaxP16ms - MinP16ms;
      BufferQuad(Group, Geo, MinP16ms, Dim);
      BufferColors(Group, Geo, V3(0,1,0));
      Geo->At+=6;
    }
    {
      r32 FramePerc = 33.333333f/TotalMs;
      r32 xOffset = FramePerc*TotalGraphWidth;
      v2 MinP16ms = Layout->Basis + V2( xOffset, MinY );
      v2 MaxP16ms = Layout->Basis + V2( xOffset+MarkerWidth, Layout->At.y );
      v2 Dim = MaxP16ms - MinP16ms;
      BufferQuad(Group, Geo, MinP16ms, Dim);
      BufferColors(Group, Geo, V3(1,1,0));
      Geo->At+=6;
    }
  }

#if 0
  u32 UnclosedMutexRecords = 0;
  u32 TotalMutexRecords = 0;
  TIMED_BLOCK("Mutex Record Collation");
  for ( u32 ThreadIndex = 0;
        ThreadIndex < TotalThreadCount;
        ++ThreadIndex)
  {
    debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
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


/******************************              *********************************/
/******************************  Call Graph  *********************************/
/******************************              *********************************/


struct called_function
{
  const char* Name;
  u32 CallCount;
};

#define MAX_RECORDED_FUNCTION_CALLS 128
static called_function ProgramFunctionCalls[MAX_RECORDED_FUNCTION_CALLS];
static called_function NullFunctionCall = {};

void
CollateAllFunctionCalls(debug_profile_scope* Current)
{
  if (!Current || !Current->Name)
    return;

  called_function* Prev = 0;
  for ( u32 FunctionIndex = 0;
      FunctionIndex < MAX_RECORDED_FUNCTION_CALLS;
      ++FunctionIndex)
  {
    called_function* Func = ProgramFunctionCalls + FunctionIndex;

    if (Func->Name == Current->Name || !Func->Name)
    {
      Func->Name = Current->Name;
      Func->CallCount++;
      u32 SwapIndex = MAX_RECORDED_FUNCTION_CALLS;
      for (s32 PrevIndex = (s32)FunctionIndex -1;
          PrevIndex >= 0;
          --PrevIndex)
      {
        Prev = ProgramFunctionCalls + PrevIndex;
        if (Prev->CallCount < Func->CallCount)
        {
          SwapIndex = PrevIndex;
        }
        else
          break;
      }

      if(SwapIndex < MAX_RECORDED_FUNCTION_CALLS)
      {
        called_function* Swap = ProgramFunctionCalls + SwapIndex;
        called_function Temp = *Swap;
        *Swap = *Func;
        *Func = Temp;
      }

      break;
    }

    Prev = Func;

    if (FunctionIndex == MAX_RECORDED_FUNCTION_CALLS-1)
    {
      Warn("MAX_RECORDED_FUNCTION_CALLS limit reached");
    }
  }

  if (Current->Sibling)
  {
    CollateAllFunctionCalls(Current->Sibling);
  }

  if (Current->Child)
  {
    CollateAllFunctionCalls(Current->Child);
  }

  return;
}

unique_debug_profile_scope *
ListContainsScope(unique_debug_profile_scope* List, debug_profile_scope* Query)
{
  unique_debug_profile_scope* Result = 0;
  while (List)
  {
    if (StringsMatch(List->Name, Query->Name))
    {
      Result = List;
      break;
    }
    List = List->NextUnique;
  }

  return Result;
}

void
BufferFirstCallToEach(debug_ui_render_group *Group,
    debug_profile_scope *Scope_in, debug_profile_scope *TreeRoot,
    memory_arena *Memory, table_layout* CallgraphLayout, u64 TotalFrameCycles, u32 Depth)
{
  unique_debug_profile_scope* UniqueScopes = {};

  debug_profile_scope* CurrentUniqueScopeQuery = Scope_in;
  while (CurrentUniqueScopeQuery)
  {
    unique_debug_profile_scope* GotUniqueScope = ListContainsScope(UniqueScopes, CurrentUniqueScopeQuery);
    if (!GotUniqueScope )
    {
      GotUniqueScope = AllocateProtection(unique_debug_profile_scope, TranArena, 1, False);
      GotUniqueScope->NextUnique = UniqueScopes;
      UniqueScopes = GotUniqueScope;
    }

    GotUniqueScope->Name = CurrentUniqueScopeQuery->Name;
    GotUniqueScope->CallCount++;
    GotUniqueScope->TotalCycles += CurrentUniqueScopeQuery->CycleCount;
    GotUniqueScope->MinCycles = Min(CurrentUniqueScopeQuery->CycleCount, GotUniqueScope->MinCycles);
    GotUniqueScope->MaxCycles = Max(CurrentUniqueScopeQuery->CycleCount, GotUniqueScope->MaxCycles);
    GotUniqueScope->Scope = CurrentUniqueScopeQuery;

    CurrentUniqueScopeQuery = CurrentUniqueScopeQuery->Sibling;
  }

  while (UniqueScopes)
  {
    u8 MainColor = (u8)HoverAndClickExpand(Group, &CallgraphLayout->Layout, UniqueScopes->Scope, WHITE, TEAL);
    BufferScopeTreeEntry(Group, UniqueScopes->Scope, CallgraphLayout, MainColor, UniqueScopes->TotalCycles, TotalFrameCycles, UniqueScopes->CallCount, Depth);

    if (UniqueScopes->Scope->Expanded)
      BufferFirstCallToEach(Group, UniqueScopes->Scope->Child, TreeRoot, Memory, CallgraphLayout, TotalFrameCycles, Depth+1);

    UniqueScopes = UniqueScopes->NextUnique;
  }

  return;
}

void
DebugDrawCollatedFunctionCalls(debug_ui_render_group *Group, debug_state *DebugState, v2 BasisP)
{
  local_persist table_layout FunctionCallLayout;
  TIMED_BLOCK("Collated Function Calls");
  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree = MainThreadState->ScopeTrees + DebugState->ReadScopeIndex;

  CollateAllFunctionCalls(MainThreadReadTree->Root);

  FunctionCallLayout.Layout.At = V2(0,0);
  FunctionCallLayout.Layout.Clip = NullClipRect;
  FunctionCallLayout.Layout.Basis = BasisP;
  for ( u32 FunctionIndex = 0;
      FunctionIndex < MAX_RECORDED_FUNCTION_CALLS;
      ++FunctionIndex)
  {
    called_function *Func = ProgramFunctionCalls + FunctionIndex;
    if (Func->Name)
    {
      Column( Func->Name, Group, &FunctionCallLayout, WHITE);
      Column( ToString(Func->CallCount), Group, &FunctionCallLayout, WHITE);
      NewRow(&FunctionCallLayout, &Group->Font);
    }
  }
  END_BLOCK("Collated Function Calls");

}

clip_rect
DebugDrawCallGraph(debug_ui_render_group *Group, debug_state *DebugState, layout *MainLayout, r64 MaxMs)
{
  v2 MouseP = Group->MouseP;

  NewLine(MainLayout, &Group->Font);
  SetFontSize(&Group->Font, 80);

  TIMED_BLOCK("Frame Ticker");
    v2 StartingAt = MainLayout->At;

    for (u32 FrameIndex = 0;
        FrameIndex < DEBUG_FRAMES_TRACKED;
        ++FrameIndex )
    {
      frame_stats *Frame = DebugState->Frames + FrameIndex;
      r32 Perc = (r32)SafeDivide0(Frame->FrameMs, MaxMs);

      v2 MinP = MainLayout->At;
      v2 MaxDim = V2(15.0, Group->Font.Size);

      v3 Color = V3(0.5f, 0.5f, 0.5f);

      debug_scope_tree *Tree = GetThreadLocalStateFor(0)->ScopeTrees + FrameIndex;
      if ( Tree == DebugState->GetWriteScopeTree() )
      {
        Color = V3(0.8f, 0.0f, 0.0f);
        Perc = 0.05f;
      }

      if ( Tree == DebugState->GetReadScopeTree(0) )
        Color = V3(0.8f, 0.8f, 0.0f);

      v2 QuadDim = V2(15.0, (Group->Font.Size) * Perc);
      v2 Offset = MaxDim - QuadDim;

      v2 DrawDim = BufferQuad(Group, &Group->TextGroup->UIGeo, MinP + Offset, QuadDim);
      MainLayout->At.x = DrawDim.x + 5.0f;
      AdvanceClip(MainLayout, MainLayout->At + V2(0, Group->Font.Size));

      if (MouseP > MinP && MouseP < DrawDim)
      {
        debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
        if (FrameIndex != MainThreadState->WriteIndex % DEBUG_FRAMES_TRACKED)
        {
          DebugState->ReadScopeIndex = FrameIndex;
          Color = V3(0.8f, 0.8f, 0.0f);
        }
      }

      BufferColors(Group->TextGroup->UIGeo.Colors, Group->TextGroup->UIGeo.At, Color);

      Group->TextGroup->UIGeo.At+=6;
    }


    r32 MaxBarHeight = Group->Font.Size;
    v2 QuadDim = V2(MainLayout->At.x, 2.0f);
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
      BufferColumn(Frame->FrameMs, 4, Group, MainLayout, WHITE);
      BufferThousands(Frame->TotalCycles, Group, MainLayout, WHITE);

      u32 TotalMutexOps = GetTotalMutexOpsForReadFrame();
      BufferThousands(TotalMutexOps, Group, MainLayout, WHITE);
    }
    NewLine(MainLayout, &Group->Font);

  END_BLOCK("Frame Ticker");

  u32 TotalThreadCount = GetWorkerThreadCount() + 1;

  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree    = MainThreadState->ScopeTrees + DebugState->ReadScopeIndex;

  local_persist table_layout CallgraphTableLayout;
  TIMED_BLOCK("Call Graph");

  CallgraphTableLayout.Layout.At = V2(0,0);
  CallgraphTableLayout.Layout.Clip = NullClipRect;
  CallgraphTableLayout.Layout.Basis = V2(0, MainLayout->Clip.Max.y);

  NewRow(&CallgraphTableLayout, &Group->Font);
  Column("Frame %",  Group,  &CallgraphTableLayout,  WHITE);
  Column("Cycles",   Group,  &CallgraphTableLayout,  WHITE);
  Column("Calls",    Group,  &CallgraphTableLayout,  WHITE);
  Column("Name",     Group,  &CallgraphTableLayout,  WHITE);
  NewRow(&CallgraphTableLayout, &Group->Font);

  for ( u32 ThreadIndex = 0;
      ThreadIndex < TotalThreadCount;
      ++ThreadIndex)
  {
    debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
    debug_scope_tree *ReadTree = ThreadState->ScopeTrees + DebugState->ReadScopeIndex;
    frame_stats *Frame = DebugState->Frames + DebugState->ReadScopeIndex;

    if (MainThreadReadTree->FrameRecorded == ReadTree->FrameRecorded)
    {
      BufferFirstCallToEach(Group, ReadTree->Root, ReadTree->Root, ThreadsafeDebugMemoryAllocator(), &CallgraphTableLayout, Frame->TotalCycles, 0);
      NewRow(&CallgraphTableLayout, &Group->Font);
    }
  }

  END_BLOCK("Call Graph");

  clip_rect Result = CallgraphTableLayout.Layout.Clip;
  return Result;
}

#if 0
void
ColumnLeft(u32 Width, const char *Text, debug_ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  u32 Len = (u32)strlen(Text);
  u32 Pad = Max(Width-Len, 0U);
  BufferValue(Text, Group, Layout, ColorIndex);
  AdvanceSpaces(Pad, Layout, &Group->Font);
}

void
ColumnRight(s32 Width, const char *Text, debug_ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  s32 Len = (s32)strlen(Text);
  s32 Pad = Max(Width-Len, 0);
  AdvanceSpaces(Pad, Layout, &Group->Font);
  BufferValue(Text, Group, Layout, ColorIndex);
}
#endif

/******************************              *********************************/
/******************************  Draw Calls  *********************************/
/******************************              *********************************/


debug_global const u32 Global_DrawCallArrayLength = 128;
debug_global debug_draw_call Global_DrawCalls[Global_DrawCallArrayLength] = {};
debug_global debug_draw_call NullDrawCall = {};

void
TrackDrawCall(const char* Caller, u32 VertexCount)
{
  u64 Index = ((u64)Caller) % Global_DrawCallArrayLength;

  debug_draw_call *DrawCall = &Global_DrawCalls[Index];

  if (DrawCall->Caller)
  {
    debug_draw_call* First = DrawCall;
    while( DrawCall->Caller &&
           !(StringsMatch(DrawCall->Caller, Caller) && DrawCall->N == VertexCount)
         )
    {
      ++Index;
      Index = Index % Global_DrawCallArrayLength;
      DrawCall = &Global_DrawCalls[Index];
      if (DrawCall == First)
      {
        Error("Draw Call table full!");
        break;
      }
    }
  }

  DrawCall->Caller = Caller;
  DrawCall->N = VertexCount;
  DrawCall->Calls++;

  return;
}

void
DebugDrawDrawCalls(debug_ui_render_group *Group, layout *Layout)
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


/*******************************            **********************************/
/*******************************  Arena UI  **********************************/
/*******************************            **********************************/


inline b32
BufferBarGraph(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, layout *Layout, r32 PercFilled, v3 Color)
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
BufferArenaBargraph(table_layout *BargraphTable, debug_ui_render_group *Group, umm TotalUsed, r32 TotalPerc, umm Remaining, v3 Color )
{
  Column( FormatMemorySize(TotalUsed), Group, BargraphTable, WHITE);
  b32 Hover = BufferBarGraph(Group, &Group->TextGroup->UIGeo, &BargraphTable->Layout, TotalPerc, Color);
  Column( FormatMemorySize(Remaining), Group, BargraphTable, WHITE);
  NewRow(BargraphTable, &Group->Font);

  b32 Click = (Hover && Group->Input->LMB.WasPressed);
  return Click;
}

v2
BufferMemoryStatsTable(memory_arena_stats MemStats, debug_ui_render_group *Group, table_layout *StatsTable, v2 BasisP)
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
BufferMemoryBargraphTable(debug_ui_render_group *Group, selected_arenas *SelectedArenas, memory_arena_stats MemStats, umm TotalUsed, memory_arena *HeadArena, table_layout *BargraphTable, v2 BasisP)
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
BufferDebugPushMetaData(debug_ui_render_group *Group, selected_arenas *SelectedArenas, umm CurrentArenaHead, table_layout *Table, v2 Basis)
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
DebugDrawMemoryHud(debug_ui_render_group *Group, debug_state *DebugState, v2 OriginalBasisP)
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
      Column(ToString(MemStats.Pushes), Group, &MemoryHudArenaTable, Color);
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

        BufferDebugPushMetaData(Group, SelectedArenas, HashArenaHead(Current->Arena), MetaTable, BasisP);
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


/*******************************              ********************************/
/*******************************  Network UI  ********************************/
/*******************************              ********************************/


void
DebugDrawNetworkHud(debug_ui_render_group *Group,
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


/******************************               ********************************/
/******************************  Graphics UI  ********************************/
/******************************               ********************************/


void
DebugDrawGraphicsHud(debug_ui_render_group *Group, debug_state *DebugState, layout *Layout)
{
  BufferValue("Graphics", Group, Layout, WHITE);

  NewLine(Layout, &Group->Font);
  NewLine(Layout, &Group->Font);

  BufferMemorySize(DebugState->BytesBufferedToCard, Group, Layout, WHITE);

  return;
}


/******************************              *********************************/
/******************************  Initialize  *********************************/
/******************************              *********************************/


b32
InitDebugRenderSystem(debug_state *DebugState, heap_allocator *Heap)
{
  AllocateMesh(&DebugState->LineMesh, 1024, Heap);

  if (!InitDebugOverlayFramebuffer(&DebugState->TextRenderGroup, ThreadsafeDebugMemoryAllocator(), "texture_atlas_0.bmp"))
  { Error("Initializing Debug Overlay Framebuffer"); }

  AllocateAndInitGeoBuffer(&DebugState->TextRenderGroup.TextGeo, 1024, ThreadsafeDebugMemoryAllocator());
  AllocateAndInitGeoBuffer(&DebugState->TextRenderGroup.UIGeo, 1024, ThreadsafeDebugMemoryAllocator());

  AllocateGpuElementBuffer(&DebugState->GameGeo, (u32)Kilobytes(64));

  DebugState->TextRenderGroup.SolidUIShader = MakeSolidUIShader(ThreadsafeDebugMemoryAllocator());

  DebugState->SelectedArenas = Allocate(selected_arenas, ThreadsafeDebugMemoryAllocator(), 1);

  DebugState->GameGeoFBO = GenFramebuffer();
  glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID);

  FramebufferTextureLayer(&DebugState->GameGeoFBO, DebugState->TextRenderGroup.FontTexture, DebugTextureArraySlice_Viewport);
  SetDrawBuffers(&DebugState->GameGeoFBO);

  v2i TextureDim = V2i(DEBUG_TEXTURE_DIM, DEBUG_TEXTURE_DIM);
  texture *DepthTexture = MakeDepthTexture( TextureDim, ThreadsafeDebugMemoryAllocator() );
  FramebufferDepthTexture(DepthTexture);

  b32 Result = CheckAndClearFramebuffer();
  Assert(Result);

  DebugState->GameGeoShader = MakeRenderToTextureShader(ThreadsafeDebugMemoryAllocator(),
                                                        &DebugState->ViewProjection);

  StandardCamera(&DebugState->Camera, 1000.0f, 100.0f);

  return Result;
}

