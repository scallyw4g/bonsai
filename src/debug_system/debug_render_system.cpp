#include <texture.cpp>
#include <stream.cpp>
#include <shader.cpp>
#include <render_init.cpp>

#include <render.h>
#include <render_utils.cpp>
#include <bonsai_mesh.cpp>
#include <gpu_mapped_buffer.cpp>

#include <work_queue.cpp>

#define u32_COUNT_PER_QUAD 6

debug_global table_render_params NullTableRenderParams = {};

debug_global ui_style DefaultUiStyle = UiStyleFromLightestColor(V3(1), V3(0.2f));

/*******************************             *********************************/
/*******************************  Z helpers  *********************************/
/*******************************             *********************************/



function r32
zIndexForBackgrounds(window_layout *Window, debug_ui_render_group *Group)
{
  // NOTE(Jesse): This value returned by this function is the bottom-most value on this slice

  u64 StackIndex = Window? Window->InteractionStackIndex : Group->InteractionStackTop;

  // NOTE(Jesse): We add one to the z-slice initially because a z-slice of 0 is
  // invalid.  It is invalid because the slices are discrete chunks of z space
  // as opposed to indices into an array, and slice 0 is just nonsensical in
  // that context.
  u64 zSlice = 1 + Group->InteractionStackTop - StackIndex;

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
function void
NewLine(layout *Layout)
{
  Layout->At.y = Layout->DrawBounds.Max.y;
  Layout->At.x = 0;
  AdvanceClip(Layout);

  return;
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

  counted_string Result = CS(Buffer);
  return Result;
}

function counted_string
CS(u64 Number)
{
  counted_string Result = NumericValueToString(Number, "%lu");
  return Result;
}

function counted_string
CS(s32 Number)
{
  counted_string Result = NumericValueToString(Number, "%i");
  return Result;
}

function counted_string
CS(u32 Number)
{
  counted_string Result = NumericValueToString(Number, "%u");
  return Result;
}

function counted_string
CS(r64 Number)
{
  counted_string Result = NumericValueToString(Number, "%.2f");
  return Result;
}

function counted_string
CS(r32 Number)
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
FlushBuffer(debug_text_render_group *TextGroup, untextured_2d_geometry_buffer *Buffer, v2 ScreenDim)
{
  TIMED_FUNCTION();

  if (TextGroup)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    SetViewport(ScreenDim);
    UseShader(&TextGroup->SolidUIShader);

    u32 AttributeIndex = 0;
    BufferVertsToCard(TextGroup->SolidUIVertexBuffer, Buffer, &AttributeIndex);
    BufferColorsToCard(TextGroup->SolidUIColorBuffer, Buffer, &AttributeIndex);

    Draw(Buffer->At);
    Buffer->At = 0;

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    AssertNoGlErrors;
  }
  else
  {
    Warn("FlushBuffer call issued without a RenderGroup!");
  }

  return;
}

function void
FlushBuffer(debug_text_render_group *TextGroup, textured_2d_geometry_buffer *Geo, v2 ScreenDim)
{
  if (TextGroup)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    SetViewport(ScreenDim);
    glUseProgram(TextGroup->Text2DShader.ID);

    // Bind Font texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, TextGroup->FontTexture->ID);
    //

    glUniform1i(TextGroup->TextTextureUniform, 0); // Assign texture unit 0 to the TextTexureUniform

    u32 AttributeIndex = 0;
    BufferVertsToCard( TextGroup->SolidUIVertexBuffer, Geo, &AttributeIndex);
    BufferUVsToCard(   TextGroup->SolidUIUVBuffer,     Geo, &AttributeIndex);
    BufferColorsToCard(TextGroup->SolidUIColorBuffer,  Geo, &AttributeIndex);

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
  else
  {
    Warn("FlushBuffer call issued without a RenderGroup!");
  }

}

function void
FlushBuffers(debug_ui_render_group *UiGroup, v2 ScreenDim)
{
  if (UiGroup->TextGroup)
  {
    FlushBuffer(UiGroup->TextGroup, &UiGroup->Geo, ScreenDim);
    FlushBuffer(UiGroup->TextGroup, &UiGroup->TextGroup->Geo, ScreenDim);
  }

  return;
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

template <typename T> function void
BufferColorsDirect(T* Geo, v3 Color)
{
  v3* Colors = Geo->Colors;
  u32 StartingIndex = Geo->At;
  if (BufferHasRoomFor(Geo, u32_COUNT_PER_QUAD))
  {
    for ( u32 ColorIndex = StartingIndex;
          ColorIndex < StartingIndex + u32_COUNT_PER_QUAD + 1;
          ++ColorIndex)
    {
      Colors[ColorIndex] = Color;
    }
  }

  return;
}


template <typename T> function void
BufferColors(debug_ui_render_group *Group, T *Geo, v3 Color)
{
  if (!BufferHasRoomFor(Geo, u32_COUNT_PER_QUAD))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  BufferColorsDirect(Geo, Color);
}

template <typename T>function clip_result
BufferQuadDirect(T *Geo, v2 MinP, v2 Dim, r32 Z, v2 ScreenDim, v2 MaxClip)
{
  // Note(Jesse): Z==0 | far-clip
  // Note(Jesse): Z==1 | near-clip
  clip_result Result = {};

  if (BufferHasRoomFor(Geo, u32_COUNT_PER_QUAD))
  {
    v3 *Dest = Geo->Verts;
    u32 StartingIndex = Geo->At;

    Assert(Z >= 0.0f && Z <= 1.0f);

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
  }
  else
  {
    Error("Ran out of room on Geo in BufferQuadDirect");
  }

  return Result;
}

function clip_result
BufferTexturedQuad(debug_ui_render_group *Group,
                   v2 MinP, v2 Dim,
                   debug_texture_array_slice TextureSlice, rect2 UV,
                   v3 Color,
                   r32 Z, v2 MaxClip)
{
  clip_result Result = {};

  if (Group->TextGroup)
  {
    textured_2d_geometry_buffer* Geo = &Group->TextGroup->Geo;

    if (!BufferHasRoomFor(Geo, u32_COUNT_PER_QUAD))
      FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

    Result = BufferQuadDirect(Geo, MinP, Dim, Z, Group->ScreenDim, MaxClip);
    switch (Result.ClipStatus)
    {
      case ClipStatus_NoClipping:
      {
        BufferQuadUVs(Geo, UV, TextureSlice);
        BufferColors(Group, Geo, Color);
        Geo->At += u32_COUNT_PER_QUAD;
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
        Geo->At += u32_COUNT_PER_QUAD;
      } break;

      case ClipStatus_FullyClipped:
      {
      } break;

      InvalidDefaultCase;
    }

  }

  return Result;
}

function clip_result
BufferUntexturedQuad(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo,
                     v2 MinP, v2 Dim, v3 Color,
                     r32 Z, v2 MaxClip)
{
  if (!BufferHasRoomFor(Geo, u32_COUNT_PER_QUAD))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  clip_result Result = BufferQuadDirect(Geo, MinP, Dim, Z, Group->ScreenDim, MaxClip);
  switch (Result.ClipStatus)
  {
    case ClipStatus_NoClipping:
    case ClipStatus_PartialClipping:
    {
      BufferColors(Group, Geo, Color);
      Geo->At += u32_COUNT_PER_QUAD;
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
                         rect2 Rect, v3 Color, r32 Z, v2 MaxClip)
{
  v2 MinP = Rect.Min;
  v2 Dim = Rect.Max - Rect.Min;
  clip_result Result = BufferUntexturedQuad(Group, Geo, MinP, Dim, Color, Z, MaxClip);
  return Result;
}

function r32
BufferChar(debug_ui_render_group *Group, u8 Char, v2 MinP, font *Font, v3 Color, r32 Z, v2 MaxClip)
{
  rect2 UV = UVsForChar(Char);

  { // Black Drop-shadow
    r32 e = zOffsetForTextShadow();
    v2 ShadowOffset = 0.1f*Font->Size;
    BufferTexturedQuad( Group,
                        MinP+ShadowOffset, Font->Size,
                        DebugTextureArraySlice_Font, UV,
                        V3(0.1f),
                        Z-e,
                        MaxClip);

  }

  BufferTexturedQuad( Group,
                      MinP, Font->Size,
                      DebugTextureArraySlice_Font, UV,
                      Color,
                      Z, MaxClip);

  r32 DeltaX = Font->Size.x;
  return DeltaX;
}

function r32
BufferChar(debug_ui_render_group *Group, u8 Char, v2 MinP, font *Font, u32 Color, r32 Z, v2 MaxClip)
{
  v3 ColorVector = GetColorData(Color).xyz;
  r32 Result = BufferChar(Group, Char, MinP, Font, ColorVector, Z, MaxClip);
  return Result;
}

function buffer_value_params
BufferValueParams( window_layout* Window, layout* Layout, r32 Z, ui_style Style = DefaultUiStyle)
{
  buffer_value_params Result = {
    .Window = Window,
    .Layout = Layout,
    .Z = Z,
    .Style = Style,
  };

  return Result;
}

function void
BufferValue(counted_string Text, debug_ui_render_group *Group, buffer_value_params Params)
{
  window_layout* Window = Params.Window;
  layout* Layout        = Params.Layout;
  r32 Z                 = Params.Z;
  ui_style Style        = Params.Style;
  v2 MaxClip            = GetAbsoluteMaxClip(Params.Window);

  v2 Padding = Style.Padding;

  v2 FontHeight = V2(0, Group->Font.Size.y);
  v2 PadX = V2(Padding.x, 0);
  v2 PadY = V2(0, Padding.y);

  /* v2 StartingP = GetAbsoluteAt(Layout); */

  Layout->At += PadX;

  for ( u32 CharIndex = 0;
      CharIndex < Text.Count;
      CharIndex++ )
  {
    v2 MinP = GetAbsoluteAt(Window, Layout) + PadY;
    Layout->At.x += BufferChar(Group, (u8)Text.Start[CharIndex], MinP, &Group->Font, Style.Color, Z, MaxClip);
    continue;
  }

  Layout->At += PadX;

  v2 MaxClipP = Layout->At + (2.0f*PadY) + FontHeight;
  AdvanceClip(Layout, MaxClipP);

  /* v2 EndingP = Layout->Basis + MaxClipP; */
  /* BufferBorder(Group, &Group->TextGroup->Geo, RectMinMax(StartingP, EndingP), V3(0, 0, 1), Z, DISABLE_CLIPPING); */

  return;
}

function void
BufferValue(const char* Text, debug_ui_render_group *Group, buffer_value_params Params)
{
  BufferValue(CS(Text), Group, Params);
  return;
}

function void
BufferValue(r32 Number, debug_ui_render_group *Group, buffer_value_params Params)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%f", Number);
  BufferValue(Buffer, Group, Params);
  return;
}

function void
BufferValue(u64 Number, debug_ui_render_group *Group, buffer_value_params Params)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Number);
  BufferValue(Buffer, Group, Params);
  return;
}

function void
BufferValue(u32 Number, debug_ui_render_group *Group, buffer_value_params Params)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Number);
  BufferValue(Buffer, Group, Params);
  return;
}

function void
BufferThousands(u64 Number, debug_ui_render_group *Group, buffer_value_params Params, u32 Columns = 10)
{
  char  *Buffer = FormatThousands(Number);
  u32 Len = (u32)Length(Buffer);
  u32 Pad = Max(Columns-Len, 0U);
  AdvanceSpaces(Pad, Params.Layout, &Group->Font);
  BufferValue(Buffer, Group, Params);
  return;
}

function void
BufferMemorySize(u64 Number, debug_ui_render_group *Group, buffer_value_params Params)
{
  char *Buffer = FormatMemorySize(Number);
  BufferValue(Buffer, Group, Params);
  return;
}

function void
BufferColumn( s32 Value, u32 ColumnWidth, debug_ui_render_group *Group, buffer_value_params Params)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%d", Value);
  u32 Len = (u32)Length(Buffer);
  u32 Pad = Max(ColumnWidth-Len, 0U);
  AdvanceSpaces(Pad, Params.Layout, &Group->Font);
  BufferValue( Buffer, Group, Params);
  return;
}

function void
BufferColumn( u32 Value, u32 ColumnWidth, debug_ui_render_group *Group, buffer_value_params Params)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Value);
  {
    u32 Len = (u32)Length(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Params.Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Params);
  return;
}

function void
BufferColumn( u64 Value, u32 ColumnWidth, debug_ui_render_group *Group, buffer_value_params Params)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Value);
  {
    u32 Len = (u32)Length(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Params.Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Params);
  return;
}

function void
BufferColumn( r64 Perc, u32 ColumnWidth, debug_ui_render_group *Group, buffer_value_params Params)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%4.1lf", Perc);
  {
    u32 Len = (u32)Length(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Params.Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Params);
  return;
}

function void
BufferColumn( r32 Perc, u32 ColumnWidth, debug_ui_render_group *Group, buffer_value_params Params)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%.1f", Perc);
  {
    u32 Len = (u32)Length(Buffer);
    u32 Pad = Max(ColumnWidth-Len, 0U);
    AdvanceSpaces(Pad, Params.Layout, &Group->Font);
  }
  BufferValue( Buffer, Group, Params);
  return;
}

function void
BufferColumn(counted_string Text, debug_ui_render_group* Group, u32 ColumnWidth, buffer_value_params Params, column_render_params RenderParams)
{
  if (RenderParams & ColumnRenderParam_RightAlign)
  {
    u32 Pad = ColumnWidth - (u32)Text.Count;
    AdvanceSpaces(Pad, Params.Layout, &Group->Font);
  }

  BufferValue(Text, Group, Params);

  return;
}

function r32
BufferTextAt(debug_ui_render_group *Group, v2 BasisP, counted_string Text, v3 Color, r32 Z, v2 ClipMax = DISABLE_CLIPPING)
{
  r32 DeltaX = 0;

  for ( u32 CharIndex = 0;
      CharIndex < Text.Count;
      CharIndex++ )
  {
    v2 MinP = BasisP + V2(Group->Font.Size.x*CharIndex, 0);
    DeltaX += BufferChar(Group, (u8)Text.Start[CharIndex], MinP, &Group->Font, Color, Z, ClipMax);
    continue;
  }

  return DeltaX;
}

function r32
BufferTextAt(debug_ui_render_group *Group, v2 BasisP, const char *Text, v3 Color, r32 Z, v2 ClipMax = DISABLE_CLIPPING)
{
  r32 Result = BufferTextAt(Group, BasisP, CS(Text), Color, Z, ClipMax);
  return Result;
}

function void
BufferBorder(debug_ui_render_group *Group, rect2 Rect, v3 Color, r32 Z, v2 MaxClip)
{
  v2 TopLeft     = Rect.Min;
  v2 BottomRight = Rect.Max;
  v2 TopRight    = V2(Rect.Max.x, Rect.Min.y);
  v2 BottomLeft  = V2(Rect.Min.x, Rect.Max.y);

  rect2 TopRect    = RectMinMax(TopLeft ,    TopRight    + V2(0, 1));
  rect2 BottomRect = RectMinMax(BottomLeft,  BottomRight - V2(0, 1));
  rect2 LeftRect   = RectMinMax(TopLeft ,    BottomLeft  + V2(1, 0));
  rect2 RightRect  = RectMinMax(TopRight,    BottomRight + V2(1, 0));

  BufferUntexturedQuad(Group, &Group->Geo, TopRect,    Color, Z, MaxClip);
  BufferUntexturedQuad(Group, &Group->Geo, LeftRect,   Color, Z, MaxClip);
  BufferUntexturedQuad(Group, &Group->Geo, RightRect,  Color, Z, MaxClip);
  BufferUntexturedQuad(Group, &Group->Geo, BottomRect, Color, Z, MaxClip);

  return;
}

function void
BufferBorder(debug_ui_render_group *Group, interactable* PickerListInteraction, v3 Color, r32 Z, v2 MaxClip)
{
  rect2 Bounds = RectMinMax(PickerListInteraction->MinP, PickerListInteraction->MaxP);
  BufferBorder(Group, Bounds, Color, Z, MaxClip);
  return;
}



/*********************************           *********************************/
/*********************************  Buttons  *********************************/
/*********************************           *********************************/



function button_interaction_result
ButtonInteraction(debug_ui_render_group* Group, rect2 Bounds, umm InteractionId, window_layout *Window, ui_style *Style)
{
  button_interaction_result Result = {};

  Bounds.Max += (Style->Padding*2.0f);

  interactable Interaction = Interactable(Bounds, InteractionId, Window);
  /* BufferBorder(Group, &Interaction, V3(1,0,0), 1.0f, DISABLE_CLIPPING); */

  Style->Color = Style->AmbientColor;

  if (Hover(Group, &Interaction))
  {
    Result.Hover = True;
    Style->Color = Style->HoverColor;
    Style->BackgroundColor = Style->BackgroundHoverColor;
  }

  if (Clicked(Group, &Interaction))
  {
    Result.Clicked = True;
    Style->Color = Style->ClickedColor;
    Style->BackgroundColor = Style->BackgroundClickedColor;
  }

  if (Pressed(Group, &Interaction))
  {
    Result.Pressed = True;
    Style->Color = Style->ClickedColor;
    Style->BackgroundColor = Style->BackgroundPressedColor;
  }

  if (Style->IsActive && !Result.Pressed)
  {
    Style->Color = Style->ActiveColor;
    Style->BackgroundColor = Style->BackgroundActiveColor;
  }

  return Result;
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
  ui_render_command_buffer *CommandBuffer = Group->CommandBuffer;

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
PushColumn(debug_ui_render_group *Group, counted_string String, column_render_params Params = ColumnRenderParam_RightAlign)
{
  ui_render_command Command = {
    .Type = RenderCommand_Column,
    .Column.String = String,
    .Column.Params = Params
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

function void
PushTextAt(debug_ui_render_group *Group, counted_string Text, v2 At, v2 ClipMax)
{
  ui_render_command Command = {
    .Type           = RenderCommand_TextAt,
    .TextAt.Text    = Text,
    .TextAt.At      = At,
    .TextAt.MaxClip = ClipMax,
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

function void
PushTooltip(debug_ui_render_group *Group, counted_string Text)
{
  PushTextAt(Group, Text, *Group->MouseP+V2(12, -7), DISABLE_CLIPPING);
  return;
}

function void
PushTexturedQuad(debug_ui_render_group *Group, debug_texture_array_slice TextureSlice)
{
  ui_render_command Command = {
    .Type = RenderCommand_TexturedQuad,
    .TexturedQuad.TextureSlice = TextureSlice,
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

function void
PushUntexturedQuadAt(debug_ui_render_group* Group, v2 At, v2 QuadDim, r32 Z, ui_style *Style = 0, quad_render_params Params = QuadRenderParam_Default )
{
  ui_render_command Command = {
    .Type = RenderCommand_UntexturedQuadAt,
    .UntexturedQuadAt.AtRelativeToWindowBasis = At,
    .UntexturedQuadAt.QuadDim = QuadDim,
    .UntexturedQuadAt.Style = Style? *Style : DefaultUiStyle,
    .UntexturedQuadAt.Z = Z,
    .UntexturedQuadAt.Params = Params,
  };

  PushUiRenderCommand(Group, &Command);
}

function void
PushUntexturedQuad(debug_ui_render_group* Group, v2 OffsetFromLayout, v2 QuadDim, r32 Z, ui_style *Style = 0, quad_render_params Params = QuadRenderParam_Default )
{
  ui_render_command Command = {
    .Type = RenderCommand_UntexturedQuad,
    .UntexturedQuad.OffsetFromLayout = OffsetFromLayout,
    .UntexturedQuad.QuadDim = QuadDim,
    .UntexturedQuad.Style = Style? *Style : DefaultUiStyle,
    .UntexturedQuad.Params = Params,
    .UntexturedQuad.Z = Z,
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

function void
PushButtonEnd(debug_ui_render_group *Group, button_end_params Params = ButtonEndParam_NoOp)
{
  ui_render_command Command = {
    .Type = RenderCommand_ButtonEnd,
    .ButtonEnd.Params = Params,
  };
  PushUiRenderCommand(Group, &Command);
  return;
}

function interactable_handle
PushButtonStart(debug_ui_render_group *Group, umm InteractionId, ui_style* Style = 0)
{
  ui_render_command Command = {
    .Type = RenderCommand_ButtonStart,
    .ButtonStart.ID = InteractionId,
    .ButtonStart.Style = Style ? *Style : DefaultUiStyle,
  };

  PushUiRenderCommand(Group, &Command);

  interactable_handle Handle = {
    .Id = InteractionId
  };

  return Handle;
}

function void
PushTableStart(debug_ui_render_group* Group)
{
  ui_render_command Command = {
    .Type = RenderCommand_TableStart,
  };

  PushUiRenderCommand(Group, &Command);

  return;
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
PushBorder(debug_ui_render_group *Group, rect2 BoundsRelativeToCurrentWindow, v3 Color)
{
  ui_render_command Command = {
    .Type = RenderCommand_Border,
    .Border.Bounds = BoundsRelativeToCurrentWindow,
    .Border.Color = Color,
  };

  PushUiRenderCommand(Group, &Command);

}


function void
PushWindowStart(debug_ui_render_group *Group, window_layout *Window)
{
  ui_render_command Command = {};
  Command.Type = RenderCommand_WindowStart;
  Command.WindowStart.Window = Window;
  PushUiRenderCommand(Group, &Command);


  umm TitleBarInteractionId = (umm)"WindowTitleBar"^(umm)Window;
  interactable_handle TitleBarHandle = { .Id = TitleBarInteractionId };
  if (Pressed(Group, &TitleBarHandle))
  {
    Window->Basis -= *Group->MouseDP; // TODO(Jesse): Can we compute this with MouseP to avoid a frame of input delay?
  }

  umm DragHandleInteractionId = (umm)"WindowResizeWidget"^(umm)Window;
  interactable_handle DragHandle = { .Id = DragHandleInteractionId };
  if (Pressed(Group, &DragHandle))
  {
    v2 TitleBounds = GetTextBounds( (u32)Length(Window->Title), &Group->Font);
    Window->MaxClip = Max(TitleBounds, Window->MaxClip);

    v2 AbsoluteTitleBounds = Window->Basis + TitleBounds;
    v2 TestMaxClip = *Group->MouseP - Window->Basis;

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
  }

  PushButtonStart(Group, TitleBarInteractionId);
    PushTextAt(Group, CS(Window->Title), Window->Basis, GetAbsoluteMaxClip(Window));
    PushUntexturedQuad(Group, V2(0), V2(Window->MaxClip.x, Group->Font.Size.y), zIndexForTitleBar(Window, Group));
  PushButtonEnd(Group);

  PushNewRow(Group);

  v2 Dim = V2(10);
  PushButtonStart(Group, DragHandleInteractionId);
    PushUntexturedQuadAt(Group, Window->MaxClip-Dim, Dim, zIndexForBorders(Window, Group));
  PushButtonEnd(Group, ButtonEndParam_DiscardButtonDrawBounds);

  PushBorder(Group, GetBounds(Window), V3(1));

  ui_style BackgroundStyle = UiStyleFromLightestColor(V3(0.25f), V3(0.25f));
  PushUntexturedQuadAt(Group, V2(0), Window->MaxClip, zIndexForBackgrounds(Window, Group), &BackgroundStyle, QuadRenderParam_NoOp);

  return;
}

function void
PushWindowEnd(debug_ui_render_group *Group, window_layout *Window)
{

  {
    ui_render_command EndCommand = {};
    EndCommand.Type = RenderCommand_WindowEnd;
    EndCommand.WindowEnd.Window = Window;
    PushUiRenderCommand(Group, &EndCommand);
  }

  return;
}

function ui_render_command*
GetCommand(ui_render_command_buffer* CommandBuffer, u32 CommandIndex)
{
  ui_render_command* Command = 0;
  if (CommandIndex < CommandBuffer->CommandCount)
    { Command = CommandBuffer->Commands+CommandIndex; }
  return Command;
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
      case RenderCommand_WindowStart:
      {
        window_layout *TestWindow = Command->WindowStart.Window;
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

function u16
GetColumnCountForTable(ui_render_command_buffer* CommandBuffer, u32 CommandIndex)
{
  ui_render_command* Command =  GetCommand(CommandBuffer, CommandIndex++);
  Assert(Command && Command->Type == RenderCommand_TableStart);

  u16 ColumnIndex = 0;
  u16 ColumnCount = 0;

  b32 FoundEnd = False;
  while (Command && !FoundEnd)
  {
    switch(Command->Type)
    {
        case RenderCommand_Column:
        {
          ++ColumnIndex;
          Assert(ColumnIndex <= u16_MAX);
        } break;

        case RenderCommand_NewRow:
        {
          ColumnCount = (u16)Max(ColumnCount, ColumnIndex);
          ColumnIndex = 0;
        } break;

        case RenderCommand_TableEnd:
        {
          FoundEnd = True;
        } break;

        default: {} break;
    }

    Command = GetCommand(CommandBuffer, CommandIndex++);
  }

  return ColumnCount;
}

function table_render_params
GetTableRenderParams(ui_render_command_buffer* CommandBuffer, u32 CommandIndex)
{
  table_render_params Result = {};
  Result.ColumnCount = GetColumnCountForTable(CommandBuffer, CommandIndex);
  Result.ColumnWidths = AllocateProtection(u32, TranArena, Result.ColumnCount, False);

  ui_render_command* Command =  GetCommand(CommandBuffer, CommandIndex++);
  Assert(Command && Command->Type == RenderCommand_TableStart);

  u32 ColumnIndex = 0;
  while (Command && !Result.OnePastTableEnd)
  {
    switch(Command->Type)
    {
        case RenderCommand_Column:
        {
          Assert(ColumnIndex < Result.ColumnCount);
          Result.ColumnWidths[ColumnIndex] = Max((u32)Command->Column.String.Count, Result.ColumnWidths[ColumnIndex]);
          ++ColumnIndex;
        } break;

        case RenderCommand_NewRow:
        {
          ColumnIndex = 0;
        } break;
        case RenderCommand_TableEnd:
        {
          Result.OnePastTableEnd = CommandIndex;
        } break;

        default: {} break;
    }

    Command = GetCommand(CommandBuffer, CommandIndex++);
  }

  return Result;

};

function u32
GetNextColumnWidth(table_render_params* Params)
{
  Assert(Params->CurrentColumn < Params->ColumnCount);
  u32 Result = Params->ColumnWidths[Params->CurrentColumn++];
  return Result;
}

function void
ProcessButtonStart(debug_ui_render_group* Group, render_state* RenderState, umm ButtonId)
{
  Assert(!RenderState->CurrentInteraction.ID);

  RenderState->CurrentInteraction.MinP = {};
  RenderState->CurrentInteraction.MaxP = {};
  RenderState->CurrentInteraction.ID = ButtonId;
  RenderState->CurrentInteraction.Window = RenderState->Window;

  RenderState->ButtonStartingDrawBounds = RenderState->Layout.DrawBounds;
  RenderState->Layout.DrawBounds = InvertedInfinityRectangle();

  if (RenderState->CurrentInteraction.ID == Group->HoverInteractionId)
  {
    Group->HoverInteractionId = 0;
    RenderState->Hover = True;
  }
  if (RenderState->CurrentInteraction.ID == Group->PressedInteractionId)
  {
    Group->PressedInteractionId = 0;
    RenderState->Pressed = True;
  }
  if (RenderState->CurrentInteraction.ID == Group->ClickedInteractionId)
  {
    Group->ClickedInteractionId = 0;
    RenderState->Clicked = True;
  }

  return;
}

function void
ProcessButtonEnd(debug_ui_render_group *Group, render_state* RenderState, ui_render_command_button_end* Command)
{
  Assert(RenderState->CurrentInteraction.ID);

  RenderState->CurrentInteraction.MinP = GetAbsoluteDrawBoundsMin(RenderState->Window, &RenderState->Layout);
  RenderState->CurrentInteraction.MaxP = GetAbsoluteDrawBoundsMax(RenderState->Window, &RenderState->Layout);

  button_interaction_result Button = ButtonInteraction( Group,
                                                        RectMinMax(RenderState->CurrentInteraction.MinP, RenderState->CurrentInteraction.MaxP ),
                                                        RenderState->CurrentInteraction.ID,
                                                        RenderState->Window,
                                                        &RenderState->Style);

  if (Button.Hover)
  {
    Group->HoverInteractionId = RenderState->CurrentInteraction.ID;
  }

  if (Button.Clicked)
  {
    Group->ClickedInteractionId = RenderState->CurrentInteraction.ID;
  }

  if (Button.Pressed)
  {
    Group->PressedInteractionId = RenderState->CurrentInteraction.ID;
  }

  RenderState->Hover = False;
  RenderState->Pressed = False;
  RenderState->Clicked = False;

  RenderState->CurrentInteraction.ID = 0;

  if (Command->Params & ButtonEndParam_DiscardButtonDrawBounds)
  {
    RenderState->Layout.DrawBounds = RenderState->ButtonStartingDrawBounds;
  }
  else
  {
    RenderState->Layout.DrawBounds.Min = Min(RenderState->ButtonStartingDrawBounds.Min, RenderState->Layout.DrawBounds.Min);
    RenderState->Layout.DrawBounds.Max = Max(RenderState->ButtonStartingDrawBounds.Max, RenderState->Layout.DrawBounds.Max);
  }

  return;
}

function void
ProcessTexturedQuadPush(debug_ui_render_group* Group, ui_render_command_textured_quad *Command, render_state* RenderState)
{
  v2 MinP = GetAbsoluteAt(RenderState->Window, &RenderState->Layout);
  r32 Z = zIndexForText(RenderState->Window, Group);
  v2 MaxClip = GetAbsoluteMaxClip(RenderState->Window);
  BufferTexturedQuad( Group, MinP, RenderState->Window->MaxClip,
                      Command->TextureSlice, UVsForFullyCoveredQuad(),
                      V3(1), Z, MaxClip);
}

function v3
SelectColorState(render_state* RenderState, ui_style Style)
{
  v3 Result = Style.Color;

  if (RenderState->Hover)
  {
    Result = Style.HoverColor;
  }
  if (RenderState->Pressed)
  {
    Result = Style.PressedColor;
  }
  if (RenderState->Clicked)
  {
    Result = Style.ClickedColor;
  }

  return Result;
}

function void
ProcessUntexturedQuadAtPush(debug_ui_render_group* Group, ui_render_command_untextured_quad_at *Command, render_state* RenderState)
{
  v2 MaxClip = GetAbsoluteMaxClip(RenderState->Window);
  v2 MinP = Command->AtRelativeToWindowBasis + (RenderState->Window ? RenderState->Window->Basis : V2(0));
  v2 Dim = Command->QuadDim;
  v3 Color = SelectColorState(RenderState, Command->Style);
  r32 Z = Command->Z;

  BufferUntexturedQuad(Group, &(Group->Geo), MinP, Dim, Color, Z, MaxClip);

  if (Command->Params & QuadRenderParam_AdvanceClip)
  {
    AdvanceClip(&RenderState->Layout, Command->AtRelativeToWindowBasis);
    AdvanceClip(&RenderState->Layout, Command->AtRelativeToWindowBasis + Dim);
  }

  return;
}

function void
ProcessUntexturedQuadPush(debug_ui_render_group* Group, ui_render_command_untextured_quad *Command, render_state* RenderState)
{
  v2 MaxClip = GetAbsoluteMaxClip(RenderState->Window);
  v2 MinP = Command->OffsetFromLayout + GetAbsoluteAt(RenderState->Window, &RenderState->Layout);
  v2 Dim = Command->QuadDim;
  v3 Color = SelectColorState(RenderState, Command->Style);
  r32 Z = Command->Z;

  BufferUntexturedQuad(Group, &(Group->Geo), MinP, Dim, Color, Z, MaxClip);

  if (Command->Params & QuadRenderParam_AdvanceClip)
  {
    AdvanceClip(&RenderState->Layout, RenderState->Layout.At + Command->OffsetFromLayout);
    AdvanceClip(&RenderState->Layout, RenderState->Layout.At + Command->OffsetFromLayout + Dim);
  }

  if (Command->Params & QuadRenderParam_AdvanceLayout)
  {
    RenderState->Layout.At.x += Dim.x;
    AdvanceClip(&RenderState->Layout);
  }

  return;
}

function void
FlushCommandBuffer(debug_ui_render_group *Group, ui_render_command_buffer *CommandBuffer)
{
  u32 CommandIndex = 0;
  ui_render_command *Command = GetCommand(CommandBuffer, CommandIndex++);

  render_state RenderState = { .Style = DefaultUiStyle };

  table_render_params TableRenderParams = {};

  while (Command)
  {
    switch(Command->Type)
    {
      case RenderCommand_WindowStart:
      {
        Assert(!RenderState.Window);
        RenderState.Window = Command->WindowStart.Window;
        Clear(&RenderState.Layout.At);
        Clear(&RenderState.Layout.DrawBounds);
      } break;

      case RenderCommand_WindowEnd:
      {
        Assert(RenderState.Window == Command->WindowEnd.Window);
        RenderState.Window = 0;
      } break;

      case RenderCommand_TableStart:
      {
        Assert( AreEqual(TableRenderParams, NullTableRenderParams));

        TableRenderParams = GetTableRenderParams(CommandBuffer, CommandIndex-1);
        if (!TableRenderParams.OnePastTableEnd)
        {
          Error("No RenderCommand_TableEnd detected.");
          CommandIndex = CommandBuffer->CommandCount;
        }
      } break;

      case RenderCommand_TableEnd:
      {
        Assert(CommandIndex == TableRenderParams.OnePastTableEnd);
        TableRenderParams = NullTableRenderParams;
      } break;

      case RenderCommand_Column:
      {
        u32 ColumnWidth = GetNextColumnWidth(&TableRenderParams);
        RenderState.Style.Color = SelectColorState(&RenderState, RenderState.Style);
        buffer_value_params Params = BufferValueParams(RenderState.Window, &RenderState.Layout, zIndexForText(RenderState.Window, Group), RenderState.Style);
        BufferColumn(Command->Column.String, Group, ColumnWidth, Params, Command->Column.Params);
      } break;

      case RenderCommand_TexturedQuad:
      {
        ProcessTexturedQuadPush(Group, &Command->TexturedQuad, &RenderState);
      } break;

      case RenderCommand_UntexturedQuadAt:
      {
        ProcessUntexturedQuadAtPush(Group, &Command->UntexturedQuadAt, &RenderState);
      } break;

      case RenderCommand_UntexturedQuad:
      {
        ProcessUntexturedQuadPush(Group, &Command->UntexturedQuad, &RenderState);
      } break;

      case RenderCommand_NewRow:
      {
        TableRenderParams.CurrentColumn = 0;
        NewLine(&RenderState.Layout);
      } break;

      case RenderCommand_TextAt:
      {
        ui_render_command_text_at TextCommand = Command->TextAt;
        BufferTextAt(Group, TextCommand.At, TextCommand.Text, V3(1), 1.0f, TextCommand.MaxClip);
      } break;

      case RenderCommand_ButtonStart:
      {
        RenderState.Style = Command->ButtonStart.Style;
        ProcessButtonStart(Group, &RenderState, Command->ButtonStart.ID);
      } break;

      case RenderCommand_ButtonEnd:
      {
        RenderState.Style = DefaultUiStyle;
        ProcessButtonEnd(Group, &RenderState, &Command->ButtonEnd);
      } break;

      case RenderCommand_Border:
      {
        BufferBorder(Group, Command->Border.Bounds, Command->Border.Color, zIndexForBorders(RenderState.Window, Group), DISABLE_CLIPPING);
      } break;

      InvalidDefaultCase;
    }

    Command = GetCommand(CommandBuffer, CommandIndex++);
  }

  CommandBuffer->CommandCount = 0;
  return;
}



/****************************                       **************************/
/****************************  Mutex Introspection  **************************/
/****************************                       **************************/



function void
PushCycleBar(debug_ui_render_group* Group, window_layout* Window, cycle_range* Range, cycle_range* Frame, r32 TotalGraphWidth, u32 Depth, ui_style *Style)
{
  Assert(Frame->StartCycle < Range->StartCycle);

  r32 FramePerc = (r32)Range->TotalCycles / (r32)Frame->TotalCycles;

  r32 BarHeight = Group->Font.Size.y;
  r32 BarWidth = FramePerc*TotalGraphWidth;
  v2 BarDim = V2(BarWidth, BarHeight);

  u64 StartCycleOffset = Range->StartCycle - Frame->StartCycle;
  r32 xOffset = GetXOffsetForHorizontalBar(StartCycleOffset, Frame->TotalCycles, TotalGraphWidth);
  r32 yOffset = Depth*Group->Font.Size.y;

  v2 OffsetFromLayout = V2(xOffset, yOffset);

  PushUntexturedQuad(Group, OffsetFromLayout, BarDim, zIndexForText(Window, Group), Style, QuadRenderParam_NoOp);

  return;
}

#if 0
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

  untextured_2d_geometry_buffer *Geo = &Group->Geo;
  cycle_range FrameRange = {FrameStartingCycle, FrameTotalCycles};

  cycle_range WaitRange = {WaitRecord->Cycle, WaitCycleCount};
  DrawCycleBar( &WaitRange, &FrameRange, TotalGraphWidth, 0, V3(1, 0, 0), Group, Geo, Layout, Z, MaxClip, 0);

  cycle_range AquiredRange = {AquiredRecord->Cycle, AquiredCycleCount};
  DrawCycleBar( &AquiredRange, &FrameRange, TotalGraphWidth, 0, V3(0, 1, 0), Group, Geo, Layout, Z, MaxClip, 0);

  return;
}
#endif



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
PushChunkView(debug_ui_render_group* Group, world_chunk* Chunk, window_layout* Window)
{
  debug_state* DebugState = GetDebugState();
  PushWindowStart(Group, Window);
    PushTableStart(Group);
      b32 DebugButtonPressed = False;

      interactable_handle PrevButton = PushButtonStart(Group, (umm)"PrevButton");
        PushColumn(Group, CS("<"));
      PushButtonEnd(Group);

      if (Clicked(Group, &PrevButton))
      {
        Chunk->PointsToLeaveRemaining = Min(Chunk->PointsToLeaveRemaining+1, Chunk->EdgeBoundaryVoxelCount);
        DebugButtonPressed = True;
      }


      interactable_handle NextButton = PushButtonStart(Group, (umm)"NextButton");
        PushColumn(Group, CS(">"));
      PushButtonEnd(Group);

      if (Clicked(Group, &NextButton))
      {
        Chunk->PointsToLeaveRemaining = Max(Chunk->PointsToLeaveRemaining-1, 0);
        DebugButtonPressed = True;
      }

      counted_string ButtonText = Chunk->DrawBoundingVoxels ? CS("|") : CS("O");

      interactable_handle ToggleBoundingVoxelsButton = PushButtonStart(Group, (umm)"ToggleBoundingVoxelsButton");
        PushColumn(Group, ButtonText);
      PushButtonEnd(Group);

      if (Clicked(Group, &ToggleBoundingVoxelsButton))
      {
        Chunk->DrawBoundingVoxels = !Chunk->DrawBoundingVoxels;
        DebugButtonPressed = True;
      }

      if (DebugButtonPressed)
      {
        Chunk->LodMesh_Complete = False;
        Chunk->LodMesh->At = 0;
        Chunk->Mesh = 0;
        Chunk->FilledCount = 0;
        Chunk->Data->Flags = Chunk_Uninitialized;
        QueueChunkForInit( DebugState->GameState, &DebugState->Plat->HighPriority, Chunk);
      }

      PushNewRow(Group);
    PushTableEnd(Group);

    PushTableStart(Group);
      interactable_handle ViewportButton = PushButtonStart(Group, (umm)"ViewportButton");
        PushTexturedQuad(Group, DebugTextureArraySlice_Viewport);
      PushButtonEnd(Group);
    PushTableEnd(Group);

  PushWindowEnd(Group, Window);

  input* WindowInput = 0;
  if (Pressed(Group, &ViewportButton))
    { WindowInput = Group->Input; }
  UpdateGameCamera( -0.005f*(*Group->MouseDP), WindowInput, Canonical_Position(0), &DebugState->Camera);
}

function void
PushChunkDetails(debug_ui_render_group* Group, world_chunk* Chunk, window_layout* Window)
{
  PushWindowStart(Group, Window);
  PushTableStart(Group);
    PushColumn(Group, CS("WorldP"));
    PushColumn(Group, CS(Chunk->WorldP.x));
    PushColumn(Group, CS(Chunk->WorldP.y));
    PushColumn(Group, CS(Chunk->WorldP.z));
    PushNewRow(Group);

    PushColumn(Group, CS("PointsToLeaveRemaining"));
    PushColumn(Group, CS(Chunk->PointsToLeaveRemaining));
    PushNewRow(Group);

    PushColumn(Group, CS("BoundaryVoxels Count"));
    PushColumn(Group, CS(Chunk->EdgeBoundaryVoxelCount));
    PushNewRow(Group);

    PushColumn(Group, CS("Triangles"));
    PushColumn(Group, CS(Chunk->TriCount));
    PushNewRow(Group);
  PushTableEnd(Group);
  PushWindowEnd(Group, Window);
}

function v2
BasisRightOf(window_layout* Window, v2 WindowSpacing = V2(50, 0))
{
  v2 Result = V2(GetAbsoluteMaxClip(Window).x, Window->Basis.y) + WindowSpacing;
  return Result;
}

function void
DrawPickedChunks(debug_ui_render_group* Group, v2 LayoutBasis)
{
  debug_state* DebugState = GetDebugState();
  world_chunk** PickedChunks = DebugState->PickedChunks;
  MapGpuElementBuffer(&DebugState->GameGeo);

  local_persist window_layout ListingWindow = WindowLayout("Picked Chunks", LayoutBasis, V2(400, 400));

  PushWindowStart(Group, &ListingWindow);
  PushTableStart(Group);

  for (u32 ChunkIndex = 0;
      ChunkIndex < DebugState->PickedChunkCount;
      ++ChunkIndex)
  {
    world_chunk *Chunk = PickedChunks[ChunkIndex];

    interactable_handle PositionButton = PushButtonStart(Group, (umm)"PositionButton"^(umm)Chunk);
      PushColumn(Group, CS(Chunk->WorldP.x) );
      PushColumn(Group, CS(Chunk->WorldP.y) );
      PushColumn(Group, CS(Chunk->WorldP.z) );
    PushButtonEnd(Group);

    if (Clicked(Group, &PositionButton)) { DebugState->HotChunk = Chunk; }

    interactable_handle CloseButton = PushButtonStart(Group, (umm)"CloseButton"^(umm)Chunk);
      PushColumn(Group, CS("X"));
    PushButtonEnd(Group);

    if ( Clicked(Group, &CloseButton) )
    {
      world_chunk** SwapChunk = PickedChunks+ChunkIndex;
      if (*SwapChunk == DebugState->HotChunk) { DebugState->HotChunk = 0; }
      *SwapChunk = PickedChunks[--DebugState->PickedChunkCount];
    }

    PushNewRow(Group);
  }

  PushTableEnd(Group);
  PushWindowEnd(Group, &ListingWindow);

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
    local_persist window_layout ChunkDetailWindow = WindowLayout("Chunk Details", BasisRightOf(&ListingWindow),     V2(1100.0f, 400.0f));
    local_persist window_layout ChunkViewWindow   = WindowLayout("Chunk View",    BasisRightOf(&ChunkDetailWindow), V2(800.0f));

    PushChunkDetails(Group, HotChunk, &ChunkDetailWindow);
    PushChunkView(Group, HotChunk, &ChunkViewWindow);
  }

  return;
}



/************************                        *****************************/
/************************  Thread Perf Bargraph  *****************************/
/************************                        *****************************/



function counted_string
BuildNameStringFor(char Prefix, const char* Name, u32 DepthAdvance)
{
  u32 NameLength = (u32)Length(Name);
  u32 FinalBufferLength = DepthAdvance + 1 + NameLength;

  char* Buffer = FormatString(TranArena, "%*s%c%s", DepthAdvance, "", Prefix, Name);
  counted_string Result = CS(Buffer, FinalBufferLength);
  return Result;
}

function void
BufferScopeTreeEntry(debug_ui_render_group *Group, debug_profile_scope *Scope,
                     u64 TotalCycles, u64 TotalFrameCycles, u64 CallCount, u32 Depth)
{
  Assert(TotalFrameCycles);

  r32 Percentage = 100.0f * (r32)SafeDivide0((r64)TotalCycles, (r64)TotalFrameCycles);
  u64 AvgCycles = (u64)SafeDivide0(TotalCycles, CallCount);

  PushColumn(Group, CS(Percentage));
  PushColumn(Group, CS(AvgCycles));
  PushColumn(Group, CS(CallCount));

  u32 DepthSpaces = (Depth*2)+1;

  char Prefix = ' ';
  if (Scope->Expanded && Scope->Child)
  {
    Prefix = '-';
  }
  else if (Scope->Child)
  {
    Prefix = '+';
  }

  counted_string NameString = BuildNameStringFor(Prefix, Scope->Name, DepthSpaces);
  PushColumn(Group, NameString, ColumnRenderParam_LeftAlign);
  PushNewRow(Group);

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
PushScopeBarsRecursive(debug_ui_render_group *Group, window_layout* Window, debug_profile_scope *Scope, cycle_range *Frame, r32 TotalGraphWidth, random_series *Entropy, u32 Depth = 0)
{
  while (Scope)
  {
    cycle_range Range = {Scope->StartingCycle, Scope->CycleCount};

    ui_style Style = UiStyleFromLightestColor(RandomV3(Entropy), V3(0.3f));

    interactable_handle Bar = PushButtonStart(Group, (umm)"CycleBarHoverInteraction"^(umm)Scope, &Style);
      PushCycleBar(Group, Window, &Range, Frame, TotalGraphWidth, Depth, &Style);
    PushButtonEnd(Group);

    if (Hover(Group, &Bar)) { PushTooltip(Group, CS(Scope->Name)); }
    if (Clicked(Group, &Bar)) { Scope->Expanded = !Scope->Expanded; }

    if (Scope->Expanded) { PushScopeBarsRecursive(Group, Window, Scope->Child, Frame, TotalGraphWidth, Entropy, Depth+1); }
    Scope = Scope->Sibling;
  }

  return;
}

function void
DebugDrawCycleThreadGraph(debug_ui_render_group *Group, debug_state *SharedState, v2 BasisP)
{
  random_series Entropy = {};
  r32 TotalGraphWidth = 1500.0f;
  local_persist window_layout CycleGraphWindow = WindowLayout("Cycle Graph", BasisP);

  PushWindowStart(Group, &CycleGraphWindow);

  SetFontSize(&Group->Font, 30);

  u32 TotalThreadCount                = GetTotalThreadCount();
  frame_stats *FrameStats             = SharedState->Frames + SharedState->ReadScopeIndex;
  cycle_range FrameCycles             = {FrameStats->StartingCycle, FrameStats->TotalCycles};

  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree    = MainThreadState->ScopeTrees + SharedState->ReadScopeIndex;

  PushTableStart(Group);
    for ( u32 ThreadIndex = 0;
          ThreadIndex < TotalThreadCount;
          ++ThreadIndex)
    {
      counted_string ThreadName = CS(FormatString(TranArena, "Thread %u", ThreadIndex));
      PushColumn(Group, ThreadName);
      PushNewRow(Group);

      debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
      debug_scope_tree *ReadTree = ThreadState->ScopeTrees + SharedState->ReadScopeIndex;
      if (MainThreadReadTree->FrameRecorded == ReadTree->FrameRecorded)
      {
        PushScopeBarsRecursive(Group, &CycleGraphWindow, ReadTree->Root, &FrameCycles, TotalGraphWidth, &Entropy);
        PushNewRow(Group);
      }

    }
  PushTableEnd(Group);

#if 0
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
      PushUntexturedQuad(Group, MinP16ms, Dim, V3(0,1,0));
    }
    {
      r32 FramePerc = 33.333333f/TotalMs;
      r32 xOffset = FramePerc*TotalGraphWidth;
      v2 MinP16ms = Layout->Basis + V2( xOffset, MinY );
      v2 MaxP16ms = Layout->Basis + V2( xOffset+MarkerWidth, Layout->At.y );
      v2 Dim = MaxP16ms - MinP16ms;
      PushUntexturedQuad(Group, MinP16ms, Dim, V3(0,1,1));
    }
  }
#endif

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

  PushWindowEnd(Group, &CycleGraphWindow);

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
    memory_arena *Memory, window_layout* Window, u64 TotalFrameCycles, u32 Depth)
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
    interactable_handle ScopeTextInteraction = PushButtonStart(Group, (umm)UniqueScopes->Scope);
      BufferScopeTreeEntry(Group, UniqueScopes->Scope, UniqueScopes->TotalCycles, TotalFrameCycles, UniqueScopes->CallCount, Depth);
    PushButtonEnd(Group);

    if (UniqueScopes->Scope->Expanded)
      BufferFirstCallToEach(Group, UniqueScopes->Scope->Child, TreeRoot, Memory, Window, TotalFrameCycles, Depth+1);

    if (Clicked(Group, &ScopeTextInteraction))
    {
      UniqueScopes->Scope->Expanded = !UniqueScopes->Scope->Expanded;
    }

    UniqueScopes = UniqueScopes->NextUnique;
  }

  PushNewRow(Group);
  return;
}

function void
DrawFrameTicker(debug_ui_render_group *Group, debug_state *DebugState, r64 MaxMs, v2 Basis)
{
  TIMED_FUNCTION();

  for (u32 FrameIndex = 0;
      FrameIndex < DEBUG_FRAMES_TRACKED;
      ++FrameIndex )
  {
    frame_stats *Frame = DebugState->Frames + FrameIndex;
    r32 Perc = (r32)SafeDivide0(Frame->FrameMs, MaxMs);

    /* v3 Color = V3(0.5f, 0.5f, 0.5f); */

    /* debug_scope_tree *Tree = GetThreadLocalStateFor(0)->ScopeTrees + FrameIndex; */
    /* if ( Tree == DebugState->GetWriteScopeTree() ) */
    /* { */
    /*   Color = V3(0.8f, 0.0f, 0.0f); */
    /*   Perc = 0.05f; */
    /* } */

    /* if ( Tree == DebugState->GetReadScopeTree(0) ) */
    /*   Color = V3(0.8f, 0.8f, 0.0f); */

    v2 MaxDim = V2(15.0f, 80.0f);
    v2 QuadDim = MaxDim * V2(1.0f, Perc);
    v2 VerticalOffset = MaxDim - QuadDim;

    /* v2 MinP = MainLayout->At; */
    /* v2 MaxP = MinP + MaxDim; */
    /* interactable Interaction = Interactable(MinP, MaxP, (umm)"CallGraphBarInteract", 0); */
    /* if (Hover(Group, &Interaction)) */
    /* { */
    /*   debug_thread_state *MainThreadState = GetThreadLocalStateFor(0); */
    /*   if (FrameIndex != MainThreadState->WriteIndex % DEBUG_FRAMES_TRACKED) */
    /*   { */
    /*     DebugState->ReadScopeIndex = FrameIndex; */
    /*     Color = V3(0.8f, 0.8f, 0.0f); */
    /*   } */
    /* } */

    ui_style Style = UiStyleFromLightestColor(V3(1,1,0), V3(0.3f), V2(5,0));
    PushUntexturedQuad(Group, Basis + VerticalOffset, QuadDim, 0.0f, &Style);
  }


#if 0
  r32 MaxBarHeight = Group->Font.Size.y;
  v2 QuadDim = V2(MainLayout->At.x, 2.0f);
  {
    r32 MsPerc = (r32)SafeDivide0(33.333, MaxMs);
    r32 MinPOffset = MaxBarHeight * MsPerc;
    v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size.y - MinPOffset };

    PushUntexturedQuadAt(Group, MinP, QuadDim, V3(1,1,0), Z, MaxClip);
  }

  {
    r32 MsPerc = (r32)SafeDivide0(16.666, MaxMs);
    r32 MinPOffset = MaxBarHeight * MsPerc;
    v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size.y - MinPOffset };

    PushUntexturedQuadAt(Group, &Group->TextGroup->UIGeo, MinP, QuadDim, V3(0,1,0), Z, MaxClip);
  }
#endif

  /* SetFontSize(&Group->Font, 30); */

  frame_stats *Frame = DebugState->Frames + DebugState->ReadScopeIndex;

  u32 TotalMutexOps = GetTotalMutexOpsForReadFrame();
  PushTableStart(Group);
    PushColumn(Group, CS(Frame->FrameMs));
    PushColumn(Group, CS(Frame->TotalCycles));
    PushColumn(Group, CS(TotalMutexOps));
    PushNewRow(Group);
  PushTableEnd(Group);

  return;
}

function void
DebugDrawCallGraph(debug_ui_render_group *Group, debug_state *DebugState, layout *MainLayout, r64 MaxMs)
{
  DrawFrameTicker(Group, DebugState, Max(33.3, MaxMs), MainLayout->At);

  NewLine(MainLayout);

  u32 TotalThreadCount = GetWorkerThreadCount() + 1;

  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree    = MainThreadState->ScopeTrees + DebugState->ReadScopeIndex;

  local_persist window_layout CallgraphWindow = WindowLayout("Callgraph", V2(0, MainLayout->At.y) + V2(50));

  TIMED_BLOCK("Call Graph");

    PushWindowStart(Group, &CallgraphWindow);
    PushTableStart(Group);

    PushColumn(Group, CS("Frame %"));
    PushColumn(Group, CS("Cycles"));;
    PushColumn(Group, CS("Calls"));
    PushColumn(Group, CS("Name"));
    PushNewRow(Group);

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
      }
    }
    PushTableEnd(Group);
    PushWindowEnd(Group, &CallgraphWindow);

  END_BLOCK("Call Graph");

  DebugDrawCycleThreadGraph(Group, DebugState, BasisRightOf(&CallgraphWindow));

  return;
}



/*************************                      ******************************/
/*************************  Collated Fun Calls  ******************************/
/*************************                      ******************************/



function void
DebugDrawCollatedFunctionCalls(debug_ui_render_group *Group, debug_state *DebugState, v2 BasisP)
{
  local_persist window_layout FunctionCallWindow = WindowLayout("Functions", BasisP);

  TIMED_BLOCK("Collated Function Calls");
  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree = MainThreadState->ScopeTrees + DebugState->ReadScopeIndex;

  CollateAllFunctionCalls(MainThreadReadTree->Root);

  PushWindowStart(Group, &FunctionCallWindow);

  PushTableStart(Group);
  for ( u32 FunctionIndex = 0;
      FunctionIndex < MAX_RECORDED_FUNCTION_CALLS;
      ++FunctionIndex)
  {
    called_function *Func = ProgramFunctionCalls + FunctionIndex;
    if (Func->Name)
    {
      PushColumn(Group, CS(Func->Name));
      PushColumn(Group, CS(Func->CallCount));
      PushNewRow(Group);
    }
  }
  PushTableEnd(Group);
  PushWindowEnd(Group, &FunctionCallWindow);

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
DebugDrawDrawCalls(debug_ui_render_group *Group, v2 WindowBasis)
{
  local_persist window_layout DrawCallWindow = WindowLayout("Draw Calls", WindowBasis);
  PushWindowStart(Group, &DrawCallWindow);

  PushTableStart(Group);

     PushColumn(Group, CS("Caller"));
     PushColumn(Group, CS("Calls"));
     PushColumn(Group, CS("Bytes"));
     PushNewRow(Group);

  for( u32 DrawCountIndex = 0;
       DrawCountIndex < Global_DrawCallArrayLength;
       ++ DrawCountIndex)
  {
     debug_draw_call *DrawCall = &Global_DrawCalls[DrawCountIndex];
     if (DrawCall->Caller)
     {
       PushColumn(Group, CS(DrawCall->Caller));
       PushColumn(Group, CS(DrawCall->Calls));
       PushColumn(Group, CS(DrawCall->N));
       PushNewRow(Group);
     }
  }

  PushTableEnd(Group);

  PushWindowEnd(Group, &DrawCallWindow);
  return;
}



/*******************************            **********************************/
/*******************************   Memory   **********************************/
/*******************************            **********************************/


function void
PushBargraph(debug_ui_render_group *Group, r32 PercFilled, r32 Z)
{
  r32 BarHeight = Group->Font.Size.y;
  r32 BarWidth = 200.0f;

  v2 BackgroundQuad = V2(BarWidth, BarHeight);
  v2 PercBarDim = BackgroundQuad * V2(PercFilled, 1);

  ui_style Style = UiStyleFromLightestColor(V3(0.6f), V3(0.3f));
  PushUntexturedQuad(Group, V2(0), BackgroundQuad, Z, &Style);

  Style = UiStyleFromLightestColor(V3(1,1,0), V3(0.3f));
  PushUntexturedQuad(Group, V2(-BackgroundQuad.x, 0), PercBarDim, Z, &Style, QuadRenderParam_NoOp);

  return;
}

function interactable_handle
PushArenaBargraph(debug_ui_render_group *Group, umm TotalUsed, r32 TotalPerc, umm Remaining, umm InteractionId, r32 Z)
{
  PushColumn(Group, CS(FormatMemorySize(TotalUsed)));

  interactable_handle Handle = PushButtonStart(Group, InteractionId);
    PushBargraph(Group, TotalPerc, Z);
  PushButtonEnd(Group);

  PushColumn(Group, CS(FormatMemorySize(Remaining)));
  PushNewRow(Group);
  return Handle;
}

function void
PushMemoryStatsTable(memory_arena_stats MemStats, debug_ui_render_group *Group)
{
  PushColumn(Group, CS("Allocs"));
  PushColumn(Group, CS(FormatMemorySize(MemStats.Allocations)));
  PushNewRow(Group);

  PushColumn(Group, CS("Pushes"));
  PushColumn(Group, CS(FormatThousands(MemStats.Pushes)));
  PushNewRow(Group);

  PushColumn(Group, CS("Remaining"));
  PushColumn(Group, CS(FormatMemorySize(MemStats.Remaining)));
  PushNewRow(Group);

  PushColumn(Group, CS("Total"));
  PushColumn(Group, CS(FormatMemorySize(MemStats.TotalAllocated)));
  PushNewRow(Group);

  return;
}

function void
PushMemoryBargraphTable(debug_ui_render_group *Group, selected_arenas *SelectedArenas, memory_arena_stats MemStats, umm TotalUsed, memory_arena *HeadArena, r32 Z)
{
  SetFontSize(&Group->Font, 22);

  PushNewRow(Group);
  v3 DefaultColor = V3(0.5f, 0.5f, 0.0);

  r32 TotalPerc = (r32)SafeDivide0(TotalUsed, MemStats.TotalAllocated);
  // TODO(Jesse): Should we do something special when interacting with this thing instead of Ignored-ing it?
  PushArenaBargraph(Group, TotalUsed, TotalPerc, MemStats.Remaining, (umm)"Ignored", Z);
  PushNewRow(Group);

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

    interactable_handle Handle = PushArenaBargraph(Group, CurrentUsed, CurrentPerc, Remaining(CurrentArena), HashArena(CurrentArena), Z);
    if (Clicked(Group, &Handle))
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
PushDebugPushMetaData(debug_ui_render_group *Group, selected_arenas *SelectedArenas, umm CurrentArenaHead)
{
  push_metadata CollatedMetaTable[META_TABLE_SIZE] = {};

  SetFontSize(&Group->Font, 24);

  PushColumn(Group, CS("Size"));
  PushColumn(Group, CS("Structs"));
  PushColumn(Group, CS("Push Count"));
  PushColumn(Group, CS("Name"));
  PushNewRow(Group);


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
      PushColumn(Group,  CS(FormatMemorySize(AllocationSize)));
      PushColumn(Group,  CS(FormatThousands(Collated->StructCount)));
      PushColumn(Group,  CS(FormatThousands(Collated->PushCount)));
      PushColumn(Group, CS(Collated->Name));
      PushNewRow(Group);
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

  PushWindowStart(Group, MemoryArenaWindow);

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

      PushTableStart(Group);
        interactable_handle ExpandInteraction = PushButtonStart(Group, (umm)"MemoryWindowExpandInteraction"^(umm)Current);
          PushColumn(Group, CS(Current->Name));
          PushColumn(Group, CS(MemorySize(MemStats.TotalAllocated)));
          PushColumn(Group, CS(ToString(MemStats.Pushes)));
          PushNewRow(Group);
        PushButtonEnd(Group);
      PushTableEnd(Group);

      if (Clicked(Group, &ExpandInteraction))
      {
        Current->Expanded = !Current->Expanded;
      }
    }

    if (Current->Expanded)
    {
      SetFontSize(&Group->Font, 28);

      PushTableStart(Group);
        PushMemoryStatsTable(MemStats, Group);
      PushTableEnd(Group);

      PushTableStart(Group);
        PushMemoryBargraphTable(Group, DebugState->SelectedArenas, MemStats, TotalUsed, Current->Arena, zIndexForText(MemoryArenaWindow, Group));
      PushTableEnd(Group);

      PushTableStart(Group);
        PushDebugPushMetaData(Group, DebugState->SelectedArenas, HashArenaHead(Current->Arena));
      PushTableEnd(Group);
    }

    continue;
  }

  PushWindowEnd(Group, MemoryArenaWindow);

  return;
}



/*******************************              ********************************/
/*******************************  Network UI  ********************************/
/*******************************              ********************************/



function void
DebugDrawNetworkHud(debug_ui_render_group *Group,
    network_connection *Network,
    server_state *ServerState,
    v2 Basis)
{
  local_persist window_layout NetworkWindow = WindowLayout("Network", Basis);

  PushWindowStart(Group, &NetworkWindow);

  PushTableStart(Group);
  if (IsConnected(Network))
  {
    PushColumn(Group, CS("O"));

    if (Network->Client)
    {
      PushColumn(Group, CS("ClientId"));
      PushColumn(Group, CS(Network->Client->Id));
      PushNewRow(Group);
    }

    for (s32 ClientIndex = 0;
        ClientIndex < MAX_CLIENTS;
        ++ClientIndex)
    {
      client_state *Client = &ServerState->Clients[ClientIndex];

      u32 Color = WHITE;

      if (Network->Client->Id == ClientIndex)
        Color = GREEN;

      PushColumn(Group, CS("Id:"));
      PushColumn(Group, CS( Client->Id));
      PushColumn(Group, CS(Client->Counter));
      PushNewRow(Group);
    }

  }
  else
  {
    PushColumn(Group, CS("X"));
    PushNewRow(Group);
  }
  PushTableEnd(Group);
  PushWindowEnd(Group, &NetworkWindow);

  return;
}



/******************************               ********************************/
/******************************  Graphics UI  ********************************/
/******************************               ********************************/



function void
DebugDrawGraphicsHud(debug_ui_render_group *Group, debug_state *DebugState, layout *Layout, r32 Z)
{
  NewLine(Layout);
  NewLine(Layout);

  buffer_value_params Params = BufferValueParams(0, Layout, Z);
  BufferMemorySize(DebugState->BytesBufferedToCard, Group, Params);

  return;
}



/******************************              *********************************/
/******************************  Initialize  *********************************/
/******************************              *********************************/



function void
InitDebugOverlayFramebuffer(debug_text_render_group *TextGroup, memory_arena *DebugArena, const char *DebugFont)
{
  TextGroup->FontTexture = LoadBitmap(DebugFont, DebugArena, DebugTextureArraySlice_Count);

  glGenBuffers(1, &TextGroup->SolidUIVertexBuffer);
  glGenBuffers(1, &TextGroup->SolidUIColorBuffer);
  glGenBuffers(1, &TextGroup->SolidUIUVBuffer);

  TextGroup->Text2DShader = LoadShaders("TextVertexShader.vertexshader",
                                        "TextVertexShader.fragmentshader", DebugArena);

  TextGroup->TextTextureUniform = glGetUniformLocation(TextGroup->Text2DShader.ID, "TextTextureSampler");

  return;
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

  DebugState->UiGroup.TextGroup = Allocate(debug_text_render_group, ThreadsafeDebugMemoryAllocator(), 1);
  DebugState->UiGroup.CommandBuffer = Allocate(ui_render_command_buffer, ThreadsafeDebugMemoryAllocator(), 1);

  InitDebugOverlayFramebuffer(DebugState->UiGroup.TextGroup, ThreadsafeDebugMemoryAllocator(), "texture_atlas_0.bmp");
  AllocateAndInitGeoBuffer(&DebugState->UiGroup.TextGroup->Geo, 1024, ThreadsafeDebugMemoryAllocator());
  AllocateAndInitGeoBuffer(&DebugState->UiGroup.Geo, 1024, ThreadsafeDebugMemoryAllocator());

  AllocateGpuElementBuffer(&DebugState->GameGeo, (u32)Megabytes(4));

  DebugState->UiGroup.TextGroup->SolidUIShader = MakeSolidUIShader(ThreadsafeDebugMemoryAllocator());

  DebugState->SelectedArenas = Allocate(selected_arenas, ThreadsafeDebugMemoryAllocator(), 1);

  DebugState->GameGeoFBO = GenFramebuffer();
  glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID);

  FramebufferTextureLayer(&DebugState->GameGeoFBO, DebugState->UiGroup.TextGroup->FontTexture, DebugTextureArraySlice_Viewport);
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

