#include <texture.cpp>
#include <stream.cpp>
#include <shader.cpp>
#include <render_init.cpp>

#include <render.h>
#include <render_utils.cpp>
#include <bonsai_mesh.cpp>
#include <gpu_mapped_buffer.cpp>

#include <work_queue.cpp>

debug_global rect2 NullClipRect = {};

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

function void
BufferQuadUVs(textured_2d_geometry_buffer* Geo, rect2 UV, debug_texture_array_slice Slice)
{
  v3 up_left    = V3(UV.Min.x, UV.Min.y, (r32)Slice);
  v3 up_right   = V3(UV.Max.x, UV.Min.y, (r32)Slice);
  v3 down_right = V3(UV.Max.x, UV.Max.y, (r32)Slice);
  v3 down_left  = V3(UV.Min.x, UV.Max.y, (r32)Slice);

  u32 StartingIndex = Geo->At;
  Geo->UVs[StartingIndex++] = up_left;
  Geo->UVs[StartingIndex++] = down_left;
  Geo->UVs[StartingIndex++] = up_right;

  Geo->UVs[StartingIndex++] = down_right;
  Geo->UVs[StartingIndex++] = up_right;
  Geo->UVs[StartingIndex++] = down_left;

  return;
}

function rect2
UVsForFullyCoveredQuad()
{
  // Note(Jesse): These are weird compared to what you might expect because
  // OpenGL screen coordinates originate at the bottom left, but are inverted
  // in our app such that the origin is at the top-left
  // @inverted_screen_y_coordinate
  v2 up_left    = V2(0.0f, 1.0f);
  v2 down_right = V2(1.0f, 0.0f);

  rect2 Result = RectMinMax(up_left, down_right);
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
  v2 down_left = GetUVForCharCode(C);
  v2 up_left  = down_left + V2(0.0f, OneOverSixteen);
  v2 down_right  = down_left + V2(OneOverSixteen, 0.0f);

  rect2 Result = RectMinMax(up_left, down_right);
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

#define TO_NDC(P) ((P * ToNDC) - 1.0f)

enum clip_status
{
  ClipStatus_NoClipping,
  ClipStatus_PartialClipping,
  ClipStatus_FullyClipped
};

struct clip_result
{
  clip_status ClipStatus;
  v2 MaxClip;

  rect2 PartialClip;
};


// Note(Jesse): Z==0 | far-clip
// Note(Jesse): Z==1 | near-clip
function clip_result
BufferQuadDirect(v3 *Dest, u32 StartingIndex, v2 MinP, v2 Dim, r32 Z, v2 ScreenDim, v2 MaxClip)
{
  Assert(Z >= 0.0f && Z <= 1.0f);

  v3 up_left    = V3( MinP.x       , MinP.y      , Z);
  v3 up_right   = V3( MinP.x+Dim.x , MinP.y      , Z);
  v3 down_right = V3( MinP.x+Dim.x , MinP.y+Dim.y, Z);
  v3 down_left  = V3( MinP.x       , MinP.y+Dim.y, Z);

  clip_result Result = {};
  if (LengthSq(MaxClip) > 0.0f)
  {
    // Partial clipping cases
    {
      if (up_left.x < MaxClip.x && up_right.x > MaxClip.x)
      {
        // NOTE(Jesse): These clipping cases require a MinClip value, which
        // we don't have because there's no way of resizing a window from the
        // min corner!
        /* r32 total = up_right.x - up_left.x; */
        /* r32 total_clipped =  up_left.x - MaxClip.x; */
        /* Result.PartialClip.Min.x = total_clipped / total; */

        Result.MaxClip.x = up_right.x = MaxClip.x;
        Result.ClipStatus = ClipStatus_PartialClipping;
      }

      if (down_left.x < MaxClip.x && down_right.x > MaxClip.x)
      {
        r32 total = down_right.x - down_left.x;
        r32 total_clipped = down_right.x - MaxClip.x;
        Result.PartialClip.Max.x = total_clipped / total;

        Result.MaxClip.x = down_right.x = MaxClip.x;
        Result.ClipStatus = ClipStatus_PartialClipping;
      }

      if (up_right.y < MaxClip.y && down_right.y > MaxClip.y)
      {
        r32 total = down_right.y - up_right.y;
        r32 total_clipped = down_right.y - MaxClip.y;
        Result.PartialClip.Max.y = total_clipped / total;

        Result.MaxClip.y = down_right.y = MaxClip.y;
        Result.ClipStatus = ClipStatus_PartialClipping;
      }

      if (up_left.y < MaxClip.y && down_left.y > MaxClip.y)
      {
        // NOTE(Jesse): These clipping cases require a MinClip value, which
        // we don't have because there's no way of resizing a window from the
        // min corner!
        /* r32 total = down_left.y - up_left.y; */
        /* r32 total_clipped = down_left.y - MaxClip.y; */
        /* Result.PartialClip.Min.y = total_clipped / total; */

        Result.MaxClip.y = down_left.y = MaxClip.y;
        Result.ClipStatus = ClipStatus_PartialClipping;
      }
    }

    // Fully Clipped
    {
      if (up_left.x >= MaxClip.x ||
          down_left.x >= MaxClip.x ||
          up_left.y >= MaxClip.y ||
          up_right.y >= MaxClip.y )
      {
        Result.ClipStatus = ClipStatus_FullyClipped;
        return Result;
      }
    }

  }

  v3 ToNDC = 2.0f/V3(ScreenDim.x, ScreenDim.y, 1.0f);

  // Native OpenGL screen coordinates are {0,0} at the bottom-left corner. This
  // maps the origin to the top-left of the screen.
  // @inverted_screen_y_coordinate
  v3 InvertYZ = V3(1.0f, -1.0f, -1.0f);

  Dest[StartingIndex++] = InvertYZ * TO_NDC(up_left);
  Dest[StartingIndex++] = InvertYZ * TO_NDC(down_left);
  Dest[StartingIndex++] = InvertYZ * TO_NDC(up_right);

  Dest[StartingIndex++] = InvertYZ * TO_NDC(down_right);
  Dest[StartingIndex++] = InvertYZ * TO_NDC(up_right);
  Dest[StartingIndex++] = InvertYZ * TO_NDC(down_left);

  Result.MaxClip = down_right.xy;

  return Result;
}

function clip_result
BufferTexturedQuad(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo,
                   v2 MinP, v2 Dim,
                   debug_texture_array_slice TextureSlice, rect2 UV,
                   v3 Color,
                   r32 Z = 0.5f, v2 MaxClip = V2(0))
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
      Assert(Result.PartialClip.Max.x >= 0.0f && Result.PartialClip.Max.x <= 0.5f);
      Assert(Result.PartialClip.Max.y >= 0.0f && Result.PartialClip.Max.y <= 0.5f);
      Assert(Result.PartialClip.Min.x >= 0.0f && Result.PartialClip.Min.x <= 0.5f);
      Assert(Result.PartialClip.Min.y >= 0.0f && Result.PartialClip.Min.y <= 0.5f);

      v2 ImaginaryMinClippingPercentage = Result.PartialClip.Min;
      v2 ImaginaryMaxClippingPercentage = Result.PartialClip.Max;

      v2 MinUvDiagonal = UV.Max - UV.Min;
      v2 MaxUvDiagonal = UV.Min - UV.Max;

      v2 MinUvModifier = MinUvDiagonal * ImaginaryMinClippingPercentage;
      v2 MaxUvModifier = MaxUvDiagonal * ImaginaryMaxClippingPercentage;

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
                     r32 Z = 0.5f, v2 MaxClip = V2(0))
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
BufferChar(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo, u32 CharIndex, v2 MinP, font *Font, const char *Text, v3 Color, v2 MaxClip = V2(0))
{
  u8 Char = (u8)Text[CharIndex];
  rect2 UV = UVsForChar(Char);

  { // Black Drop-shadow
    v2 ShadowOffset = 0.1f*V2(Font->Size);
    BufferTexturedQuad( Group, Geo,
                        MinP+ShadowOffset, V2(Font->Size),
                        DebugTextureArraySlice_Font, UV,
                        V3(0),
                        0.9999f, MaxClip);

  }

  clip_result ClipResult = BufferTexturedQuad( Group, Geo,
                                         MinP, V2(Font->Size),
                                         DebugTextureArraySlice_Font, UV,
                                         Color,
                                         1.0f, MaxClip);

  r32 DeltaX = 0;
  if (ClipResult.ClipStatus != ClipStatus_FullyClipped)
  {
    DeltaX = (ClipResult.MaxClip.x - MinP.x);
  }

  return DeltaX;
}

function r32
BufferChar(debug_ui_render_group *Group, textured_2d_geometry_buffer *Geo, u32 CharIndex, v2 MinP, font *Font, const char *Text, u32 Color, v2 MaxClip = V2(0))
{
  v3 ColorVector = GetColorData(Color).xyz;
  r32 Result = BufferChar(Group, Geo, CharIndex, MinP, Font, Text, ColorVector, MaxClip);
  return Result;
}

function r32
BufferValue(const char* Text, debug_ui_render_group *Group, layout *Layout,
            v3 Color, v2 MaxClip = V2(0), ui_style* Style = 0)
{
  textured_2d_geometry_buffer *Geo = &Group->TextGroup->TextGeo;

  u32 QuadCount = (u32)strlen(Text);

  r32 DeltaX = 0;

  v2 Padding = Style? Style->Padding : V2(0.0f);

  if (LengthSq(MaxClip) > 0.0f) MaxClip+=Layout->Basis;

  for ( u32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = Layout->Basis + Layout->At + Padding + V2(Group->Font.Size*CharIndex, 0);
    DeltaX += BufferChar(Group, Geo, CharIndex, MinP, &Group->Font, Text, Color, MaxClip);
    continue;
  }

  Layout->At.x += (DeltaX + (Padding.x*2.0f));

  AdvanceClip(Layout, &Group->Font, Style);
  return DeltaX;
}

function r32
BufferValue(const char* Text, debug_ui_render_group *Group, layout *Layout, u32 Color, v2 MaxClip = V2(0), ui_style* Style = 0)
{
  v3 ColorVector = GetColorData(Color).xyz;
  r32 Result = BufferValue(Text, Group, Layout, ColorVector, MaxClip, Style);
  return Result;
}

function void
BufferValue(r32 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%f", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex);
  return;
}

function void
BufferValue(u64 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%lu", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex);
  return;
}

function void
BufferValue(u32 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char Buffer[32] = {};
  sprintf(Buffer, "%u", Number);
  BufferValue(Buffer, Group, Layout, ColorIndex);
  return;
}

void
EndClipRect(debug_ui_render_group *Group, window_layout *Window, untextured_2d_geometry_buffer *Geo, v3 Color = V3(0.2f))
{
  layout *Layout = &Window->Table.Layout;

  v2 MinP = Layout->Clip.Min + Layout->Basis;
  v2 Dim = Min(Layout->Clip.Max, Window->MaxClip) - Layout->Clip.Min;

  BufferUntexturedQuad(Group, Geo, MinP, Dim, Color, 0.0f);
  return;
}


/*****************************                ********************************/
/*****************************  Text Helpers  ********************************/
/*****************************                ********************************/

function void
AdvanceSpaces(u32 N, layout *Layout, font *Font)
{
  Layout->At.x += (N*Font->Size);
  AdvanceClip(Layout);
  return;
}

function rect2
NewLine(layout *Layout)
{
  v2 Min = { Layout->Basis.x, Layout->Basis.y + Layout->At.y };
  Layout->At.y = Layout->Clip.Max.y;
  v2 Max = Layout->Basis + Layout->At;

  rect2 Bounds = RectMinMax(Min, Max);
  Layout->At.x = 0;
  AdvanceClip(Layout);
  return Bounds;
}

r32
BufferLine(const char* Text, u32 Color, layout *Layout, debug_ui_render_group *Group)
{
  r32 xOffset = BufferValue(Text, Group, Layout, Color);
  NewLine(Layout);
  return xOffset;
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

function void
BufferMemorySize(u64 Number, debug_ui_render_group *Group, layout *Layout, u32 ColorIndex)
{
  char *Buffer = FormatMemorySize(Number);
  BufferValue( Buffer, Group, Layout, ColorIndex);
  return;
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

function void
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

function void
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

function void
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

function void
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

function void
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

function void
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
Column(const char* ColumnText, debug_ui_render_group* Group, table* Table, u8 Color = WHITE, u8 HoverColor = TEAL, v2 MaxClip = V2(0))
{
  layout *Layout = &Table->Layout;

  table_column *Column = Table->Columns + Table->ColumnIndex;
  Table->ColumnIndex = (Table->ColumnIndex+1)%MAX_TABLE_COLUMNS;

  u32 TextLength = (u32)strlen(ColumnText);
  Column->Max = Max(Column->Max, TextLength + 1);

  u32 Pad = Column->Max - TextLength;
  AdvanceSpaces(Pad, Layout, &Group->Font);

  v2 Min = Layout->Basis + Layout->At;
  v2 Max = Min + GetTextBounds(TextLength, &Group->Font);

  u8 UseColor = Color;
  rect2 Bounds = RectMinMax(Min, Max);
  if (IsInsideRect(Bounds, Group->MouseP))
  {
    UseColor = HoverColor;
  }

  BufferValue(ColumnText, Group, Layout, UseColor, MaxClip);

  return Bounds;
}

function rect2
Column(const char* ColumnText, debug_ui_render_group* Group, window_layout* Window, u8 Color = WHITE, u8 HoverColor = TEAL)
{
  rect2 Result = Column(ColumnText, Group, &Window->Table, Color, HoverColor, Window->MaxClip);
  return Result;
}

function b32
Button(const char* ColumnText, debug_ui_render_group *Group, layout* Layout, ui_style* Style = 0)
{
  b32 Result = False;
  u32 TextLength = (u32)strlen(ColumnText);

  v2 Min = Layout->Basis + Layout->At;
  v2 Max = Min + GetTextBounds(TextLength, &Group->Font);

  v3 UseColor = V3(1.0f);
  rect2 Bounds = RectMinMax(Min, Max);

  if (Style)
  {
    Bounds.Max += (Style->Padding*2.0f);
    UseColor = Style->Color;
  }

  if (IsInsideRect(Bounds, Group->MouseP))
  {
    if (Style)
    {
      UseColor = Style->HoverColor;
    }

    if (Group->Input->LMB.IsDown)
    {
      Result = True;
      if (Style)
      {
        UseColor = Style->ClickColor;
      }
    }
  }

  if (Style && Style->IsActive && !Result)
  {
    UseColor = Style->ActiveColor;
  }

  BufferValue(ColumnText, Group, Layout, UseColor, V2(0), Style);

  return Result;
}

function b32
Button(const char* ColumnText, debug_ui_render_group *Group, window_layout* Window, u8 Color)
{
  b32 Result = False;
  rect2 Bounds = Column(ColumnText, Group, Window, Color);
  if (IsInsideRect(Bounds, Group->MouseP) && Group->Input->LMB.WasPressed)
  {
    Result = True;
  }

  return Result;
}

function void
BufferScopeTreeEntry(debug_ui_render_group *Group, debug_profile_scope *Scope, window_layout* Window,
    u8 Color, u64 TotalCycles, u64 TotalFrameCycles, u64 CallCount, u32 Depth)
{
  Assert(TotalFrameCycles);

  r32 Percentage = 100.0f * (r32)SafeDivide0((r64)TotalCycles, (r64)TotalFrameCycles);
  u64 AvgCycles = (u64)SafeDivide0(TotalCycles, CallCount);

  Column(ToString(Percentage), Group, Window, Color);
  Column(ToString(AvgCycles),  Group, Window, Color);
  Column(ToString(CallCount),  Group, Window, Color);

  layout *Layout = &Window->Table.Layout;
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
  NewRow(Window);

  return;
}

function rect2
GetNextLineBounds(layout *Layout, font *Font)
{
  v2 StartingP = GetAbsoluteAt(Layout);

  // FIXME(Jesse): Should line length be systemized somehow?
  v2 EndingP = StartingP + V2(100000.0f, Font->LineHeight);
  rect2 Result = { StartingP, EndingP };
  return Result;
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

function void
PadBottom(layout *Layout, r32 Pad)
{
  Layout->At.y += Pad;
}

function void
SetFontSize(font *Font, r32 FontSize)
{
  Font->Size = FontSize;
  Font->LineHeight = FontSize * 1.3f;
  return;
}

function r32
BufferTextAt(debug_ui_render_group *Group, v2 BasisP, const char *Text, u32 Color)
{
  textured_2d_geometry_buffer *Geo = &Group->TextGroup->TextGeo;

  u32 QuadCount = (u32)strlen(Text);

  r32 DeltaX = 0;

  for ( u32 CharIndex = 0;
      CharIndex < QuadCount;
      CharIndex++ )
  {
    v2 MinP = BasisP + V2(Group->Font.Size*CharIndex, 0);
    DeltaX += BufferChar(Group, Geo, CharIndex, MinP, &Group->Font, Text, Color);
    continue;
  }

  return DeltaX;
}

function void
DoTooltip(debug_ui_render_group *Group, const char *Text)
{
  BufferTextAt(Group, Group->MouseP+V2(12, -7), Text, WHITE);
  return;
}


/****************************                       **************************/
/****************************  Mutex Introspection  **************************/
/****************************                       **************************/


function b32
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

  interactable Interaction = Interactable(MinP, MinP+BarDim, (umm)"CycleBarHoverInteraction" );
  b32 Result = Hover(Group, &Interaction);
  if (Result)
  {
    Color *= 0.5f;
    if (Tooltip) { DoTooltip(Group, Tooltip); }
  }

  clip_result Clip = BufferUntexturedQuad(Group, Geo, MinP, BarDim, Color);
  if (Clip.ClipStatus != ClipStatus_FullyClipped)
  {
    AdvanceClip(Layout, Clip.MaxClip - Layout->Basis);
  }

  return Result;
}

function void
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
BufferRectangleAt(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo,
                  v2 MinP, v2 Dim, v3 Color, r32 Z = 0.5f)
{
  BufferUntexturedQuad(Group, Geo, MinP, Dim, Color, Z);
  return;
}

function void
BufferRectangleAt(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo,
                         rect2 Rect, v3 Color, r32 Z = 0.5f)
{
  v2 MinP = Rect.Min;
  v2 Dim = Rect.Max - Rect.Min;
  BufferRectangleAt(Group, Geo, MinP, Dim, Color, Z);

  return;
}

function b32
Button(debug_ui_render_group* Group, rect2 Rect, ui_style* Style, umm InteractionId, r32 Z = 0.5f)
{
  b32 Result = False;

  v3 UseColor = Style->Color;

  interactable Interaction = Interactable(Rect, InteractionId);
  if (Hover(Group, &Interaction))
    UseColor = Style->HoverColor;

  if (Pressed(Group, &Interaction))
  {
    Result = True;
    UseColor = Style->ClickColor;
  }

  BufferRectangleAt(Group, &Group->TextGroup->UIGeo, Rect, UseColor, Z);

  return Result;
}

function void
WindowInteractions(debug_ui_render_group* Group, window_layout* Window)
{
  untextured_2d_geometry_buffer *Geo = &Group->TextGroup->UIGeo;
  textured_2d_geometry_buffer *TitleBuffer = &Group->TextGroup->TextGeo;

  if (Window->Title)
  {
    BufferValue(Window->Title, Group, &Window->Table.Layout, WHITE, Window->MaxClip);
  }

  NewRow(Window);

  r32 Z = 0.6f;

  v2 TopLeft = Window->Table.Layout.Basis;
  v2 TopRight = Window->Table.Layout.Basis + V2(Window->MaxClip.x, 0);
  v2 BottomLeft = Window->Table.Layout.Basis + V2(0, Window->MaxClip.y);
  v2 BottomRight = Window->Table.Layout.Basis + Window->MaxClip;

  {
    ui_style Style = StandardStyling(V3(0.0f, 0.5f, 0.5f));
    rect2 Rect = RectMinMax(TopLeft, TopRight + V2(0.0f, Group->Font.Size));
    if (Button(Group, Rect, &Style, (umm)"WindowTitleBar"^(umm)Window))
    {
      Window->Table.Layout.Basis += -1.0f*Group->MouseDP;
    }
  }

  v3 BorderColor = V3(1, 1, 1);
  BufferRectangleAt(Group, Geo, RectMinMax(TopLeft, TopRight + V2(0, 1)), BorderColor, Z);
  BufferRectangleAt(Group, Geo, RectMinMax(TopLeft, BottomLeft + V2(1, 0)), BorderColor, Z);
  BufferRectangleAt(Group, Geo, RectMinMax(TopRight, BottomRight + V2(1, 0)), BorderColor, Z);
  BufferRectangleAt(Group, Geo, RectMinMax(BottomLeft, BottomRight + V2(0, 1)), BorderColor, Z);


  {
    ui_style Style = StandardStyling(V3(0.8f, 0.8f, 0.0f));
    v2 Dim = V2(10);
    v2 MinP = BottomRight - (Dim/2);
    rect2 Rect = RectMinDim(MinP, Dim);
    if (Button(Group, Rect, &Style, (umm)"WindowResizeWidget"^(umm)Window))
    {
      Window->MaxClip = Max(V2(0), Window->MaxClip-Group->MouseDP);
    }
  }

  v3 BackgroundColor = V3(0.2f, 0.2f, 0.2f);
  BufferRectangleAt(Group, Geo, RectMinMax(TopLeft, BottomRight), BackgroundColor, 0.0f);

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

function void
DrawPickedChunks(debug_ui_render_group* Group, v2 LayoutBasis)
{
  debug_state* DebugState = GetDebugState();

  local_persist window_layout PickerWindow_ = WindowLayout(LayoutBasis);
  window_layout *ListingWindow = &PickerWindow_;

  ListingWindow->Table.Layout.At = V2(0,0);
  ListingWindow->Table.Layout.Clip = InvertedInfinityRectangle();
  ListingWindow->Table.Layout.Basis = LayoutBasis;

  world_chunk** PickedChunks = DebugState->PickedChunks;

  MapGpuElementBuffer(&DebugState->GameGeo);

  for (u32 ChunkIndex = 0;
      ChunkIndex < DebugState->PickedChunkCount;
      ++ChunkIndex)
  {
    world_chunk *Chunk = PickedChunks[ChunkIndex];

    u8 MainColor = Chunk == DebugState->HotChunk ? PINK : WHITE;

    interactable PickerListInteraction = StartInteractable(&ListingWindow->Table.Layout, (umm)ListingWindow);
      Column(ToString(Chunk->WorldP.x), Group, ListingWindow, MainColor, MainColor);
      Column(ToString(Chunk->WorldP.y), Group, ListingWindow, MainColor, MainColor);
      Column(ToString(Chunk->WorldP.z), Group, ListingWindow, MainColor, MainColor);
    EndInteractable(Group, ListingWindow, &PickerListInteraction);

    if (Clicked(Group, &PickerListInteraction))
    {
      DebugState->HotChunk = Chunk;
    }

    if (Button("X", Group, ListingWindow, RED))
    {
      world_chunk** SwapChunk = PickedChunks+ChunkIndex;
      if (*SwapChunk == DebugState->HotChunk)
      {
        DebugState->HotChunk = 0;
      }

      *SwapChunk = PickedChunks[DebugState->PickedChunkCount-1];
      DebugState->PickedChunkCount = DebugState->PickedChunkCount-1;
    }

    NewRow(ListingWindow);
  }
  untextured_2d_geometry_buffer *Geo = &Group->TextGroup->UIGeo;
  EndClipRect(Group, ListingWindow, Geo);

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

  world_chunk *HotChunk = DebugState->HotChunk;
  if (HotChunk)
  {
    v2 ChunkDetailWindowMaxClip = V2(950.0f, 600.0f);
    v2 WindowSpacing = V2(100, 0);

    r32 ListingWindowDimX = Min(ListingWindow->Table.Layout.Clip.Max.x, ListingWindow->MaxClip.x) - ListingWindow->Table.Layout.Clip.Min.x;

    {
      local_persist window_layout ChunkDetailWindow = WindowLayout(LayoutBasis + V2(ListingWindowDimX, 0.0f) + WindowSpacing, ChunkDetailWindowMaxClip, "ChunkDetailWindow");
      Clear(&ChunkDetailWindow.Table.Layout.At);
      Clear(&ChunkDetailWindow.Table.Layout.Clip);
      WindowInteractions(Group, &ChunkDetailWindow);

      BufferChunkDetails(Group, HotChunk, &ChunkDetailWindow);
    }


    local_persist window_layout PickerWindow = WindowLayout(LayoutBasis + V2(ChunkDetailWindowMaxClip.x, 0) + WindowSpacing*2 + V2(ListingWindowDimX, 0.0f), V2(800.0f), "PickedChunks");
    Clear(&PickerWindow.Table.Layout.At);
    Clear(&PickerWindow.Table.Layout.Clip);

    WindowInteractions(Group, &PickerWindow);

    b32 DebugButtonPressed = False;
    if ( Button("<", Group, &PickerWindow, WHITE) )
    {
      HotChunk->PointsToLeaveRemaining = Min(HotChunk->PointsToLeaveRemaining+1, HotChunk->EdgeBoundaryVoxelCount);
      DebugButtonPressed = True;
    }

    if ( Button(">", Group, &PickerWindow, WHITE) )
    {
      HotChunk->PointsToLeaveRemaining = Max(HotChunk->PointsToLeaveRemaining-1, 0);
      DebugButtonPressed = True;
    }

    const char* ButtonText = HotChunk->DrawBoundingVoxels ? "|" : "O";
    if ( Button(ButtonText, Group, &PickerWindow, WHITE) )
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

    BufferTexturedQuad( Group, &Group->TextGroup->TextGeo, MinP, QuadDim,
                        DebugTextureArraySlice_Viewport, UVsForFullyCoveredQuad(),
                        V3(1) );

    interactable Interaction = Interactable(MinP, MinP+QuadDim, (umm)"PickerWindowDragInteraction");
    input* WindowInput = Group->Input;
    if (!Pressed(Group, &Interaction))
    {
      WindowInput = 0;
    }

    UpdateGameCamera( -0.005f*Group->MouseDP,
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
      NewLine(&ChildrensLayout);
      DrawScopeBarsRecursive(Group, Geo, Scope->Child, &ChildrensLayout, Frame, TotalGraphWidth, Entropy);
      AdvanceClip(&ChildrensLayout);
      Layout->Clip.Max = Max(ChildrensLayout.Clip.Max, Layout->Clip.Max);
      Layout->Clip.Min = Max(ChildrensLayout.Clip.Min, Layout->Clip.Min);
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


  local_persist window_layout CycleGraphWindow;
  CycleGraphWindow.Table.Layout.At = V2(0,0);
  CycleGraphWindow.Table.Layout.Clip = NullClipRect;
  CycleGraphWindow.Table.Layout.Basis = BasisP;

  layout* Layout = &CycleGraphWindow.Table.Layout;

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
    BeginClipRect(&CycleGraphWindow);

    char *ThreadName = FormatString(TranArena, "Thread %u", ThreadIndex);
    Column(ThreadName, Group, &CycleGraphWindow, WHITE);
    NewRow(&CycleGraphWindow);

    debug_thread_state *ThreadState = GetThreadLocalStateFor(ThreadIndex);
    debug_scope_tree *ReadTree = ThreadState->ScopeTrees + SharedState->ReadScopeIndex;
    if (MainThreadReadTree->FrameRecorded == ReadTree->FrameRecorded)
    {
      DrawScopeBarsRecursive(Group, Geo, ReadTree->Root, Layout, &FrameCycles, TotalGraphWidth, &Entropy);
    }

    Layout->At.y = Layout->Clip.Max.y + 25; // Advance vertical at for next thread

    EndClipRect(Group, &CycleGraphWindow, Geo);
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
      BufferUntexturedQuad(Group, Geo, MinP16ms, Dim, V3(0,1,0));
    }
    {
      r32 FramePerc = 33.333333f/TotalMs;
      r32 xOffset = FramePerc*TotalGraphWidth;
      v2 MinP16ms = Layout->Basis + V2( xOffset, MinY );
      v2 MaxP16ms = Layout->Basis + V2( xOffset+MarkerWidth, Layout->At.y );
      v2 Dim = MaxP16ms - MinP16ms;
      BufferUntexturedQuad(Group, Geo, MinP16ms, Dim, V3(0,1,1));
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
    memory_arena *Memory, window_layout* CallgraphLayout, u64 TotalFrameCycles, u32 Depth)
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
    u8 MainColor = (u8)HoverAndClickExpand(Group, &CallgraphLayout->Table.Layout, UniqueScopes->Scope, WHITE, TEAL);
    BufferScopeTreeEntry(Group, UniqueScopes->Scope, CallgraphLayout, MainColor, UniqueScopes->TotalCycles, TotalFrameCycles, UniqueScopes->CallCount, Depth);

    if (UniqueScopes->Scope->Expanded)
      BufferFirstCallToEach(Group, UniqueScopes->Scope->Child, TreeRoot, Memory, CallgraphLayout, TotalFrameCycles, Depth+1);

    UniqueScopes = UniqueScopes->NextUnique;
  }

  return;
}


function rect2
DebugDrawCallGraph(debug_ui_render_group *Group, debug_state *DebugState, layout *MainLayout, r64 MaxMs)
{
  v2 MouseP = Group->MouseP;

  NewLine(MainLayout);
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

      interactable Interaction = Interactable(MinP, MinP+Offset, (umm)"CallGraphBarInteract" );
      if (Hover(Group, &Interaction))
      {
        debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
        if (FrameIndex != MainThreadState->WriteIndex % DEBUG_FRAMES_TRACKED)
        {
          DebugState->ReadScopeIndex = FrameIndex;
          Color = V3(0.8f, 0.8f, 0.0f);
        }
      }

      clip_result Clip = BufferUntexturedQuad(Group, &Group->TextGroup->UIGeo, MinP + Offset, QuadDim, Color);
      if (Clip.ClipStatus != ClipStatus_FullyClipped)
      {
        MainLayout->At.x = Clip.MaxClip.x + 5.0f;
        AdvanceClip(MainLayout, MainLayout->At + V2(0, Group->Font.Size));
      }
    }


    r32 MaxBarHeight = Group->Font.Size;
    v2 QuadDim = V2(MainLayout->At.x, 2.0f);
    {
      r32 MsPerc = (r32)SafeDivide0(33.333, MaxMs);
      r32 MinPOffset = MaxBarHeight * MsPerc;
      v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size - MinPOffset };

      BufferUntexturedQuad(Group, &Group->TextGroup->UIGeo, MinP, QuadDim, V3(1,1,0));
    }

    {
      r32 MsPerc = (r32)SafeDivide0(16.666, MaxMs);
      r32 MinPOffset = MaxBarHeight * MsPerc;
      v2 MinP = { StartingAt.x, StartingAt.y + Group->Font.Size - MinPOffset };

      BufferUntexturedQuad(Group, &Group->TextGroup->UIGeo, MinP, QuadDim, V3(0,1,0));
    }

    { // Current ReadTree info
      SetFontSize(&Group->Font, 30);
      frame_stats *Frame = DebugState->Frames + DebugState->ReadScopeIndex;
      BufferColumn(Frame->FrameMs, 4, Group, MainLayout, WHITE);
      BufferThousands(Frame->TotalCycles, Group, MainLayout, WHITE);

      u32 TotalMutexOps = GetTotalMutexOpsForReadFrame();
      BufferThousands(TotalMutexOps, Group, MainLayout, WHITE);
    }
    NewLine(MainLayout);

  END_BLOCK("Frame Ticker");

  u32 TotalThreadCount = GetWorkerThreadCount() + 1;

  debug_thread_state *MainThreadState = GetThreadLocalStateFor(0);
  debug_scope_tree *MainThreadReadTree    = MainThreadState->ScopeTrees + DebugState->ReadScopeIndex;

  local_persist window_layout CallgraphWindow;
  TIMED_BLOCK("Call Graph");

  CallgraphWindow.Table.Layout.At = V2(0,0);
  CallgraphWindow.Table.Layout.Clip = NullClipRect;
  CallgraphWindow.Table.Layout.Basis = V2(0, MainLayout->Clip.Max.y);

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

  rect2 Result = CallgraphWindow.Table.Layout.Clip;
  return Result;
}

#if 0
function void
ColumnLeft(u32 Width, const char *Text, debug_ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  u32 Len = (u32)strlen(Text);
  u32 Pad = Max(Width-Len, 0U);
  BufferValue(Text, Group, Layout, ColorIndex);
  AdvanceSpaces(Pad, Layout, &Group->Font);
}

function void
ColumnRight(s32 Width, const char *Text, debug_ui_render_group* Group, layout *Layout, u32 ColorIndex )
{
  s32 Len = (s32)strlen(Text);
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
  local_persist window_layout FunctionCallWindow = WindowLayout(BasisP);
  FunctionCallWindow.Title = "Functions";

  Clear(&FunctionCallWindow.Table.Layout.At);
  Clear(&FunctionCallWindow.Table.Layout.Clip);

  WindowInteractions(Group, &FunctionCallWindow);

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
      Column( Func->Name, Group, &FunctionCallWindow, WHITE);
      Column( ToString(Func->CallCount), Group, &FunctionCallWindow, WHITE);
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
DebugDrawDrawCalls(debug_ui_render_group *Group, layout *Layout)
{
  NewLine(Layout);
  NewLine(Layout);

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
       NewLine(Layout);
     }
  }

  return;
}


/*******************************            **********************************/
/*******************************  Arena UI  **********************************/
/*******************************            **********************************/


function b32
BufferBarGraph(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo, table *Table, r32 PercFilled, v3 Color, v2 MaxClip)
{
  r32 BarHeight = Group->Font.Size;
  r32 BarWidth = 200.0f;

  v2 MinP = Table->Layout.At + Table->Layout.Basis;
  v2 BarDim = V2(BarWidth, BarHeight);
  v2 PercBarDim = V2(BarWidth, BarHeight) * V2(PercFilled, 1);

  BufferUntexturedQuad(Group, Geo, MinP, BarDim, V3(0.25f), 0.5f, Table->Layout.Basis+MaxClip);

  rect2 BarRect = { MinP, MinP + BarDim };
  b32 Hovering = IsInsideRect(BarRect, Group->MouseP);

  if (Hovering)
    Color = {{ 1, 0, 1 }};

  BufferUntexturedQuad(Group, Geo, MinP, PercBarDim, Color, 0.5f, Table->Layout.Basis+MaxClip);

  Table->Layout.At.x += BarDim.x;
  AdvanceClip(&Table->Layout);

  return Hovering;
}

function b32
BufferArenaBargraph(table *Table, debug_ui_render_group *Group, umm TotalUsed, r32 TotalPerc, umm Remaining, v3 Color, v2 MaxClip)
{
  Column( FormatMemorySize(TotalUsed), Group, Table, WHITE, WHITE, MaxClip);
  b32 Hover = BufferBarGraph(Group, &Group->TextGroup->UIGeo, Table, TotalPerc, Color, MaxClip);
  Column( FormatMemorySize(Remaining), Group, Table, WHITE, WHITE, MaxClip);
  NewRow(Table);

  b32 Click = (Hover && Group->Input->LMB.WasPressed);
  return Click;
}

function void
BufferMemoryStatsTable(memory_arena_stats MemStats, debug_ui_render_group *Group, table *StatsTable, v2 MaxClip)
{
  Column("Allocs", Group, StatsTable, WHITE, WHITE, MaxClip);
  Column(FormatMemorySize(MemStats.Allocations), Group, StatsTable, WHITE, WHITE, MaxClip);
  NewRow(StatsTable);

  Column("Pushes", Group, StatsTable, WHITE, WHITE, MaxClip);
  Column(FormatThousands(MemStats.Pushes), Group, StatsTable, WHITE, WHITE, MaxClip);
  NewRow(StatsTable);

  Column("Remaining", Group, StatsTable, WHITE, WHITE, MaxClip);
  Column(FormatMemorySize(MemStats.Remaining), Group, StatsTable, WHITE, WHITE, MaxClip);
  NewRow(StatsTable);

  Column("Total", Group, StatsTable, WHITE, WHITE, MaxClip);
  Column(FormatMemorySize(MemStats.TotalAllocated), Group, StatsTable, WHITE, WHITE, MaxClip);
  NewRow(StatsTable);

  return;
}

function void
BufferMemoryBargraphTable(debug_ui_render_group *Group, selected_arenas *SelectedArenas, memory_arena_stats MemStats, umm TotalUsed, memory_arena *HeadArena, table *Table, v2 MaxClip)
{
  SetFontSize(&Group->Font, 22);

  NewRow(Table);
  v3 DefaultColor = V3(0.5f, 0.5f, 0.0);

  r32 TotalPerc = (r32)SafeDivide0(TotalUsed, MemStats.TotalAllocated);
  b32 ToggleAllArenas = BufferArenaBargraph(Table, Group, TotalUsed, TotalPerc, MemStats.Remaining, DefaultColor, MaxClip);
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

    b32 GotClicked = BufferArenaBargraph(Table, Group, CurrentUsed, CurrentPerc, Remaining(CurrentArena), Color, MaxClip);

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

function layout *
BufferDebugPushMetaData(debug_ui_render_group *Group, selected_arenas *SelectedArenas, umm CurrentArenaHead, window_layout* Window)
{
  push_metadata CollatedMetaTable[META_TABLE_SIZE] = {};

  layout *Layout = &Window->Table.Layout;

  SetFontSize(&Group->Font, 24);

  Column("Size", Group, Window, WHITE);
  Column("Structs", Group, Window, WHITE);
  Column("Push Count", Group, Window, WHITE);
  Column("Name", Group, Window, WHITE);
  NewRow(Window);


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
      Column( FormatMemorySize(AllocationSize), Group, Window, WHITE);
      Column( FormatThousands(Collated->StructCount), Group, Window, WHITE);
      Column( FormatThousands(Collated->PushCount), Group, Window, WHITE);
      Column(Collated->Name, Group, Window, WHITE);
      NewRow(Window);
    }

    continue;
  }


  NewLine(Layout);

  return Layout;
}

function window_layout
SubWindowAt(window_layout* Original, v2 NewBasis)
{
  window_layout Result = {};

  Result.Table.Layout.Basis = NewBasis;
  Result.MaxClip = Original->Table.Layout.Basis + Original->MaxClip - NewBasis;

  return Result;
}

function void
MergeTables(table* Src, table* Dest)
{
  v2 SrcAtRelativeToDest = GetAbsoluteAt(&Src->Layout) - Dest->Layout.Basis;
  Dest->Layout.At = Max(Dest->Layout.At, SrcAtRelativeToDest);
  AdvanceClip(&Dest->Layout);
  return;
}

function window_layout*
MergeWindowLayouts(window_layout* Src, window_layout* Dest)
{
  v2 SrcAtRelativeToDest = GetAbsoluteAt(&Src->Table.Layout) - Dest->Table.Layout.Basis;
  Dest->Table.Layout.At = Max(Dest->Table.Layout.At, SrcAtRelativeToDest);

  AdvanceClip(&Dest->Table.Layout);

  return Dest;
}

function layout
TableLayoutFromSibling(table* Sibling)
{
  layout Result = Sibling->Layout;
  Result.Clip.Min = Sibling->Layout.At;
  Result.Clip.Max = Sibling->Layout.At;

  return Result;
}

function void
DebugDrawMemoryHud(debug_ui_render_group *Group, debug_state *DebugState, window_layout* Window)
{
  Window->Table.Layout.At = V2(0);
  Window->Table.Layout.Clip = InvertedInfinityRectangle();
  Window->Title = "Memory Arenas";

  WindowInteractions(Group, Window);

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
      NewLine(&Window->Table.Layout);
      u8 Color = HoverAndClickExpand(Group, &Window->Table.Layout, Current, WHITE, TEAL);

      Column(Current->Name, Group, Window, Color);
      Column(MemorySize(MemStats.TotalAllocated), Group, Window, Color);
      Column(ToString(MemStats.Pushes), Group, Window, Color);
      NewRow(Window);
    }


    if (Current->Expanded)
    {
      SetFontSize(&Group->Font, 28);

      r32 TopOfStatsTable = Window->Table.Layout.Clip.Max.y;

      local_persist table MemoryStatsTable = {};
      MemoryStatsTable.Layout = TableLayoutFromSibling(&Window->Table);
      BufferMemoryStatsTable(MemStats, Group, &MemoryStatsTable, Window->MaxClip);
      r32 RightOfStatsTable = MemoryStatsTable.Layout.Clip.Max.x;

      local_persist table MemoryBargraphTable = {};
      MemoryBargraphTable.Layout = TableLayoutFromSibling(&MemoryStatsTable);
      BufferMemoryBargraphTable(Group, DebugState->SelectedArenas, MemStats, TotalUsed, Current->Arena, &MemoryBargraphTable, Window->MaxClip);
      r32 RightOfBargraphTable = MemoryBargraphTable.Layout.Clip.Max.x;

      window_layout TmpWindow = SubWindowAt(Window, Window->Table.Layout.Basis + V2( Max(RightOfStatsTable, RightOfBargraphTable), TopOfStatsTable));
      BufferDebugPushMetaData(Group, DebugState->SelectedArenas, HashArenaHead(Current->Arena), &TmpWindow);
      MergeWindowLayouts(&TmpWindow, Window);

      MergeTables(&MemoryStatsTable, &Window->Table);
      MergeTables(&MemoryBargraphTable, &Window->Table);
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
    layout *Layout)
{

  if (IsConnected(Network))
  {
    BufferValue("O", Group, Layout, GREEN);

    AdvanceSpaces(2, Layout, &Group->Font);

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

      AdvanceSpaces(1, Layout, &Group->Font);
      BufferValue("Id:", Group, Layout, WHITE);
      BufferColumn( Client->Id, 2, Group, Layout, WHITE);
      AdvanceSpaces(2, Layout, &Group->Font);
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


/******************************               ********************************/
/******************************  Graphics UI  ********************************/
/******************************               ********************************/


function void
DebugDrawGraphicsHud(debug_ui_render_group *Group, debug_state *DebugState, layout *Layout)
{
  NewLine(Layout);
  NewLine(Layout);

  BufferMemorySize(DebugState->BytesBufferedToCard, Group, Layout, WHITE);

  return;
}


/******************************              *********************************/
/******************************  Initialize  *********************************/
/******************************              *********************************/


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

