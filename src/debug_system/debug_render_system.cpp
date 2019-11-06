#include <texture.cpp>
#include <stream.cpp>
#include <shader.cpp>
#include <render_init.cpp>

#include <render.h>
#include <render_utils.cpp>
#include <bonsai_mesh.cpp>
#include <gpu_mapped_buffer.cpp>

#include <work_queue.cpp>



/*******************************             *********************************/
/*******************************  Z helpers  *********************************/
/*******************************             *********************************/



function r32
zIndexForBackgrounds(window_layout *Window, debug_ui_render_group *Group)
{
  // NOTE(Jesse): This is the bottom-most value on this slice
  //
  // NOTE(Jesse): We add one to the z-slice initially because a z-slice of 0 is
  // invalid.  It is invalid because the slices are discrete chunks of z space
  // as opposed to indices into an array, and slice 0 is just nonsensical in
  // that context.
  u64 zSlice = 1 + Group->InteractionStackTop - Window->InteractionStackIndex;
  r32 Result = 1.0f - ( (r32)zSlice / DEBUG_MAX_UI_WINDOW_SLICES );
  Assert(Result <= 1.0f && Result >= 0.0f);
  return Result;
}

function r32
zSliceInterval(void)
{
  r32 Result = (r32)1.0f / DEBUG_MAX_UI_WINDOW_SLICES;
  Assert(Result <= 1.0f && Result >= 0.0f);
  return Result;
}

function r32
zOffsetForTextShadow(void)
{
  r32 Result = (zSliceInterval()*0.05f);
  return Result;
}

function r32
zIndexForBorders(window_layout *Window, debug_ui_render_group *Group)
{
  r32 Result = zIndexForBackgrounds(Window, Group) + (zSliceInterval()*0.9f);
  Assert(Result <= 1.0f && Result >= 0.0f);
  return Result;
}

function r32
zIndexForText(window_layout *Window, debug_ui_render_group *Group)
{
  r32 Result = zIndexForBackgrounds(Window, Group) + (zSliceInterval()*0.45f);
  Assert(Result <= 1.0f && Result >= 0.0f);
  return Result;
}

function r32
zIndexForTitleBar(window_layout *Window, debug_ui_render_group *Group)
{
  r32 Result = zIndexForBackgrounds(Window, Group) + (zSliceInterval()*0.30f);
  Assert(Result <= 1.0f && Result >= 0.0f);
  return Result;
}

function r32
WindowTitleBarHeight(font* Font)
{
  r32 Result = Font->Size.y;
  return Result;
}




/*****************************                ********************************/
/*****************************  Text Helpers  ********************************/
/*****************************                ********************************/



function void
SetFontSize(font *Font, r32 FontSize)
{
  Font->Size.x = FontSize;
  Font->Size.y = FontSize * 1.3f;
  return;
}

function void
AdvanceSpaces(u32 N, layout *Layout, font *Font)
{
  Layout->At.x += (N*Font->Size.x);
  AdvanceClip(Layout);
  return;
}

// FIXME(Jesse): This doesn't do what the user might expect if they just dump
// a bunch of NewLine() functions in a row
function rect2
NewLine(layout *Layout)
{
  v2 Min = { Layout->Basis.x, Layout->Basis.y + Layout->At.y };
  Layout->At.y = Layout->DrawBounds.Max.y;
  v2 Max = Layout->Basis + Layout->At;

  rect2 Bounds = RectMinMax(Min, Max);
  Layout->At.x = 0;
  AdvanceClip(Layout);
  return Bounds;
}

function rect2
NewRow(table *Table)
{
  Table->ColumnIndex = 0;
  rect2 Bounds = NewLine(&Table->Layout);
  return Bounds;
}

function rect2
NewRow(window_layout *Window)
{
  rect2 Bounds = NewRow(&Window->Table);
  return Bounds;
}

function char*
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

template<typename number_type>counted_string
NumericValueToString(number_type Number, const char* Format)
{
  u32 BufferLength = 32;
  char *Buffer = AllocateProtection(char, TranArena, BufferLength, False);
  snprintf(Buffer, BufferLength, Format, Number);

  counted_string Result = CountedString(Buffer);
  return Result;
}

function counted_string
AsString(u64 Number)
{
  counted_string Result = NumericValueToString(Number, "%lu");
  return Result;
}

function counted_string
AsString(s32 Number)
{
  counted_string Result = NumericValueToString(Number, "%i");
  return Result;
}

function counted_string
AsString(u32 Number)
{
  counted_string Result = NumericValueToString(Number, "%u");
  return Result;
}

function counted_string
AsString(r32 Number)
{
  counted_string Result = NumericValueToString(Number, "%.2f");
  return Result;
}
function char*
ToString(u64 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%lu", Number);
  return Buffer;
}

function char*
ToString(s32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%i", Number);
  return Buffer;
}

function char*
ToString(u32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%u", Number);
  return Buffer;
}

function char*
ToString(r32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  sprintf(Buffer, "%.2f", Number);
  return Buffer;
}

function char*
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

function char*
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

function v2
GetTextBounds(u32 TextLength, font* Font)
{
  v2 Result = {};
  Result.x = TextLength * Font->Size.x;
  Result.y = Font->Size.y;
  return Result;
}



/******************************                *******************************/
/******************************  2D Buffering  *******************************/
/******************************                *******************************/



function void
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

function void
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
  BufferVertsToCard( RG->SolidUIVertexBuffer, Geo, &AttributeIndex);
  BufferUVsToCard(   RG->SolidUIUVBuffer,     Geo, &AttributeIndex);
  BufferColorsToCard(RG->SolidUIColorBuffer,  Geo, &AttributeIndex);

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

function void
BufferQuadUVs(textured_2d_geometry_buffer* Geo, rect2 UV, debug_texture_array_slice Slice)
{
  v3 LeftTop    = V3(UV.Min.x, UV.Min.y, (r32)Slice);
  v3 RightTop   = V3(UV.Max.x, UV.Min.y, (r32)Slice);
  v3 RightBottom = V3(UV.Max.x, UV.Max.y, (r32)Slice);
  v3 LeftBottom  = V3(UV.Min.x, UV.Max.y, (r32)Slice);

  u32 StartingIndex = Geo->At;
  Geo->UVs[StartingIndex++] = LeftTop;
  Geo->UVs[StartingIndex++] = LeftBottom;
  Geo->UVs[StartingIndex++] = RightTop;

  Geo->UVs[StartingIndex++] = RightBottom;
  Geo->UVs[StartingIndex++] = RightTop;
  Geo->UVs[StartingIndex++] = LeftBottom;

  return;
}

function rect2
UVsForFullyCoveredQuad(void)
{
  // Note(Jesse): These are weird compared to what you might expect because
  // OpenGL screen coordinates originate at the bottom left, but are inverted
  // in our app such that the origin is at the top-left
  // @inverted_screen_y_coordinate
  v2 LeftTop    = V2(0.0f, 1.0f);
  v2 RightBottom = V2(1.0f, 0.0f);

  rect2 Result = RectMinMax(LeftTop, RightBottom);
  return Result;
}

function rect2
UVsForChar(u8 C)
{
  r32 OneOverSixteen = 1.0f/16.0f;

  // Note(Jesse): These are weird compared to what you might expect because
  // OpenGL screen coordinates originate at the bottom left, but are inverted
  // in our app such that the origin is at the top-left
  // @inverted_screen_y_coordinate
  v2 LeftBottom = GetUVForCharCode(C);
  v2 LeftTop  = LeftBottom + V2(0.0f, OneOverSixteen);
  v2 RightBottom  = LeftBottom + V2(OneOverSixteen, 0.0f);

  rect2 Result = RectMinMax(LeftTop, RightBottom);
  return Result;
}

function void
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

function void
BufferColors(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo, v3 Color)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  BufferColors(Geo->Colors, Geo->At, Color);
}

function void
BufferColors(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, v3 Color)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  BufferColors(Geo->Colors, Geo->At, Color);
}

function clip_result
BufferQuadDirect(v3 *Dest, u32 StartingIndex, v2 MinP, v2 Dim, r32 Z, v2 ScreenDim, v2 MaxClip)
{
  // Note(Jesse): Z==0 | far-clip
  // Note(Jesse): Z==1 | near-clip

  Assert(Z >= 0.0f && Z <= 1.0f);

  clip_result Result = {};

  if ( MaxClip.x <= MinP.x || MaxClip.y <= MinP.y )
  {
    Result.ClipStatus = ClipStatus_FullyClipped;
  }
  else
  {
    r32 Left   = MinP.x;
    r32 Right  = Left+Dim.x;
    r32 Top    = MinP.y;
    r32 Bottom = Top+Dim.y;

    v3 LeftTop    = V3(Left, Top, Z);
    v3 RightTop   = V3(Right, Top, Z);
    v3 RightBottom = V3(Right, Bottom, Z);
    v3 LeftBottom  = V3(Left, Bottom, Z);

    if (Left < MaxClip.x && Right > MaxClip.x)
    {
      r32 Total = RightBottom.x - LeftBottom.x;
      r32 TotalClipped = RightBottom.x - MaxClip.x;
      Result.PartialClip.Max.x = TotalClipped / Total;

      Result.MaxClip.x = RightTop.x = RightBottom.x = MaxClip.x;
      Result.ClipStatus = ClipStatus_PartialClipping;

      Assert(Result.PartialClip.Max.x >= 0.0f && Result.PartialClip.Max.x <= 1.0f);
    }

    if (Top < MaxClip.y && Bottom > MaxClip.y)
    {
      r32 Total = RightBottom.y - RightTop.y;
      r32 TotalClipped = RightBottom.y - MaxClip.y;
      Result.PartialClip.Max.y = TotalClipped / Total;

      Result.MaxClip.y = LeftBottom.y = RightBottom.y = MaxClip.y;
      Result.ClipStatus = ClipStatus_PartialClipping;

      Assert(Result.PartialClip.Max.y >= 0.0f && Result.PartialClip.Max.y <= 1.0f);
    }

    #define TO_NDC(P) ((P * ToNDC) - 1.0f)
    v3 ToNDC = 2.0f/V3(ScreenDim.x, ScreenDim.y, 1.0f);

    // Native OpenGL screen coordinates are {0,0} at the bottom-left corner. This
    // maps the origin to the top-left of the screen.
    // @inverted_screen_y_coordinate
    v3 InvertYZ = V3(1.0f, -1.0f, -1.0f);

    Dest[StartingIndex++] = InvertYZ * TO_NDC(LeftTop);
    Dest[StartingIndex++] = InvertYZ * TO_NDC(LeftBottom);
    Dest[StartingIndex++] = InvertYZ * TO_NDC(RightTop);

    Dest[StartingIndex++] = InvertYZ * TO_NDC(RightBottom);
    Dest[StartingIndex++] = InvertYZ * TO_NDC(RightTop);
    Dest[StartingIndex++] = InvertYZ * TO_NDC(LeftBottom);
    #undef TO_NDC

    Result.MaxClip = RightBottom.xy;
  }

  return Result;
}

function clip_result
BufferTexturedQuad(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo,
                   v2 MinP, v2 Dim,
                   debug_texture_array_slice TextureSlice, rect2 UV,
                   v3 Color,
                   r32 Z, v2 MaxClip)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  clip_result Result = BufferQuadDirect(Geo->Verts, Geo->At, MinP, Dim, Z, Group->ScreenDim, MaxClip);
  switch (Result.ClipStatus)
  {
    case ClipStatus_NoClipping:
    {
      BufferQuadUVs(Geo, UV, TextureSlice);
      BufferColors(Group, Geo, Color);
      Geo->At += 6;
    } break;

    case ClipStatus_PartialClipping:
    {
      v2 MinUvDiagonal = UV.Max - UV.Min;
      v2 MaxUvDiagonal = UV.Min - UV.Max;

      v2 MinUvModifier = MinUvDiagonal * Result.PartialClip.Min;
      v2 MaxUvModifier = MaxUvDiagonal * Result.PartialClip.Max;

      UV.Min += MinUvModifier;
      UV.Max += MaxUvModifier;

      BufferQuadUVs(Geo, UV, TextureSlice);
      BufferColors(Group, Geo, Color);
      Geo->At += 6;
    } break;

    case ClipStatus_FullyClipped:
    {
    } break;

    InvalidDefaultCase;
  }

  return Result;
}

function clip_result
BufferUntexturedQuad(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo,
                     v2 MinP, v2 Dim, v3 Color,
                     r32 Z, v2 MaxClip)
{
  if (BufferIsFull(Geo, 6))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  clip_result Result = BufferQuadDirect(Geo->Verts, Geo->At, MinP, Dim, Z, Group->ScreenDim, MaxClip);
  switch (Result.ClipStatus)
  {
    case ClipStatus_NoClipping:
    case ClipStatus_PartialClipping:
    {
      BufferColors(Group, Geo, Color);
      Geo->At += 6;
    } break;

    case ClipStatus_FullyClipped:
    {
    } break;

    InvalidDefaultCase;
  }

  return Result;
}

function r32
BufferChar(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo, u8 Char, v2 MinP, font *Font, v3 Color, r32 Z, v2 MaxClip)
{
  rect2 UV = UVsForChar(Char);

  { // Black Drop-shadow
    r32 e = zOffsetForTextShadow();
    v2 ShadowOffset = 0.1f*Font->Size;
    BufferTexturedQuad( Group, Geo,
                        MinP+ShadowOffset, Font->Size,
                        DebugTextureArraySlice_Font, UV,
                        V3(0.1f),
                        Z-e,
                        MaxClip);

  }

  clip_result ClipResult = BufferTexturedQuad( Group, Geo,
                                               MinP, Font->Size,
                                               DebugTextureArraySlice_Font, UV,
                                               Color,
                                               Z, MaxClip);

  r32 DeltaX = Font->Size.x;
  return DeltaX;
}

function r32
BufferChar(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo, u8 Char, v2 MinP, font *Font, u32 Color, r32 Z, v2 MaxClip)
{
  v3 ColorVector = GetColorData(Color).xyz;
  r32 Result = BufferChar(Group, Geo, Char, MinP, Font, ColorVector, Z, MaxClip);
  return Result;
}

function void
BufferValue(counted_string Text, debug_ui_render_group *Group, layout *Layout, v3 Color, r32 Z, v2 MaxClip, ui_style* Style = 0)
{
  textured_2d_geometry_buffer *Geo = &Group->TextGroup->TextGeo;

  v2 Padding = Style? Style->Padding : V2(0.0f);

  v2 FontHeight = V2(0, Group->Font.Size.y);
  r32 PadX = Padding.x;
  v2 PadY = V2(0, Padding.y);

  Layout->At.x += PadX;

  for ( u32 CharIndex = 0;
      CharIndex < Text.Count;
      CharIndex++ )
  {
    v2 MinP = GetAbsoluteAt(Layout) + PadY;
    Layout->At.x += BufferChar(Group, Geo, (u8)Text.Start[CharIndex], MinP, &Group->Font, Color, Z, MaxClip);
    continue;
  }

  Layout->At.x += PadX;

  v2 MaxClipP = Layout->At + (2.0f*PadY) + FontHeight;

  AdvanceClip(Layout, MaxClipP);

  return;
}

function void
BufferValue(const char* Text, debug_ui_render_group *Group, layout *Layout, v3 Color, r32 Z, v2 MaxClip, ui_style* Style = 0)
{
  BufferValue(CountedString(Text), Group, Layout, Color, Z, MaxClip, Style);
  return;
}

function void
BufferValue(const char* Text, debug_ui_render_group *Group, layout *Layout, u32 Color, r32 Z, v2 MaxClip, ui_style* Style = 0)
{
  v3 ColorVector = GetColorData(Color).xyz;
  BufferValue(Text, Group, Layout, ColorVector, Z, MaxClip, Style);
  return;
}

function void
BufferValue(r32 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%f", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function void
BufferValue(u64 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function void
BufferValue(u32 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function void
BufferThousands(u64 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip, u32 Columns = 10)
{
  char  *Buffer = FormatThousands(Number);
  u32 Len = (u32)Length(Buffer);
  u32 Pad = Max(Columns-Len, 0U);
  AdvanceSpaces(Pad, Layout, &Group->Font);
  BufferValue( Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function void
BufferMemorySize(u64 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip)
{
  char *Buffer = FormatMemorySize(Number);
  BufferValue( Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function void
BufferColumn( s32 Value, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%d", Value);
  u32 Len = (u32)Length(Buffer);
  u32 Pad = Max(ColumnWidth-Len, 0U);
  AdvanceSpaces(Pad, Layout, &Group->Font);
  BufferValue( Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function void
BufferColumn( u32 Value, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Value);
  {
    u32 Len = (u32)Length(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function void
BufferColumn( u64 Value, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Value);
  {
    u32 Len = (u32)Length(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function void
BufferColumn( r64 Perc, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%4.1lf", Perc);
  {
    u32 Len = (u32)Length(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function void
BufferColumn( r32 Perc, u32 ColumnWidth, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex, r32 Z, v2 MaxClip)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%.1f", Perc);
  {
    u32 Len = (u32)Length(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Layout, ColorIndex, Z, MaxClip);
  return;
}

function rect2
Column(counted_string Text, debug_ui_render_group* Group, layout* Layout, u32 ColumnWidth, r32 Z, v2 MaxClip, u8 Color = WHITE)
{
  u32 Pad = ColumnWidth - (u32)Text.Count;

  AdvanceSpaces(Pad, Layout, &Group->Font);

  v2 Min = Layout->Basis + Layout->At;
  v2 Max = Min + GetTextBounds((u32)Text.Count, &Group->Font);
  rect2 Bounds = RectMinMax(Min, Max);

  ui_style PadParams = {};
  PadParams.Padding = V2(10, 10);

  BufferValue(Text, Group, Layout, GetColorData(Color).xyz, Z, MaxClip, &PadParams);

  return Bounds;
}

function rect2
Column(const char* ColumnText, debug_ui_render_group* Group, table* Table, r32 Z, v2 MaxClip, u8 Color = WHITE)
{
  layout *Layout = &Table->Layout;

  table_column *Col = Table->Columns + Table->ColumnIndex;
  Table->ColumnIndex = (Table->ColumnIndex+1)%MAX_TABLE_COLUMNS;

  u32 TextLength = (u32)Length(ColumnText);
  Col->Max = Max(Col->Max, TextLength);

  rect2 Bounds = Column(CountedString(ColumnText, TextLength), Group, Layout, Col->Max, Z, MaxClip, Color);

  return Bounds;
}

function rect2
Column(const char* ColumnText, debug_ui_render_group* Group, window_layout* Window, u8 Color = WHITE)
{
  r32 Z = zIndexForText(Window, Group);
  rect2 Result = Column(ColumnText, Group, &Window->Table, Z, GetAbsoluteMaxClip(Window), Color);
  return Result;
}

function r32
BufferTextAt(debug_ui_render_group *Group, v2 BasisP, const char *Text, u32 Color, r32 Z, v2 ClipMax = DISABLE_CLIPPING)
{
  textured_2d_geometry_buffer *Geo = &Group->TextGroup->TextGeo;

  u32 QuadCount = (u32)Length(Text);

  r32 DeltaX = 0;

  for ( u32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = BasisP + V2(Group->Font.Size.x*CharIndex, 0);
    DeltaX += BufferChar(Group, Geo, (u8)Text[CharIndex], MinP, &Group->Font, Color, Z, ClipMax);
    continue;
  }

  return DeltaX;
}

function void
DoTooltip(debug_ui_render_group *Group, const char *Text, r32 Z = 1.0f)
{
  BufferTextAt(Group, *Group->MouseP+V2(12, -7), Text, WHITE, Z);
  return;
}

function void
BufferRectangleAt(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo,
                  v2 MinP, v2 Dim, v3 Color, r32 Z, v2 MaxClip)
{
  BufferUntexturedQuad(Group, Geo, MinP, Dim, Color, Z, MaxClip);
  return;
}

function void
BufferRectangleAt(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo,
                         rect2 Rect, v3 Color, r32 Z, v2 MaxClip)
{
  v2 MinP = Rect.Min;
  v2 Dim = Rect.Max - Rect.Min;
  BufferRectangleAt(Group, Geo, MinP, Dim, Color, Z, MaxClip);

  return;
}

function void
Border(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, rect2 Rect, v3 Color, r32 Z, v2 MaxClip)
{
  v2 TopLeft     = Rect.Min;
  v2 BottomRight = Rect.Max;
  v2 TopRight    = V2(Rect.Max.x, Rect.Min.y);
  v2 BottomLeft  = V2(Rect.Min.x, Rect.Max.y);

  rect2 TopRect    = RectMinMax(TopLeft ,    TopRight    - V2(0, 1));
  rect2 BottomRect = RectMinMax(BottomLeft,  BottomRight + V2(0, 1));
  rect2 LeftRect   = RectMinMax(TopLeft ,    BottomLeft  - V2(1, 0));
  rect2 RightRect  = RectMinMax(TopRight,    BottomRight + V2(1, 0));

  BufferRectangleAt(Group, Geo, TopRect,    Color, Z, MaxClip);
  BufferRectangleAt(Group, Geo, LeftRect,   Color, Z, MaxClip);
  BufferRectangleAt(Group, Geo, RightRect,  Color, Z, MaxClip);
  BufferRectangleAt(Group, Geo, BottomRect, Color, Z, MaxClip);

  return;
}

function void
Border(debug_ui_render_group *Group, interactable* PickerListInteraction, v3 Color, r32 Z, v2 MaxClip)
{
  rect2 Bounds = RectMinMax(PickerListInteraction->MinP, PickerListInteraction->MaxP);
  Border(Group, &Group->TextGroup->UIGeo, Bounds, Color, Z, MaxClip);
  return;
}



/*********************************           *********************************/
/*********************************  Buttons  *********************************/
/*********************************           *********************************/



function button_interaction_result
ButtonInteraction(debug_ui_render_group* Group, rect2 Bounds, umm InteractionId, window_layout *Window, ui_style *Style = 0)
{
  button_interaction_result Result = {};
  Result.Color = V3(1);

  if (Style)
  {
    Bounds.Max += (Style->Padding*2.0f);
    Result.Color = Style->Color;
  }

  interactable Interaction = Interactable(Bounds, InteractionId, Window);

  Border(Group, &Interaction, V3(1,0,0), 1.0f, DISABLE_CLIPPING);

  if (Hover(Group, &Interaction))
  {
    Result.Hover = True;
    if (Style)
    {
      Result.Color = Style->HoverColor;
    }
  }

  if (Pressed(Group, &Interaction))
  {
    Result.Pressed = True;
    if (Style)
    {
      Result.Color = Style->ClickColor;
    }
  }

  if (Clicked(Group, &Interaction))
  {
    Result.Clicked = True;
    if (Style)
    {
      Result.Color = Style->ClickColor;
    }
  }

  if (Style && Style->IsActive && !Result.Pressed)
  {
    Result.Color = Style->ActiveColor;
  }

  return Result;
}

function b32
Button(debug_ui_render_group* Group, rect2 Bounds, umm InteractionId, r32 Z, v2 MaxClip, window_layout* Window, ui_style* Style)
{
  button_interaction_result Result = ButtonInteraction(Group, Bounds, InteractionId, Window, Style);
  BufferRectangleAt(Group, &Group->TextGroup->UIGeo, Bounds, Result.Color, Z, MaxClip);
  return Result.Pressed;
}

function b32
Button(const char* ColumnText, debug_ui_render_group *Group, layout* Layout, umm InteractionId, r32 Z, v2 MaxClip, window_layout* Window, ui_style* Style = 0)
{
  v2 Min = GetAbsoluteAt(Layout);
  v2 Max = Min + GetTextBounds( (u32)Length(ColumnText), &Group->Font);
  rect2 Bounds = RectMinMax(Min, Max);

  button_interaction_result Result = ButtonInteraction(Group, Bounds, InteractionId, Window, Style);

  BufferValue(ColumnText, Group, Layout, Result.Color, Z, MaxClip, Style);

  return Result.Pressed;
}

function b32
Button(const char* ButtonText, debug_ui_render_group *Group, window_layout* Window, ui_style* Style = 0, umm InteractionIdModifier = 0)
{
  umm InteractionId = (umm)ButtonText^(umm)Window;
  InteractionId = InteractionIdModifier? InteractionId^InteractionIdModifier : InteractionId;

  r32 Z = zIndexForText(Window, Group);
  b32 Result = Button(ButtonText, Group, &Window->Table.Layout, InteractionId, Z, GetAbsoluteMaxClip(Window), Window, Style);
  return Result;
}



/*********************************           *********************************/
/*********************************  Windows  *********************************/
/*********************************           *********************************/



function void
RenderWindowInteractions(debug_ui_render_group* Group, window_layout* Window, layout* Layout)
{
  Clear(&Window->Table.Layout.At);
  Clear(&Window->Table.Layout.DrawBounds);

  v2 TitleBounds = GetTextBounds( (u32)Length(Window->Title), &Group->Font);
  Window->MaxClip = Max(TitleBounds, Window->MaxClip);

  rect2 WindowBounds = GetWindowBounds(Window);

  umm DragHandleId = (umm)"WindowResizeWidget"^(umm)Window;
  interactable DragHandle = Interactable( Rect2(0), DragHandleId, Window);
  if (Pressed(Group, &DragHandle))
  {
    v2 AbsoluteTitleBounds = Layout->Basis + TitleBounds;
    v2 TestMaxClip = *Group->MouseP - Layout->Basis;

    if (Group->MouseP->x > AbsoluteTitleBounds.x )
    {
      Window->MaxClip.x = Max(TitleBounds.x, TestMaxClip.x);
    }
    else
    {
      Window->MaxClip.x = TitleBounds.x;
    }

    if (Group->MouseP->y > AbsoluteTitleBounds.y )
    {
      Window->MaxClip.y = Max(TitleBounds.y, TestMaxClip.y);
    }
    else
    {
      Window->MaxClip.y = TitleBounds.y;
    }

    WindowBounds = GetWindowBounds(Window);
  }

  umm TitleBarInteractionId = (umm)"WindowTitleBar"^(umm)Window;
  interactable TitleBarInteraction = Interactable( Rect2(0), TitleBarInteractionId, Window);
  if (Pressed(Group, &TitleBarInteraction))
  {
    Layout->Basis -= *Group->MouseDP; // TODO(Jesse): Can we compute this with MouseP to avoid a frame of input delay?
    WindowBounds = GetWindowBounds(Window);
  }

  {
    ui_style DragHandleStyle = StandardStyling(V3(1));
    v2 Dim = V2(10);
    v2 MinP = Layout->Basis + Window->MaxClip - Dim;
    rect2 DragHandleRect = RectMinDim(MinP, Dim);
    Button(Group, DragHandleRect, DragHandleId, zIndexForBorders(Window, Group), WindowBounds.Max, Window, &DragHandleStyle);
  }

  {
    ui_style BackgroundStyle = StandardStyling(V3(0.0f, 0.5f, 0.5f));
    rect2 TitleBarRect = RectMinMax(WindowBounds.Min, TopRight(WindowBounds) + V2(0.0f, Group->Font.Size.y));
    Button(Group, TitleBarRect, TitleBarInteractionId, zIndexForTitleBar(Window, Group), WindowBounds.Max, Window, &BackgroundStyle);
  }

  if (Window->Title)
  {
    BufferValue(Window->InteractionStackIndex, Group, Layout, WHITE, zIndexForText(Window, Group), WindowBounds.Max);
    AdvanceSpaces(1, Layout, &Group->Font);
    BufferValue(Window->Title, Group, Layout, V3(1), zIndexForText(Window, Group), WindowBounds.Max);
    NewRow(Window);
  }


  {
    r32 Z = zIndexForBorders(Window, Group);
    v3 BorderColor = V3(1, 1, 1);
    untextured_2d_geometry_buffer *UiGeo = &Group->TextGroup->UIGeo;
    Border(Group, UiGeo, WindowBounds, BorderColor, Z, DISABLE_CLIPPING);
  }

  {
    v3 BackgroundColor = V3(0.1f, 0.1f, 0.1f);
    r32 Z = zIndexForBackgrounds(Window, Group);
    untextured_2d_geometry_buffer *UiGeo = &Group->TextGroup->UIGeo;
    BufferRectangleAt(Group, UiGeo, WindowBounds, BackgroundColor, Z, WindowBounds.Max);
  }

  return;
}



/****************************                     ****************************/
/****************************  Command Buffering  ****************************/
/****************************                     ****************************/


// TODO(Jesse): Test this actually gets respected!!
// @respect_invalid_render_command_index
#define INVALID_RENDER_COMMAND_INDEX (UINT_MAX)

function u32
PushUiRenderCommand(debug_ui_render_group *Group, ui_render_command* Command)
{
  ui_render_command_buffer *CommandBuffer = &Group->CommandBuffer;

  u32 Result = CommandBuffer->CommandCount;

  if (CommandBuffer->CommandCount < MAX_UI_RENDER_COMMAND_COUNT)
  {
    CommandBuffer->Commands[CommandBuffer->CommandCount++] = *Command;
  }
  else
  {
    // TODO(Jesse): Test this actually gets respected!!
    // @respect_invalid_render_command_index
    Result = INVALID_RENDER_COMMAND_INDEX;
    Error("Exhausted RenderCommandBuffer Space!");
  }

  return Result;
}

function void
PushNewRow(debug_ui_render_group *Group)
{
  ui_render_command Command = {
    .Type = RenderCommand_NewRow
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

function void
PushColumn(debug_ui_render_group *Group, counted_string String)
{
  ui_render_command Command = {
    .Type = RenderCommand_Column,
    .Column.String = String
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

function u32
PushButtonEnd(debug_ui_render_group *Group)
{
  ui_render_command Command = { .Type = RenderCommand_ButtonEnd };
  u32 OnePastEndIndex = PushUiRenderCommand(Group, &Command) + 1;
  return OnePastEndIndex;
}

function interactable_handle
PushButtonStart(debug_ui_render_group *Group)
{
  ui_render_command Command = {
    .Type = RenderCommand_ButtonStart
  };

  u32 ButtonStartIndex = PushUiRenderCommand(Group, &Command);

  interactable_handle Handle = {
    .StartIndex = ButtonStartIndex
  };

  return Handle;
}

function void
PushTableEnd(debug_ui_render_group *Group)
{
  ui_render_command Command = {
    .Type = RenderCommand_TableEnd,
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

function void
PushTableStart(debug_ui_render_group *Group, table_handle *Handle)
{
  ui_render_command Command = {};
  Command.Type = RenderCommand_TableStart;
  Command.Table.Handle = *Handle;

  PushUiRenderCommand(Group, &Command);

  return;
}

function void
PushWindowInteraction(debug_ui_render_group *Group, window_layout *Window)
{
  ui_render_command Command = {};
  Command.Type = RenderCommand_WindowInteractions;
  Command.WindowInteraction.Window = Window;

  PushUiRenderCommand(Group, &Command);
  return;
}

function window_layout*
GetHighestWindow(debug_ui_render_group* Group, ui_render_command_buffer* CommandBuffer)
{
  u64 HighestInteractionStackIndex = 0;
  window_layout* HighestWindow = 0;

  for (u32 CommandIndex = 0;
      CommandIndex < CommandBuffer->CommandCount;
      ++CommandIndex)
  {
    ui_render_command *Command = CommandBuffer->Commands+CommandIndex;
    switch(Command->Type)
    {
      case RenderCommand_WindowInteractions:
      {
        window_layout *TestWindow = Command->WindowInteraction.Window;
        b32 InsideWindowBounds = IsInsideRect(GetWindowBounds(TestWindow), *Group->MouseP);
        b32 FoundNewHighestStackIndex = HighestInteractionStackIndex <= TestWindow->InteractionStackIndex;
        if ( InsideWindowBounds && FoundNewHighestStackIndex )
        {
          HighestWindow = TestWindow;
          HighestInteractionStackIndex = TestWindow->InteractionStackIndex;
        }

      } break;

      default: {} break;
    }
  }

  b32 Clicked = (Group->Input->LMB.Clicked || Group->Input->RMB.Clicked);
  if (Clicked && HighestWindow)
  {
    HighestWindow->InteractionStackIndex = ++Group->InteractionStackTop;
  }

  return HighestWindow;
}

function ui_render_command*
GetCommand(ui_render_command_buffer* CommandBuffer, u32 CommandIndex)
{
  ui_render_command* Command = 0;
  if (CommandIndex < CommandBuffer->CommandCount)
    { Command = CommandBuffer->Commands+CommandIndex; }
  return Command;
}

// TODO(Jesse): Bulletproof this such that we can have any number of columns!
// @max_column_widths
#define MAX_COLUMN_WIDTHS (128)
static u32 ColumnWidths[MAX_COLUMN_WIDTHS] = {};

function u32
RenderTable(debug_ui_render_group* Group, ui_render_command_buffer* CommandBuffer, u32 FirstCommandIndex)
{
  u32 Result = 0;

  u32 RowCount = 0;
  u32 ColumnCount = 0;
  u32 MaxColumnCount = 0;

  {
    u32 CommandIndex = FirstCommandIndex;
    ui_render_command* Command =  GetCommand(CommandBuffer, CommandIndex++);
    Assert(Command && Command->Type == RenderCommand_TableStart);

    while (Command && !Result)
    {
      switch(Command->Type)
      {
          case RenderCommand_Column:
          {
            // @max_column_widths
            Assert(ColumnCount < MAX_COLUMN_WIDTHS);
            ColumnWidths[ColumnCount] = Max((u32)Command->Column.String.Count, ColumnWidths[ColumnCount]);
            ++ColumnCount;
          } break;

          case RenderCommand_NewRow:
          {
            MaxColumnCount = Max(MaxColumnCount, ColumnCount);
            ColumnCount = 0;
            RowCount++;
          } break;

          case RenderCommand_ButtonStart:
          {
          } break;

          case RenderCommand_ButtonEnd:
          {
          } break;

          case RenderCommand_TableEnd:
          {
            Result = CommandIndex;
          } break;

          // TODO(Jesse): Nested tables!
          case RenderCommand_TableStart: {} break;

          InvalidDefaultCase;
      }

      Command =  GetCommand(CommandBuffer, CommandIndex++);
    }
  }

  if (Result)
  {
    u32 ColumnIndex = 0;

    window_layout* Window = 0;

    u32 CommandIndex = FirstCommandIndex;
    while (CommandIndex < Result)
    {
      ui_render_command* Command = GetCommand(CommandBuffer, CommandIndex++);
      switch(Command->Type)
      {
          case RenderCommand_TableStart:
          {
            Window = Command->Table.Handle.Window;
          } break;

          case RenderCommand_Column:
          {
            if(Window)
            {
              Column(Command->Column.String, Group, &Window->Table.Layout, ColumnWidths[ColumnIndex], zIndexForText(Window, Group), GetAbsoluteMaxClip(Window));
            }
            else
            {
              Error("No Window found to render column text!");
            }

            ++ColumnIndex;
          } break;

          case RenderCommand_NewRow:
          {
            ColumnIndex = 0;
            NewLine(&Window->Table.Layout);
          } break;

          case RenderCommand_ButtonStart:
          {
          } break;

          case RenderCommand_ButtonEnd:
          {
          } break;

          case RenderCommand_TableEnd:
          {
            Assert(CommandIndex == Result);
          } break;

          InvalidDefaultCase;
        }
    }
  }
  else
  {
    Error("Command tree parse error while rendering table");
  }

  return Result;
}

function void
FlushCommandBuffer(debug_ui_render_group *Group, ui_render_command_buffer *CommandBuffer)
{
  for (u32 CommandIndex = 0;
      CommandIndex < CommandBuffer->CommandCount;
      ++CommandIndex)
  {
    ui_render_command *Command = CommandBuffer->Commands+CommandIndex;
    switch(Command->Type)
    {
      case RenderCommand_WindowInteractions:
      {
        window_layout* Window = Command->WindowInteraction.Window;
        RenderWindowInteractions(Group, Window, &Window->Table.Layout);
      } break;

      case RenderCommand_TableStart:
      {
        CommandIndex = RenderTable(Group, CommandBuffer, CommandIndex);
      } break;

      InvalidDefaultCase;
    }
  }

  CommandBuffer->CommandCount = 0;

  return;
}



/****************************                       **************************/
/****************************  Mutex Introspection  **************************/
/****************************                       **************************/



function b32
DrawCycleBar( cycle_range *Range, cycle_range *Frame, r32 TotalGraphWidth, const char *Tooltip, v3 Color,
              debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, layout *Layout, r32 Z, v2 MaxClip)
{
  Assert(Frame->StartCycle < Range->StartCycle);

  b32 Result = False;

  r32 FramePerc = (r32)Range->TotalCycles / (r32)Frame->TotalCycles;

  r32 BarHeight = Group->Font.Size.y;
  r32 BarWidth = FramePerc*TotalGraphWidth;
  if (BarWidth > 0.1f)
  {
    v2 BarDim = V2(BarWidth, BarHeight);

    // Advance to the appropriate starting place along graph
    u64 StartCycleOffset = Range->StartCycle - Frame->StartCycle;
    r32 XOffset = GetXOffsetForHorizontalBar(StartCycleOffset, Frame->TotalCycles, TotalGraphWidth);

    v2 MinP = Layout->At + Layout->Basis + V2(XOffset, 0);

    interactable Interaction = Interactable(MinP, MinP+BarDim, (umm)"CycleBarHoverInteraction", 0);
    Result = Hover(Group, &Interaction);
    if (Result)
    {
      Color *= 0.5f;
      if (Tooltip) { DoTooltip(Group, Tooltip); }
    }

    clip_result Clip = BufferUntexturedQuad(Group, Geo, MinP, BarDim, Color, Z, MaxClip);
    if (Clip.ClipStatus != ClipStatus_FullyClipped)
    {
      AdvanceClip(Layout, Clip.MaxClip - Layout->Basis);
    }
  }

  return Result;
}

function void
DrawWaitingBar(mutex_op_record *WaitRecord, mutex_op_record *AquiredRecord, mutex_op_record *ReleasedRecord,
               debug_ui_render_group *Group, layout *Layout, u64 FrameStartingCycle, u64 FrameTotalCycles, r32 TotalGraphWidth, r32 Z, v2 MaxClip)
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
  DrawCycleBar( &WaitRange, &FrameRange, TotalGraphWidth, 0, V3(1, 0, 0), Group, Geo, Layout, Z, MaxClip);

  cycle_range AquiredRange = {AquiredRecord->Cycle, AquiredCycleCount};
  DrawCycleBar( &AquiredRange, &FrameRange, TotalGraphWidth, 0, V3(0, 1, 0), Group, Geo, Layout, Z, MaxClip);

  return;
}



/****************************                 ********************************/
/****************************  Picked Chunks  ********************************/
/****************************                 ********************************/



function void
ComputePickRay(platform *Plat, m4* ViewProjection)
{
  debug_state *DebugState = GetDebugState();

  m4 InverseViewProjection = {};
  b32 Inverted = Inverse((r32*)ViewProjection, (r32*)&InverseViewProjection);
  Assert(Inverted);

  v3 MouseMinWorldP = Unproject( Plat->MouseP,
                                 0.0f,
                                 V2(Plat->WindowWidth, Plat->WindowHeight),
                                 &InverseViewProjection);

  v3 MouseMaxWorldP = Unproject( Plat->MouseP,
                                 1.0f,
                                 V2(Plat->WindowWidth, Plat->WindowHeight),
                                 &InverseViewProjection);

  v3 RayDirection = Normalize(MouseMaxWorldP - MouseMinWorldP);

  DebugState->PickRay = { MouseMinWorldP, RayDirection };

  if (DebugState->DoChunkPicking)
  {
    DebugState->PickedChunkCount = 0;
    DebugState->HotChunk = 0;
  }

  return;
}

function void
BufferChunkDetails(debug_ui_render_group* Group, world_chunk* Chunk, window_layout* Window)
{
  Column("WorldP", Group, Window, WHITE);
  Column(ToString(Chunk->WorldP.x), Group, Window, WHITE);
  Column(ToString(Chunk->WorldP.y), Group, Window, WHITE);
  Column(ToString(Chunk->WorldP.z), Group, Window, WHITE);
  NewRow(Window);

  Column("PointsToLeaveRemaining", Group, Window, WHITE);
  Column(ToString(Chunk->PointsToLeaveRemaining), Group, Window, WHITE);
  NewRow(Window);

  Column("BoundaryVoxels Count", Group, Window, WHITE);
  Column(ToString(Chunk->EdgeBoundaryVoxelCount), Group, Window, WHITE);
  NewRow(Window);

  Column("Triangles", Group, Window, WHITE);
  Column(ToString(Chunk->TriCount), Group, Window, WHITE);
  NewRow(Window);

  return;
}

function table_handle
StartTableBelow(debug_ui_render_group* Group, window_layout* Src)
{
  v2 Basis = Src->Table.Layout.Basis + V2(Src->Table.Layout.DrawBounds.Min.x, Src->Table.Layout.At.y);
  table_handle Result = {.Window = Src};

  PushTableStart(Group, &Result);

  return Result;
}

function void
EndTable(debug_ui_render_group* Group)
{
  PushTableEnd(Group);
  return;
}

function interactable_handle
StartButton(debug_ui_render_group* Group)
{
  interactable_handle Result = PushButtonStart(Group);
  return Result;
}

function void
EndButton(debug_ui_render_group* Group, interactable_handle* Handle)
{
  Handle->OnePastEndIndex = PushButtonEnd(Group);
  return;
}

function void
NewRow(debug_ui_render_group* Group)
{
  PushNewRow(Group);
}

function void
DrawPickedChunks(debug_ui_render_group* Group, v2 LayoutBasis)
{
  debug_state* DebugState = GetDebugState();
  world_chunk** PickedChunks = DebugState->PickedChunks;
  MapGpuElementBuffer(&DebugState->GameGeo);

  local_persist window_layout ListingWindow = WindowLayout("Picked Chunks", LayoutBasis, V2(400, 150));
  PushWindowInteraction(Group, &ListingWindow);

  table_handle PickerTable = StartTableBelow(Group, &ListingWindow);

  for (u32 ChunkIndex = 0;
      ChunkIndex < DebugState->PickedChunkCount;
      ++ChunkIndex)
  {
    world_chunk *Chunk = PickedChunks[ChunkIndex];

    r32 Z = zIndexForText(&ListingWindow, Group);
    v2 Clip = GetAbsoluteMaxClip(&ListingWindow);
    u8 Color = WHITE;

    interactable_handle PositionButton = StartButton(Group);
      PushColumn(Group, AsString(Chunk->WorldP.x) );
      PushColumn(Group, AsString(Chunk->WorldP.y) );
      PushColumn(Group, AsString(Chunk->WorldP.z) );
    EndButton(Group, &PositionButton);

    if (Clicked(Group, &PositionButton)) { DebugState->HotChunk = Chunk; }

    ui_style ButtonStyling = {};
    ButtonStyling.Color = V3(1,0,0);

    interactable_handle CloseButton = StartButton(Group);
      PushColumn(Group, CountedString("X"));
    EndButton(Group, &CloseButton);

    if ( Clicked(Group, &CloseButton) )
    {
      world_chunk** SwapChunk = PickedChunks+ChunkIndex;
      if (*SwapChunk == DebugState->HotChunk) { DebugState->HotChunk = 0; }
      *SwapChunk = PickedChunks[--DebugState->PickedChunkCount];
    }

    NewRow(Group);
  }

  EndTable(Group);

  world_chunk *HotChunk = DebugState->HotChunk;
  if (HotChunk)
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

  if (HotChunk)
  {
    v2 WindowSpacing = V2(140, 0);

    local_persist window_layout ChunkDetailWindow = WindowLayout("Chunk Details",
                                                                  V2(GetAbsoluteMaxClip(&ListingWindow).x, GetAbsoluteMin(&ListingWindow).y) + WindowSpacing,
                                                                  V2(1100.0f, 400.0f));

    PushWindowInteraction(Group, &ChunkDetailWindow);

    BufferChunkDetails(Group, HotChunk, &ChunkDetailWindow);

    local_persist window_layout PickerWindow = WindowLayout("Chunk View",
                                                            V2(GetAbsoluteMaxClip(&ChunkDetailWindow).x, GetAbsoluteMin(&ChunkDetailWindow).y) + WindowSpacing,
                                                            V2(800.0f));
    PushWindowInteraction(Group, &PickerWindow);

    b32 DebugButtonPressed = False;
    // FIXME(Jesse): This is dependant on framerate and the button will be triggered on each frame!  Yikes.
    if (Button("<", Group, &PickerWindow))
    {
      HotChunk->PointsToLeaveRemaining = Min(HotChunk->PointsToLeaveRemaining+1, HotChunk->EdgeBoundaryVoxelCount);
      DebugButtonPressed = True;
    }

    // FIXME(Jesse): This is dependant on framerate and the button will be triggered on each frame!  Yikes.
    if (Button(">", Group, &PickerWindow))
    {
      HotChunk->PointsToLeaveRemaining = Max(HotChunk->PointsToLeaveRemaining-1, 0);
      DebugButtonPressed = True;
    }

    const char* ButtonText = HotChunk->DrawBoundingVoxels ? "|" : "O";
    if (Button(ButtonText, Group, &PickerWindow))
    {
      HotChunk->DrawBoundingVoxels = !HotChunk->DrawBoundingVoxels;
      DebugButtonPressed = True;
    }

    if (DebugButtonPressed)
    {
      HotChunk->LodMesh_Complete = False;
      HotChunk->LodMesh->At = 0;
      HotChunk->Mesh = 0;
      HotChunk->FilledCount = 0;
      HotChunk->Data->Flags = Chunk_Uninitialized;
      QueueChunkForInit( DebugState->GameState, &DebugState->Plat->HighPriority, HotChunk);
    }

    NewRow(&PickerWindow);

    v2 MinP = GetAbsoluteAt(&PickerWindow.Table.Layout);
    v2 QuadDim = PickerWindow.MaxClip - PickerWindow.Table.Layout.At;

    r32 ChunkDetailZ = zIndexForText(&PickerWindow, Group);
    BufferTexturedQuad( Group, &Group->TextGroup->TextGeo, MinP, QuadDim,
                        DebugTextureArraySlice_Viewport, UVsForFullyCoveredQuad(),
                        V3(1), ChunkDetailZ, GetAbsoluteMaxClip(&PickerWindow));

    interactable Interaction = Interactable(MinP, MinP+QuadDim, (umm)"PickerWindowDragInteraction", &PickerWindow);
    input* WindowInput = Group->Input;
    if (!Pressed(Group, &Interaction))
    {
      WindowInput = 0;
    }

    UpdateGameCamera( -0.005f*(*Group->MouseDP),
                      WindowInput,
                      Canonical_Position(0),
                      &DebugState->Camera);
  }

  return;
}



/************************                        *****************************/
/************************  Thread Perf Bargraph  *****************************/
/************************                        *****************************/



function void
BufferScopeTreeEntry(debug_ui_render_group *Group, debug_profile_scope *Scope, window_layout* Window,
                     u8 Color, u64 TotalCycles, u64 TotalFrameCycles, u64 CallCount, u32 Depth)
{
  Assert(TotalFrameCycles);

  r32 Percentage = 100.0f * (r32)SafeDivide0((r64)TotalCycles, (r64)TotalFrameCycles);
  u64 AvgCycles = (u64)SafeDivide0(TotalCycles, CallCount);

  r32 Z = zIndexForText(Window, Group);

  Column(ToString(Percentage), Group, &Window->Table, Z, DISABLE_CLIPPING);
  Column(ToString(AvgCycles),  Group, &Window->Table, Z, DISABLE_CLIPPING);
  Column(ToString(CallCount),  Group, &Window->Table, Z, DISABLE_CLIPPING);

  layout *Layout = &Window->Table.Layout;
  AdvanceSpaces((Depth*2)+1, Layout, &Group->Font);

  if (Scope->Expanded && Scope->Child)
  {
    BufferValue("-", Group, Layout, Color, Z, DISABLE_CLIPPING);
  }
  else if (Scope->Child)
  {
    BufferValue("+", Group, Layout, Color, Z, DISABLE_CLIPPING);
  }
  else
  {
    AdvanceSpaces(1, Layout, &Group->Font);
  }

  BufferValue(Scope->Name, Group, Layout, Color, Z, DISABLE_CLIPPING);
  NewRow(Window);

  return;
}

#if 0
function scope_stats
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

function void
DrawScopeBarsRecursive(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, debug_profile_scope *Scope,
                       layout *Layout, cycle_range *Frame, r32 TotalGraphWidth, random_series *Entropy, r32 Z, v2 MaxClip)
{
  while (Scope)
  {
    Assert(Scope->Name);

    cycle_range Range = {Scope->StartingCycle, Scope->CycleCount};
    v3 Color = RandomV3(Entropy);

    b32 Hovering = DrawCycleBar( &Range, Frame, TotalGraphWidth, Scope->Name, Color, Group, Geo, Layout, Z, MaxClip);

    if (Hovering && Group->Input->LMB.Clicked)
      Scope->Expanded = !Scope->Expanded;

    if (Scope->Expanded)
    {
      Layout->At.y += Group->Font.Size.y;
      DrawScopeBarsRecursive(Group, Geo, Scope->Child, Layout, Frame, TotalGraphWidth, Entropy, Z, MaxClip);
      AdvanceClip(Layout);
      Layout->At.y -= Group->Font.Size.y;
    }

    Scope = Scope->Sibling;
  }

  return;
}

function void
DebugDrawCycleThreadGraph(debug_ui_render_group *Group, debug_state *SharedState, v2 BasisP)
{
  random_series Entropy = {};
  r32 TotalGraphWidth = 1500.0f;
  untextured_2d_geometry_buffer *Geo = &Group->TextGroup->UIGeo;


  local_persist window_layout CycleGraphWindow = WindowLayout("Cycle Graph", BasisP);

  // TODO(Jesse): Call this for CycleGraphWindow!!
  // PushWindowInteraction()

  layout* Layout = &CycleGraphWindow.Table.Layout;

  SetFontSize(&Group->Font, 30);

  r32 MinY = Layout->Basis.y + Layout->At.y;

  u32 TotalThreadCount                = GetTotalThreadCount();
  frame_stats *FrameStats             = SharedState->Frames + SharedState->ReadScopeIndex;
  cycle_range FrameCycles             = {FrameStats->StartingCycle, FrameStats->TotalCycles};

  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree    = MainThreadState->ScopeTrees + SharedState->ReadScopeIndex;

  r32 Z = zIndexForText(&CycleGraphWindow, Group);
  for ( u32 ThreadIndex = 0;
        ThreadIndex < TotalThreadCount;
        ++ThreadIndex)
  {
    char *ThreadName = FormatString(TranArena, "Thread %u", ThreadIndex);
    Column(ThreadName, Group, &CycleGraphWindow.Table, Z, DISABLE_CLIPPING);
    NewRow(&CycleGraphWindow);

    debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
    debug_scope_tree *ReadTree = ThreadState->ScopeTrees + SharedState->ReadScopeIndex;
    if (MainThreadReadTree->FrameRecorded == ReadTree->FrameRecorded)
    {
      DrawScopeBarsRecursive(Group, Geo, ReadTree->Root, Layout, &FrameCycles, TotalGraphWidth, &Entropy, Z, DISABLE_CLIPPING);
    }

    NewRow(&CycleGraphWindow);
  }

  NewLine(Layout);

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
      BufferUntexturedQuad(Group, Geo, MinP16ms, Dim, V3(0,1,0), Z, DISABLE_CLIPPING);
    }
    {
      r32 FramePerc = 33.333333f/TotalMs;
      r32 xOffset = FramePerc*TotalGraphWidth;
      v2 MinP16ms = Layout->Basis + V2( xOffset, MinY );
      v2 MaxP16ms = Layout->Basis + V2( xOffset+MarkerWidth, Layout->At.y );
      v2 Dim = MaxP16ms - MinP16ms;
      BufferUntexturedQuad(Group, Geo, MinP16ms, Dim, V3(0,1,1), Z, DISABLE_CLIPPING);
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



#define MAX_RECORDED_FUNCTION_CALLS 256
static called_function ProgramFunctionCalls[MAX_RECORDED_FUNCTION_CALLS];
static called_function NullFunctionCall = {};

function void
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
      s32 SwapIndex = MAX_RECORDED_FUNCTION_CALLS;
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

function unique_debug_profile_scope *
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

function void
BufferFirstCallToEach(debug_ui_render_group *Group,
    debug_profile_scope *Scope_in, debug_profile_scope *TreeRoot,
    memory_arena *Memory, window_layout* CallgraphWindow, u64 TotalFrameCycles, u32 Depth)
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
    interactable ScopeTextInteraction = StartInteractable(&CallgraphWindow->Table.Layout, (umm)UniqueScopes->Scope, 0);
      BufferScopeTreeEntry(Group, UniqueScopes->Scope, CallgraphWindow, WHITE, UniqueScopes->TotalCycles, TotalFrameCycles, UniqueScopes->CallCount, Depth);
    EndInteractable(CallgraphWindow, &ScopeTextInteraction);

    if (UniqueScopes->Scope->Expanded)
      BufferFirstCallToEach(Group, UniqueScopes->Scope->Child, TreeRoot, Memory, CallgraphWindow, TotalFrameCycles, Depth+1);

    if (Clicked(Group, &ScopeTextInteraction))
    {
      UniqueScopes->Scope->Expanded = !UniqueScopes->Scope->Expanded;
    }

    UniqueScopes = UniqueScopes->NextUnique;
  }

  return;
}


function rect2
DebugDrawCallGraph(debug_ui_render_group *Group, debug_state *DebugState, layout *MainLayout, r64 MaxMs)
{
  NewLine(MainLayout);
  SetFontSize(&Group->Font, 80);

  // TODO(Jesse): Factor this such that we've got a window to do a real Z computation!
  r32 Z = 1.0f;

  // TODO(Jesse): Factor this such that we've got a window to do a real clipping computation!
  v2 MaxClip = V2(FLT_MAX);

  TIMED_BLOCK("Frame Ticker");
    v2 StartingAt = MainLayout->At;

    for (u32 FrameIndex = 0;
        FrameIndex < DEBUG_FRAMES_TRACKED;
        ++FrameIndex )
    {
      frame_stats *Frame = DebugState->Frames + FrameIndex;
      r32 Perc = (r32)SafeDivide0(Frame->FrameMs, MaxMs);

      v2 MinP = MainLayout->At;
      v2 MaxDim = V2(15.0, Group->Font.Size.y);
      v2 MaxP = MinP + MaxDim;

      v3 Color = V3(0.5f, 0.5f, 0.5f);

      debug_scope_tree *Tree = GetThreadLocalStateFor(0)->ScopeTrees + FrameIndex;
      if ( Tree == DebugState->GetWriteScopeTree() )
      {
        Color = V3(0.8f, 0.0f, 0.0f);
        Perc = 0.05f;
      }

      if ( Tree == DebugState->GetReadScopeTree(0) )
        Color = V3(0.8f, 0.8f, 0.0f);

      v2 QuadDim = MaxDim * V2(1.0f, Perc);
      v2 VerticalOffset = MaxDim - QuadDim;

      interactable Interaction = Interactable(MinP, MaxP, (umm)"CallGraphBarInteract", 0);
      if (Hover(Group, &Interaction))
      {
        debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
        if (FrameIndex != MainThreadState->WriteIndex % DEBUG_FRAMES_TRACKED)
        {
          DebugState->ReadScopeIndex = FrameIndex;
          Color = V3(0.8f, 0.8f, 0.0f);
        }
      }

      clip_result Clip = BufferUntexturedQuad(Group, &Group->TextGroup->UIGeo, MinP + VerticalOffset, QuadDim, Color, Z, MaxClip);
      if (Clip.ClipStatus != ClipStatus_FullyClipped)
      {
        MainLayout->At.x = Clip.MaxClip.x + 5.0f;
        AdvanceClip(MainLayout, MainLayout->At + V2(0, Group->Font.Size.y));
      }
    }


    r32 MaxBarHeight = Group->Font.Size.y;
    v2 QuadDim = V2(MainLayout->At.x, 2.0f);
    {
      r32 MsPerc = (r32)SafeDivide0(33.333, MaxMs);
      r32 MinPOffset = MaxBarHeight * MsPerc;
      v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size.y - MinPOffset };

      BufferUntexturedQuad(Group, &Group->TextGroup->UIGeo, MinP, QuadDim, V3(1,1,0), Z, MaxClip);
    }

    {
      r32 MsPerc = (r32)SafeDivide0(16.666, MaxMs);
      r32 MinPOffset = MaxBarHeight * MsPerc;
      v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size.y - MinPOffset };

      BufferUntexturedQuad(Group, &Group->TextGroup->UIGeo, MinP, QuadDim, V3(0,1,0), Z, MaxClip);
    }

    { // Current ReadTree info
      SetFontSize(&Group->Font, 30);
      frame_stats *Frame = DebugState->Frames + DebugState->ReadScopeIndex;
      BufferColumn(Frame->FrameMs, 4, Group, MainLayout, WHITE, Z, MaxClip);
      BufferThousands(Frame->TotalCycles, Group, MainLayout, WHITE, Z, MaxClip);

      u32 TotalMutexOps = GetTotalMutexOpsForReadFrame();
      BufferThousands(TotalMutexOps, Group, MainLayout, WHITE, Z, MaxClip);
    }
    NewLine(MainLayout);
  END_BLOCK("Frame Ticker");

  u32 TotalThreadCount = GetWorkerThreadCount() + 1;

  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree    = MainThreadState->ScopeTrees + DebugState->ReadScopeIndex;

  local_persist window_layout CallgraphWindow = WindowLayout("Callgraph", V2(0, MainLayout->DrawBounds.Max.y));

  TIMED_BLOCK("Call Graph");

    Clear(&CallgraphWindow.Table.Layout.At);
    Clear(&CallgraphWindow.Table.Layout.DrawBounds);

    NewRow(&CallgraphWindow);
    Column("Frame %",  Group,  &CallgraphWindow,  WHITE);
    Column("Cycles",   Group,  &CallgraphWindow,  WHITE);
    Column("Calls",    Group,  &CallgraphWindow,  WHITE);
    Column("Name",     Group,  &CallgraphWindow,  WHITE);
    NewRow(&CallgraphWindow);

    for ( u32 ThreadIndex = 0;
        ThreadIndex < TotalThreadCount;
        ++ThreadIndex)
    {
      debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
      debug_scope_tree *ReadTree = ThreadState->ScopeTrees + DebugState->ReadScopeIndex;
      frame_stats *Frame = DebugState->Frames + DebugState->ReadScopeIndex;

      if (MainThreadReadTree->FrameRecorded == ReadTree->FrameRecorded)
      {
        BufferFirstCallToEach(Group, ReadTree->Root, ReadTree->Root, ThreadsafeDebugMemoryAllocator(), &CallgraphWindow, Frame->TotalCycles, 0);
        NewRow(&CallgraphWindow);
      }
    }

  END_BLOCK("Call Graph");

  rect2 Result = CallgraphWindow.Table.Layout.DrawBounds;
  return Result;
}

#if 0
function void
ColumnLeft(u32 Width, const char *Text, debug_ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  u32 Len = (u32)Length(Text);
  u32 Pad = Max(Width-Len, 0U);
  BufferValue(Text, Group, Layout, ColorIndex);
  AdvanceSpaces(Pad, Layout, &Group->Font);
}

function void
ColumnRight(s32 Width, const char *Text, debug_ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  s32 Len = (s32)Length(Text);
  s32 Pad = Max(Width-Len, 0);
  AdvanceSpaces(Pad, Layout, &Group->Font);
  BufferValue(Text, Group, Layout, ColorIndex);
}
#endif



/*************************                      ******************************/
/*************************  Collated Fun Calls  ******************************/
/*************************                      ******************************/



function void
DebugDrawCollatedFunctionCalls(debug_ui_render_group *Group, debug_state *DebugState, v2 BasisP)
{
  local_persist window_layout FunctionCallWindow = WindowLayout("Functions", BasisP);

  PushWindowInteraction(Group, &FunctionCallWindow);

  TIMED_BLOCK("Collated Function Calls");
  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree = MainThreadState->ScopeTrees + DebugState->ReadScopeIndex;

  CollateAllFunctionCalls(MainThreadReadTree->Root);

  for ( u32 FunctionIndex = 0;
      FunctionIndex < MAX_RECORDED_FUNCTION_CALLS;
      ++FunctionIndex)
  {
    called_function *Func = ProgramFunctionCalls + FunctionIndex;
    if (Func->Name)
    {
      Column(Func->Name, Group, &FunctionCallWindow);
      Column( ToString(Func->CallCount), Group, &FunctionCallWindow);
      NewRow(&FunctionCallWindow);
    }
  }
  END_BLOCK("Collated Function Calls");

}



/******************************              *********************************/
/******************************  Draw Calls  *********************************/
/******************************              *********************************/



debug_global const u32 Global_DrawCallArrayLength = 128;
debug_global debug_draw_call Global_DrawCalls[Global_DrawCallArrayLength] = {};
debug_global debug_draw_call NullDrawCall = {};

function void
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

function void
DebugDrawDrawCalls(debug_ui_render_group *Group, layout *WindowBasis)
{
  local_persist window_layout DrawCallWindow = WindowLayout("Draw Calls", GetAbsoluteAt(WindowBasis));
  PushWindowInteraction(Group, &DrawCallWindow);

  layout *Layout = &DrawCallWindow.Table.Layout;
  NewLine(Layout);
  NewLine(Layout);

  r32 Z = zIndexForText(&DrawCallWindow, Group);

  for( u32 DrawCountIndex = 0;
       DrawCountIndex < Global_DrawCallArrayLength;
       ++ DrawCountIndex)
  {
     debug_draw_call *DrawCall = &Global_DrawCalls[DrawCountIndex];
     if (DrawCall->Caller)
     {
       BufferThousands(DrawCall->Calls, Group, Layout, WHITE, Z, GetAbsoluteMaxClip(&DrawCallWindow));
       BufferThousands(DrawCall->N, Group, Layout, WHITE, Z, GetAbsoluteMaxClip(&DrawCallWindow));
       AdvanceSpaces(2, Layout, &Group->Font);
       BufferValue(DrawCall->Caller, Group, Layout, WHITE, Z, GetAbsoluteMaxClip(&DrawCallWindow));
       NewLine(Layout);
     }
  }

  return;
}



/*******************************            **********************************/
/*******************************  Arena UI  **********************************/
/*******************************            **********************************/



function b32
BufferBarGraph(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, table *Table, r32 PercFilled, v3 Color, r32 Z, v2 MaxClip)
{
  r32 BarHeight = Group->Font.Size.y;
  r32 BarWidth = 200.0f;

  v2 MinP = Table->Layout.At + Table->Layout.Basis;
  v2 BarDim = V2(BarWidth, BarHeight);
  v2 PercBarDim = V2(BarWidth, BarHeight) * V2(PercFilled, 1);

  BufferUntexturedQuad(Group, Geo, MinP, BarDim, V3(0.25f), Z, MaxClip);

  rect2 BarRect = { MinP, MinP + BarDim };
  b32 Hovering = IsInsideRect(BarRect, *Group->MouseP);

  if (Hovering)
    Color = {{ 1, 0, 1 }};

  BufferUntexturedQuad(Group, Geo, MinP, PercBarDim, Color, Z, MaxClip);

  Table->Layout.At.x += BarDim.x;
  AdvanceClip(&Table->Layout);

  return Hovering;
}

function b32
BufferArenaBargraph(table *Table, debug_ui_render_group *Group, umm TotalUsed, r32 TotalPerc, umm Remaining, v3 Color, r32 Z, v2 MaxClip)
{
  Column( FormatMemorySize(TotalUsed), Group, Table, Z, MaxClip);
  b32 Hover = BufferBarGraph(Group, &Group->TextGroup->UIGeo, Table, TotalPerc, Color, Z, MaxClip);
  Column( FormatMemorySize(Remaining), Group, Table, Z, MaxClip);
  NewRow(Table);

  b32 Click = (Hover && Group->Input->LMB.Clicked);
  return Click;
}

function void
BufferMemoryStatsTable(memory_arena_stats MemStats, debug_ui_render_group *Group, table *StatsTable, r32 Z, v2 MaxClip)
{
  Column("Allocs", Group, StatsTable, Z, MaxClip);
  Column(FormatMemorySize(MemStats.Allocations), Group, StatsTable, Z, MaxClip);
  NewRow(StatsTable);

  Column("Pushes", Group, StatsTable, Z, MaxClip);
  Column(FormatThousands(MemStats.Pushes), Group, StatsTable, Z, MaxClip);
  NewRow(StatsTable);

  Column("Remaining", Group, StatsTable, Z, MaxClip);
  Column(FormatMemorySize(MemStats.Remaining), Group, StatsTable, Z, MaxClip);
  NewRow(StatsTable);

  Column("Total", Group, StatsTable, Z, MaxClip);
  Column(FormatMemorySize(MemStats.TotalAllocated), Group, StatsTable, Z, MaxClip);
  NewRow(StatsTable);

  return;
}

function void
BufferMemoryBargraphTable(debug_ui_render_group *Group, selected_arenas *SelectedArenas, memory_arena_stats MemStats, umm TotalUsed, memory_arena *HeadArena, table *Table, r32 Z, v2 MaxClip)
{
  SetFontSize(&Group->Font, 22);

  NewRow(Table);
  v3 DefaultColor = V3(0.5f, 0.5f, 0.0);

  r32 TotalPerc = (r32)SafeDivide0(TotalUsed, MemStats.TotalAllocated);
  b32 ToggleAllArenas = BufferArenaBargraph(Table, Group, TotalUsed, TotalPerc, MemStats.Remaining, DefaultColor, Z, MaxClip);
  NewRow(Table);

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

    b32 GotClicked = BufferArenaBargraph(Table, Group, CurrentUsed, CurrentPerc, Remaining(CurrentArena), Color, Z, MaxClip);

    if (ToggleAllArenas || GotClicked)
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

function void
BufferDebugPushMetaData(debug_ui_render_group *Group, selected_arenas *SelectedArenas, umm CurrentArenaHead, table* Table, r32 Z, v2 MaxClip)
{
  push_metadata CollatedMetaTable[META_TABLE_SIZE] = {};

  SetFontSize(&Group->Font, 24);

  Column("Size", Group, Table, Z, MaxClip);
  Column("Structs", Group, Table, Z, MaxClip);
  Column("Push Count", Group, Table, Z, MaxClip);
  Column("Name", Group, Table, Z, MaxClip);
  NewRow(Table);


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
      Column( FormatMemorySize(AllocationSize), Group, Table, Z, MaxClip);
      Column( FormatThousands(Collated->StructCount), Group, Table, Z, MaxClip);
      Column( FormatThousands(Collated->PushCount), Group, Table, Z, MaxClip);
      Column(Collated->Name, Group, Table, Z, MaxClip);
      NewRow(Table);
    }

    continue;
  }

  return;
}

#if 0
function window_layout
SubWindowAt(window_layout* Original, v2 NewBasis)
{
  window_layout Result = {};

  Result.Table.Layout.Basis = NewBasis;
  Result.MaxClip = Original->Table.Layout.Basis + Original->MaxClip - NewBasis;

  return Result;
}
#endif

function void
DebugDrawMemoryHud(debug_ui_render_group *Group, debug_state *DebugState, v2 MemoryWindowBasis)
{
  local_persist window_layout MemoryArenaWindowInstance = WindowLayout("Memory Arenas", MemoryWindowBasis);
  window_layout* MemoryArenaWindow = &MemoryArenaWindowInstance;
  PushWindowInteraction(Group, MemoryArenaWindow);

  r32 Z = zIndexForText(MemoryArenaWindow, Group);

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
      NewLine(&MemoryArenaWindow->Table.Layout);

      interactable ExpandInteraction = StartInteractable(&MemoryArenaWindow->Table.Layout, (umm)"MemoryWindowExpandInteraction"^(umm)Current, MemoryArenaWindow);
        Column(Current->Name, Group, MemoryArenaWindow);
        Column(MemorySize(MemStats.TotalAllocated), Group, MemoryArenaWindow);
        Column(ToString(MemStats.Pushes), Group, MemoryArenaWindow);
        NewRow(MemoryArenaWindow);
      EndInteractable(MemoryArenaWindow, &ExpandInteraction);

      if (Clicked(Group, &ExpandInteraction))
      {
        Current->Expanded = !Current->Expanded;
      }
    }

    if (Current->Expanded)
    {
      SetFontSize(&Group->Font, 28);

      local_persist table MemoryStatsTable = {};
      MemoryStatsTable.Layout = LayoutBelow(&MemoryArenaWindow->Table);
      BufferMemoryStatsTable(MemStats, Group, &MemoryStatsTable, Z, GetAbsoluteMaxClip(MemoryArenaWindow));

      local_persist table MemoryBargraphTable = {};
      MemoryBargraphTable.Layout = LayoutBelow(&MemoryStatsTable);
      BufferMemoryBargraphTable(Group, DebugState->SelectedArenas, MemStats, TotalUsed, Current->Arena, &MemoryBargraphTable, Z, GetAbsoluteMaxClip(MemoryArenaWindow));

      table TmpTable = TableLayoutAt(&MemoryArenaWindow->Table);
      MergeTables(&MemoryStatsTable, &TmpTable);
      MergeTables(&MemoryBargraphTable, &TmpTable);

      {
        v3 BorderColor = V3(1, 1, 1);
        rect2 TableBounds = GetBounds(&TmpTable);
        Border(Group, &Group->TextGroup->UIGeo, TableBounds, BorderColor, 1.0f, DISABLE_CLIPPING);
      }

      local_persist table PushMetadataTable = {};
      PushMetadataTable.Layout = LayoutRightOf(&TmpTable);
      BufferDebugPushMetaData(Group, DebugState->SelectedArenas, HashArenaHead(Current->Arena), &PushMetadataTable, Z, GetAbsoluteMaxClip(MemoryArenaWindow));

      {
        v3 BorderColor = V3(1, 1, 1);
        rect2 TableBounds = GetBounds(&PushMetadataTable);
        Border(Group, &Group->TextGroup->UIGeo, TableBounds, BorderColor, 1.0f, DISABLE_CLIPPING);
      }

      MergeTables(&TmpTable, &MemoryArenaWindow->Table);
      MergeTables(&PushMetadataTable, &MemoryArenaWindow->Table);
    }

    continue;
  }


  return;
}



/*******************************              ********************************/
/*******************************  Network UI  ********************************/
/*******************************              ********************************/



function void
DebugDrawNetworkHud(debug_ui_render_group *Group,
    network_connection *Network,
    server_state *ServerState,
    layout *WindowBasis)
{
  local_persist window_layout NetworkWindow = WindowLayout("Network", WindowBasis->At);
  PushWindowInteraction(Group, &NetworkWindow);

  layout* Layout = &NetworkWindow.Table.Layout;
  r32 Z = zIndexForText(&NetworkWindow, Group);
  if (IsConnected(Network))
  {
    BufferValue("O", Group, Layout, GREEN, Z, GetAbsoluteMaxClip(&NetworkWindow));

    AdvanceSpaces(2, Layout, &Group->Font);

    if (Network->Client)
    {
      BufferValue("ClientId", Group, Layout, WHITE, Z, GetAbsoluteMaxClip(&NetworkWindow));
      BufferColumn( Network->Client->Id, 2, Group, Layout, WHITE, Z, GetAbsoluteMaxClip(&NetworkWindow));
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

      AdvanceSpaces(1, Layout, &Group->Font);
      BufferValue("Id:", Group, Layout, WHITE, Z, GetAbsoluteMaxClip(&NetworkWindow));
      BufferColumn( Client->Id, 2, Group, Layout, WHITE, Z, GetAbsoluteMaxClip(&NetworkWindow));
      AdvanceSpaces(2, Layout, &Group->Font);
      BufferColumn(Client->Counter, 7, Group, Layout, Color, Z, GetAbsoluteMaxClip(&NetworkWindow));
      NewLine(Layout);
    }

  }
  else
  {
    BufferValue("X", Group, Layout, RED, Z, GetAbsoluteMaxClip(&NetworkWindow));
    NewLine(Layout);
  }

  return;
}



/******************************               ********************************/
/******************************  Graphics UI  ********************************/
/******************************               ********************************/



function void
DebugDrawGraphicsHud(debug_ui_render_group *Group, debug_state *DebugState, layout *Layout, r32 Z, v2 MaxClip)
{
  NewLine(Layout);
  NewLine(Layout);

  BufferMemorySize(DebugState->BytesBufferedToCard, Group, Layout, WHITE, Z, MaxClip);

  return;
}



/******************************              *********************************/
/******************************  Initialize  *********************************/
/******************************              *********************************/



function b32
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

function void
AllocateAndInitGeoBuffer(textured_2d_geometry_buffer *Geo, u32 VertCount, memory_arena *DebugArena)
{
  Geo->Verts  = Allocate(v3, DebugArena, VertCount);
  Geo->Colors = Allocate(v3, DebugArena, VertCount);
  Geo->UVs    = Allocate(v3, DebugArena, VertCount);

  Geo->End = VertCount;
  Geo->At = 0;
}

function void
AllocateAndInitGeoBuffer(untextured_2d_geometry_buffer *Geo, u32 VertCount, memory_arena *DebugArena)
{
  Geo->Verts = Allocate(v3, DebugArena, VertCount);
  Geo->Colors = Allocate(v3, DebugArena, VertCount);

  Geo->End = VertCount;
  Geo->At = 0;
  return;
}

function shader
MakeSolidUIShader(memory_arena *Memory)
{
  shader SimpleTextureShader = LoadShaders( "SimpleColor.vertexshader",
                                            "SimpleColor.fragmentshader",
                                             Memory);
  return SimpleTextureShader;
}

function shader
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

function b32
InitDebugRenderSystem(debug_state *DebugState, heap_allocator *Heap)
{
  AllocateMesh(&DebugState->LineMesh, 1024, Heap);

  if (!InitDebugOverlayFramebuffer(&DebugState->TextRenderGroup, ThreadsafeDebugMemoryAllocator(), "texture_atlas_0.bmp"))
  { Error("Initializing Debug Overlay Framebuffer"); }

  AllocateAndInitGeoBuffer(&DebugState->TextRenderGroup.TextGeo, 1024, ThreadsafeDebugMemoryAllocator());
  AllocateAndInitGeoBuffer(&DebugState->TextRenderGroup.UIGeo, 1024, ThreadsafeDebugMemoryAllocator());

  AllocateGpuElementBuffer(&DebugState->GameGeo, (u32)Megabytes(4));

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

