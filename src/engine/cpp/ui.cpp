#if PLATFORM_GL_IMPLEMENTATIONS

#define u32_COUNT_PER_QUAD (6)

/*******************************              *********************************/
/*******************************  DrawBounds  *********************************/
/*******************************              *********************************/



inline void
AdvanceClip(layout *Layout, v2 TestP)
{
  v2 AbsP = TestP + Layout->Basis;
  while (Layout)
  {
    v2 RelP = AbsP - Layout->Basis;
    Layout->DrawBounds.Min = Min(RelP, Layout->DrawBounds.Min);
    Layout->DrawBounds.Max = Max(RelP, Layout->DrawBounds.Max);

    Layout = Layout->Prev;
  }
}

inline void
AdvanceClip(layout *Layout)
{
  AdvanceClip(Layout, Layout->At);
}

r32
GetXOffsetForHorizontalBar(u64 StartCycleOffset, u64 FrameTotalCycles, r32 TotalGraphWidth)
{
  r32 XOffset = ((r32)StartCycleOffset/(r32)FrameTotalCycles)*TotalGraphWidth;
  return XOffset;
}



/*****************************                ********************************/
/*****************************  Text Helpers  ********************************/
/*****************************                ********************************/


link_internal void
AdvanceSpaces(u32 N, layout *Layout, v2 FontSize)
{
  Layout->At.x += (N*FontSize.x);
  AdvanceClip(Layout);
  return;
}

link_internal void
NewLine(layout *Layout)
{
  r32 VerticalAdvance = 0;
  if (Layout->At.x == 0.0f)
  {
    VerticalAdvance = Global_Font.Size.y;
  }

  Layout->At.y = Layout->DrawBounds.Max.y + VerticalAdvance;
  Layout->At.x = 0.0f;

  AdvanceClip(Layout);

  return;
}

link_internal v2
GetTextBounds(u32 TextLength, font* Font)
{
  v2 Result = {};
  Result.x = TextLength * Font->Size.x;
  Result.y = Font->Size.y;
  return Result;
}

link_internal v3
SelectColorState(render_state* RenderState, ui_style *Style)
{
  v3 Result = Style->Color;

  if (RenderState->Hover)
  {
    Result = Style->HoverColor;
  }
  if (RenderState->Pressed)
  {
    Result = Style->PressedColor;
  }
  if (RenderState->Clicked)
  {
    Result = Style->ClickedColor;
  }

  return Result;
}



/******************************                *******************************/
/******************************  2D Buffering  *******************************/
/******************************                *******************************/



link_internal void
ClearFramebuffer(framebuffer FBO)
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, FBO.ID);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
}

link_internal void
ClearFramebuffers(render_entity_to_texture_group *Group)
{
  ClearFramebuffer(Group->GameGeoFBO);
}

link_internal void
FlushBuffer(debug_text_render_group *TextGroup, untextured_2d_geometry_buffer *Buffer, v2 ScreenDim)
{
  TIMED_FUNCTION();

  if (TextGroup)
  {
    GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
    SetViewport(ScreenDim);
    UseShader(&TextGroup->SolidUIShader);

    u32 AttributeIndex = 0;
    BufferVertsToCard(TextGroup->SolidUIVertexBuffer, Buffer, &AttributeIndex);
    BufferColorsToCard(TextGroup->SolidUIColorBuffer, Buffer, &AttributeIndex);

    Draw(Buffer->At);
    Buffer->At = 0;

    GL.DisableVertexAttribArray(0);
    GL.DisableVertexAttribArray(1);

    AssertNoGlErrors;
  }
  else
  {
    Warn("FlushBuffer call issued without a RenderGroup!");
  }

  return;
}

link_internal void
FlushBuffer(debug_text_render_group *TextGroup, textured_2d_geometry_buffer *Geo, v2 ScreenDim)
{
  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
  SetViewport(ScreenDim);
  GL.UseProgram(TextGroup->Text2DShader.ID);

  GL.ActiveTexture(GL_TEXTURE0);
  GL.BindTexture(GL_TEXTURE_2D_ARRAY, TextGroup->DebugTextureArray->ID);

  GL.Uniform1i(TextGroup->TextTextureUniform, 0); // Assign texture unit 0 to the TextTexureUniform

  u32 AttributeIndex = 0;
  BufferVertsToCard( TextGroup->SolidUIVertexBuffer, Geo, &AttributeIndex);
  BufferUVsToCard(   TextGroup->SolidUIUVBuffer,     Geo, &AttributeIndex);
  BufferColorsToCard(TextGroup->SolidUIColorBuffer,  Geo, &AttributeIndex);

  GL.Enable(GL_BLEND);
  GL.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Draw(Geo->At);
  Geo->At = 0;

  GL.Disable(GL_BLEND);

  GL.DisableVertexAttribArray(0);
  GL.DisableVertexAttribArray(1);
  GL.DisableVertexAttribArray(2);
  AssertNoGlErrors;
}

link_internal void
FlushBuffers(debug_ui_render_group *UiGroup, v2 ScreenDim)
{
  if (UiGroup->TextGroup)
  {
    FlushBuffer(UiGroup->TextGroup, &UiGroup->Geo, ScreenDim);
    FlushBuffer(UiGroup->TextGroup, &UiGroup->TextGroup->Geo, ScreenDim);
  }

  return;
}

link_internal void
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

link_internal rect2
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

link_internal rect2
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

template <typename T> link_internal void
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

template <typename T> link_internal void
BufferColors(debug_ui_render_group *Group, T *Geo, v3 Color)
{
  if (!BufferHasRoomFor(Geo, u32_COUNT_PER_QUAD))
    FlushBuffer(Group->TextGroup, Geo, Group->ScreenDim);

  BufferColorsDirect(Geo, Color);
}

template <typename T>link_internal clip_result
BufferQuadDirect(T *Geo, v2 MinP, v2 Dim, r32 Z, v2 ScreenDim, v2 MaxClip)
{
  Assert(ScreenDim.x > 0);
  Assert(ScreenDim.y > 0);

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

link_internal clip_result
BufferTexturedQuad(debug_ui_render_group *Group,
                   debug_texture_array_slice TextureSlice,
                   v2 MinP, v2 Dim, rect2 UV, v3 Color, r32 Z, v2 MaxClip)
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

link_internal clip_result
BufferUntexturedQuad(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo,
                     v2 MinP, v2 Dim, v3 Color, r32 Z, v2 MaxClip)
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

link_internal clip_result
BufferUntexturedQuad(debug_ui_render_group *Group, untextured_2d_geometry_buffer *Geo,
                         rect2 Rect, v3 Color, r32 Z, v2 MaxClip)
{
  v2 MinP = Rect.Min;
  v2 Dim = Rect.Max - Rect.Min;
  clip_result Result = BufferUntexturedQuad(Group, Geo, MinP, Dim, Color, Z, MaxClip);
  return Result;
}

link_internal r32
BufferChar(debug_ui_render_group *Group, u8 Char, v2 MinP, v2 FontSize, v3 Color, r32 Z, v2 MaxClip)
{
  rect2 UV = UVsForChar(Char);

  // Lightly text gets a dark shadow, dark text gets a light shadow
  v3 ShadowColor = V3(0.1f);
  if (Length(Color) < 0.6f)
  {
    ShadowColor = V3(0.9f);
  }

  v2 ShadowOffset = 0.075f*FontSize;
  BufferTexturedQuad( Group, DebugTextureArraySlice_Font,
                      MinP+ShadowOffset, FontSize, UV, ShadowColor, Z, MaxClip);

  BufferTexturedQuad( Group, DebugTextureArraySlice_Font,
                      MinP, FontSize, UV, Color, Z, MaxClip);

  r32 DeltaX = FontSize.x;
  return DeltaX;
}

link_internal r32
BufferChar(debug_ui_render_group *Group, u8 Char, v2 MinP, v2 FontSize, u32 Color, r32 Z, v2 MaxClip)
{
  v3 ColorVector = GetColorData(DefaultPalette, Color).xyz;
  r32 Result = BufferChar(Group, Char, MinP, FontSize, ColorVector, Z, MaxClip);
  return Result;
}

link_internal void
BufferBorder(debug_ui_render_group *Group, rect2 Rect, v3 Color, r32 Z, v2 MaxClip)
{
  v2 TopLeft     = Rect.Min;
  v2 BottomRight = Rect.Max;
  v2 TopRight    = V2(Rect.Max.x, Rect.Min.y);
  v2 BottomLeft  = V2(Rect.Min.x, Rect.Max.y);

  r32 BorderWidth = 1;
  rect2 TopRect    = RectMinMax(TopLeft ,    TopRight    + V2(0, BorderWidth));
  rect2 BottomRect = RectMinMax(BottomLeft,  BottomRight - V2(0, BorderWidth));
  rect2 LeftRect   = RectMinMax(TopLeft ,    BottomLeft  + V2(BorderWidth, 0));
  rect2 RightRect  = RectMinMax(TopRight,    BottomRight + V2(BorderWidth, 0));

  BufferUntexturedQuad(Group, &Group->Geo, TopRect,    Color, Z, MaxClip);
  BufferUntexturedQuad(Group, &Group->Geo, LeftRect,   Color, Z, MaxClip);
  BufferUntexturedQuad(Group, &Group->Geo, RightRect,  Color, Z, MaxClip);
  BufferUntexturedQuad(Group, &Group->Geo, BottomRect, Color, Z, MaxClip);

  return;
}

link_internal void
BufferBorder(debug_ui_render_group *Group, interactable* PickerListInteraction, v3 Color, r32 Z, v2 MaxClip)
{
  rect2 Bounds = RectMinMax(PickerListInteraction->MinP, PickerListInteraction->MaxP);
  BufferBorder(Group, Bounds, Color, Z, MaxClip);
  return;
}

link_internal void
AdvanceLayoutStackBy(v2 Delta, layout* Layout)
{
  while (Layout) {
    Layout->At += Delta;
    AdvanceClip(Layout);
    Layout = Layout->Prev;
  }
}

link_internal void
BufferValue(counted_string Text, debug_ui_render_group *Group, layout* Layout, v3 Color, ui_style* Style = &DefaultStyle, r32 Z = 1.0f, v2 MaxClip = DISABLE_CLIPPING)
{
  r32 xDelta = 0;

  for ( u32 CharIndex = 0;
      CharIndex < Text.Count;
      CharIndex++ )
  {
    v2 MinP = GetAbsoluteAt(Layout) + V2(xDelta, 0);
    xDelta += BufferChar(Group, (u8)Text.Start[CharIndex], MinP, Style->Font.Size, Color, Z, MaxClip);
  }

  AdvanceLayoutStackBy(V2(xDelta, 0), Layout);

  v2 MaxP = Layout->At + V2(0, Style->Font.Size.y);
  AdvanceClip(Layout, MaxP);

#if DEBUG_UI_OUTLINE_VALUES
  v2 EndingP = Layout->Basis + MaxClipP;
  BufferBorder(Group, RectMinMax(StartingP, EndingP), V3(0, 0, 1), Z, DISABLE_CLIPPING);
#endif

  return;
}

link_internal void
BufferValue(counted_string Text, debug_ui_render_group *Group, render_state* RenderState, ui_style* Style)
{
  layout* Layout = RenderState->Layout;
  r32 Z          = GetZ(zDepth_Text, RenderState->Window);
  v2 MaxClip     = GetAbsoluteMaxClip(RenderState->Window);
  v3 Color       = SelectColorState(RenderState, Style);

  BufferValue(Text, Group, Layout, Color, Style, Z, MaxClip);
  return;
}

link_internal r32
BufferTextAt(debug_ui_render_group *Group, v2 BasisP, counted_string Text, v2 FontSize, v3 Color, r32 Z, v2 ClipMax = DISABLE_CLIPPING)
{
  r32 DeltaX = 0;

  for ( u32 CharIndex = 0;
      CharIndex < Text.Count;
      CharIndex++ )
  {
    v2 MinP = BasisP + V2(FontSize.x*CharIndex, 0);
    DeltaX += BufferChar(Group, (u8)Text.Start[CharIndex], MinP, FontSize, Color, Z, ClipMax);
    continue;
  }


  return DeltaX;
}



// TODO(Jesse, id: 104, tags: cleanup, robustness): Test this actually gets respected!!
#define INVALID_RENDER_COMMAND_INDEX (u32_MAX)

link_internal u32
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
    // TODO(Jesse, id: 105, tags: id_104, cleanup, robustness): Test this actually gets respected!!
    Result = INVALID_RENDER_COMMAND_INDEX;
    Error("Exhausted RenderCommandBuffer Space!");
  }

  return Result;
}

link_internal void
PushNewRow(debug_ui_render_group *Group)
{
  ui_render_command Command = {
    .Type = type_ui_render_command_new_row
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

link_internal void
Text(debug_ui_render_group* Group, counted_string String, ui_style *Style = 0)
{
  ui_render_command Command = {
    .Type = type_ui_render_command_text,
    .ui_render_command_text = {
      .String = String,
      .Style = Style ? *Style : DefaultStyle
    }
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

link_internal void
StartColumn(debug_ui_render_group *Group, ui_style* Style = 0, v4 Padding = V4(0), column_render_params Params = ColumnRenderParam_RightAlign)
{
  ui_render_command Command = {
    .Type = type_ui_render_command_column_start,
    .ui_render_command_column_start.Style           = Style? *Style : DefaultStyle,
    .ui_render_command_column_start.Params          = Params,
    .ui_render_command_column_start.Layout = {
      .Padding = Padding,
    }
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

link_internal void
EndColumn(debug_ui_render_group* Group)
{
  ui_render_command Command = {
    .Type = type_ui_render_command_column_end,
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

link_internal void
PushColumn(debug_ui_render_group *Group, counted_string String, ui_style* Style = 0, v4 Padding = DefaultColumnPadding, column_render_params Params = ColumnRenderParam_RightAlign)
{
  StartColumn(Group, Style, Padding, Params);
    Text(Group, String, Style);
  EndColumn(Group);

  return;
}

link_internal void
PushTextAt(debug_ui_render_group *Group, counted_string Text, v2 At, v2 ClipMax)
{
  ui_render_command Command = {
    .Type           = type_ui_render_command_text_at,

    .ui_render_command_text_at.Text    = Text,
    .ui_render_command_text_at.At      = At,
    .ui_render_command_text_at.MaxClip = ClipMax,
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

link_internal void
PushTooltip(debug_ui_render_group *Group, counted_string Text)
{
  PushTextAt(Group, Text, *Group->MouseP+V2(12, -7), DISABLE_CLIPPING);
  return;
}

link_internal void
PushTexturedQuad(debug_ui_render_group *Group, debug_texture_array_slice TextureSlice, z_depth zDepth)
{
  ui_render_command Command = {
    .Type = type_ui_render_command_textured_quad,

    .ui_render_command_textured_quad.TextureSlice = TextureSlice,
    .ui_render_command_textured_quad.zDepth = zDepth,
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

link_internal void
PushUntexturedQuadAt(debug_ui_render_group* Group, v2 At, v2 QuadDim, z_depth zDepth, ui_style *Style = 0 )
{
  ui_render_command Command =
  {
    .Type = type_ui_render_command_untextured_quad_at,
    .ui_render_command_untextured_quad_at =
    {
      .QuadDim = QuadDim,
      .zDepth  = zDepth,
      .Style = Style? *Style : DefaultStyle,
      .Layout  =
      {
        .At         = At,
        .DrawBounds = InvertedInfinityRectangle(),
      }
    }
  };

  PushUiRenderCommand(Group, &Command);
}

link_internal void
PushUntexturedQuad(debug_ui_render_group* Group, v2 Offset, v2 QuadDim, z_depth zDepth, ui_style *Style = 0, v4 Padding = V4(0), quad_render_params Params = QuadRenderParam_Default )
{
  ui_render_command Command = {
    .Type = type_ui_render_command_untextured_quad,

    .ui_render_command_untextured_quad =
    {
      .QuadDim = QuadDim,
      .Params  = Params,
      .zDepth  = zDepth,
      .Style   = Style? *Style : DefaultStyle,
      .Layout  =
      {
        .At = Offset,
        .Padding = Padding,
        .DrawBounds = InvertedInfinityRectangle(),
      },
    }
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

link_internal void
PushButtonEnd(debug_ui_render_group *Group, button_end_params Params = ButtonEndParam_NoOp)
{
  ui_render_command Command = {
    .Type = type_ui_render_command_button_end,
    .ui_render_command_button_end.Params = Params,
  };
  PushUiRenderCommand(Group, &Command);
  return;
}

link_internal interactable_handle
PushButtonStart(debug_ui_render_group *Group, umm InteractionId, ui_style* Style = 0)
{
  ui_render_command Command = {
    .Type = type_ui_render_command_button_start,
    .ui_render_command_button_start.ID = InteractionId,
    .ui_render_command_button_start.Style = Style ? *Style : DefaultStyle,
  };

  PushUiRenderCommand(Group, &Command);

  interactable_handle Handle = {
    .Id = InteractionId
  };

  return Handle;
}

link_internal ui_element_reference
PushTableStart(debug_ui_render_group* Group, relative_position Position = Position_None,  ui_element_reference RelativeTo = {})
{
  ui_render_command Command = {
    .Type = type_ui_render_command_table_start,

    .ui_render_command_table_start.RelativeTo = RelativeTo,
    .ui_render_command_table_start.Position = Position,
  };

  u32 ElementIndex = PushUiRenderCommand(Group, &Command);

  ui_element_reference Result = {
    .Index = ElementIndex,
  };

  return Result;
}

link_internal void
PushTableEnd(debug_ui_render_group *Group)
{
  ui_render_command Command = {
    .Type = type_ui_render_command_table_end,
  };

  PushUiRenderCommand(Group, &Command);

  return;
}

link_internal void
PushBorder(debug_ui_render_group *Group, rect2 BoundsRelativeToCurrentWindow, v3 Color)
{
  ui_render_command Command = {
    .Type = type_ui_render_command_border,
    .ui_render_command_border.Bounds = BoundsRelativeToCurrentWindow,
    .ui_render_command_border.Color = Color,
  };

  PushUiRenderCommand(Group, &Command);

}

link_internal void
PushWindowStart(debug_ui_render_group *Group, window_layout *Window)
{
  umm TitleBarInteractionId = (umm)"WindowTitleBar"^(umm)Window;
  interactable_handle TitleBarHandle = { .Id = TitleBarInteractionId };
  if (Pressed(Group, &TitleBarHandle))
  {
    Window->Basis -= *Group->MouseDP; // TODO(Jesse, id: 107, tags: cleanup, speed): Can we compute this with MouseP to avoid a frame of input delay?
  }

  umm ResizeHandleInteractionId = (umm)"WindowResizeWidget"^(umm)Window;
  interactable_handle ResizeHandle = { .Id = ResizeHandleInteractionId };

  v2 TitleBounds = V2(Window->Title.Count*Global_Font.Size.x, Global_Font.Size.y);
  Window->MaxClip = Max(TitleBounds, Window->MaxClip);

  if (Pressed(Group, &ResizeHandle))
  {
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


  ui_render_command Command = {
    .Type = type_ui_render_command_window_start,

    .ui_render_command_window_start = {
      .Window = Window,
      .Layout = {
        .Basis = Window->Basis
      }
    }
  };

  PushUiRenderCommand(Group, &Command);

  // NOTE(Jesse): Must come first to take precedence over the title bar when clicking
  v2 Dim = V2(20);
  PushButtonStart(Group, ResizeHandleInteractionId);
    PushUntexturedQuadAt(Group, GetAbsoluteMaxClip(Window)-Dim, Dim, zDepth_Border);
  PushButtonEnd(Group);

  ui_style TextStyle = UiStyleFromLightestColor(V3(0.25f));
  PushButtonStart(Group, TitleBarInteractionId);
    Text(Group, Window->Title, &TextStyle);
    Text(Group, CSz(" "));
    Text(Group, CS(Window->InteractionStackIndex), &TextStyle);
    PushUntexturedQuadAt(Group, Window->Basis, V2(Window->MaxClip.x, Global_Font.Size.y), zDepth_TitleBar);
  PushButtonEnd(Group);

  PushBorder(Group, GetBounds(Window), V3(1.f));

  ui_style BackgroundStyle = UiStyleFromLightestColor(V3(0.25f));
  PushUntexturedQuadAt(Group, Window->Basis, Window->MaxClip, zDepth_Background, &BackgroundStyle);

  return;
}

link_internal void
PushWindowEnd(debug_ui_render_group *Group, window_layout *Window)
{
  ui_render_command EndCommand = {};
  EndCommand.Type = type_ui_render_command_window_end;
  EndCommand.ui_render_command_window_end.Window = Window;
  PushUiRenderCommand(Group, &EndCommand);
  return;
}



/*********************************           *********************************/
/*********************************  Buttons  *********************************/
/*********************************           *********************************/



link_internal button_interaction_result
ButtonInteraction(debug_ui_render_group* Group, rect2 Bounds, umm InteractionId, window_layout *Window, ui_style *Style)
{
  button_interaction_result Result = {};

  interactable Interaction = Interactable(Bounds, InteractionId, Window);

#if DEBUG_UI_OUTLINE_BUTTONS
  BufferBorder(Group, Rect2(Interaction), V3(1,0,0), 1.0f, DISABLE_CLIPPING);
#endif

  if (Hover(Group, &Interaction))
  {
    Result.Hover = True;
    Style->Color = Style->HoverColor;
  }

  if (Clicked(Group, &Interaction))
  {
    Result.Clicked = True;
    Style->Color = Style->ClickedColor;
  }

  if (Pressed(Group, &Interaction))
  {
    Result.Pressed = True;
    Style->Color = Style->ClickedColor;
  }

  if (Style->IsActive && !Result.Pressed)
  {
    Style->Color = Style->ActiveColor;
  }

  return Result;
}

link_internal b32
Button(debug_ui_render_group* Group, counted_string ButtonName, umm ButtonId, ui_style* Style = 0, v4 Padding = DefaultButtonPadding)
{
  // TODO(Jesse, id: 108, tags: cleanup, potential_bug): Do we have to pass the style to both of these functions, and is that a good idea?
  interactable_handle Button = PushButtonStart(Group, ButtonId, Style);
    PushColumn(Group, ButtonName, Style, Padding);
  PushButtonEnd(Group);

  b32 Result = Clicked(Group, &Button);
  return Result;
}



/**************************                      *****************************/
/**************************  Command Processing  *****************************/
/**************************                      *****************************/



#define GetCommandAs(TypeName, CommandBuffer, CommandIndex)                     \
  &(GetCommand((CommandBuffer), (CommandIndex))->ui_render_command_##TypeName); \
  ui_render_command* TempCommand = GetCommand((CommandBuffer), (CommandIndex)); \
  Assert(TempCommand->Type == type_ui_render_command_##TypeName)


#define RenderCommandAs(TypeName, Command)                                  \
  (ui_render_command_##TypeName *)&(Command)->ui_render_command_##TypeName; \
  Assert((Command)->Type == type_ui_render_command_##TypeName)


link_internal void
PushLayout(layout** Dest, layout* Layout)
{
  Assert(!Layout->Prev);
  Layout->Prev = *Dest;
  *Dest = Layout;

  v2 Pad = V2(Layout->Padding.Left, Layout->Padding.Top);
  AdvanceLayoutStackBy(Pad, Layout);
}

link_internal layout*
PopLayout(layout** Layout)
{
  layout* PoppedLayout = *Layout;

  v4 Padding = PoppedLayout->Padding;
  v2 ClipTest = PoppedLayout->DrawBounds.Max + V2(0, Padding.Bottom);
  AdvanceClip(PoppedLayout, ClipTest);

  v2 Advance = V2(Padding.Right, -Padding.Top);
  AdvanceLayoutStackBy(Advance, PoppedLayout);

  layout* PrevLayout = PoppedLayout->Prev;
  PoppedLayout->Prev = 0;
  *Layout = PrevLayout;

  return PoppedLayout;
}

link_internal ui_render_command*
GetCommand(ui_render_command_buffer* CommandBuffer, u32 CommandIndex)
{
  ui_render_command* Command = 0;
  if (CommandIndex < CommandBuffer->CommandCount)
    { Command = CommandBuffer->Commands+CommandIndex; }
  return Command;
}

link_internal window_layout*
GetHighestWindow(debug_ui_render_group* Group, ui_render_command_buffer* CommandBuffer)
{
  u64 HighestInteractionStackIndex = 0;
  window_layout* HighestWindow = 0;

  for (u32 CommandIndex = 0;
      CommandIndex < CommandBuffer->CommandCount;
      ++CommandIndex)
  {
    ui_render_command *Command = GetCommand(CommandBuffer, CommandIndex);
    switch(Command->Type)
    {
      case type_ui_render_command_window_start:
      {
        ui_render_command_window_start* WindowStart = RenderCommandAs(window_start, Command);
        window_layout *TestWindow = WindowStart->Window;

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

link_internal void
ProcessButtonStart(debug_ui_render_group* Group, render_state* RenderState, umm ButtonId)
{
  if (ButtonId == Group->HoverInteractionId)
  {
    Group->HoverInteractionId = 0;
    RenderState->Hover = True;
  }
  if (ButtonId == Group->ClickedInteractionId)
  {
    Group->ClickedInteractionId = 0;
    RenderState->Clicked = True;
  }
  if (ButtonId == Group->PressedInteractionId)
  {
    // Intentionally reset to 0 outside of this bonsai_function, because it's
    // dependant on the mouse buttons being released.
    // TODO(Jesse, id: 109, tags: cleanup, robustness): Reset this in here?
    RenderState->Pressed = True;
  }

  return;
}

link_internal void
ProcessButtonEnd(debug_ui_render_group *Group, umm InteractionId, render_state* RenderState, rect2 AbsButtonBounds, ui_style* Style)
{
  Assert(InteractionId);

  button_interaction_result Button = ButtonInteraction( Group,
                                                        AbsButtonBounds,
                                                        InteractionId,
                                                        RenderState->Window,
                                                        Style);

  if (Button.Hover)
  {
    Group->HoverInteractionId = InteractionId;
  }

  if (Button.Clicked)
  {
    Group->ClickedInteractionId = InteractionId;
  }

  if (Button.Pressed)
  {
    Group->PressedInteractionId = InteractionId;
  }

  RenderState->Hover = False;
  RenderState->Pressed = False;
  RenderState->Clicked = False;

  return;
}

link_internal void
ProcessTexturedQuadPush(debug_ui_render_group* Group, ui_render_command_textured_quad *Command, render_state* RenderState)
{
  v2 MinP    = GetAbsoluteAt(RenderState->Layout);
  v2 Dim     = RenderState->Window->MaxClip;
  r32 Z      = GetZ(Command->zDepth, RenderState->Window);
  v2 MaxClip = GetAbsoluteMaxClip(RenderState->Window);

  clip_result Clip = BufferTexturedQuad( Group, Command->TextureSlice, MinP, Dim, UVsForFullyCoveredQuad(), V3(1), Z, MaxClip);

  Command->Layout.Basis = MinP;
  Command->Layout.DrawBounds.Max = Clip.MaxClip - MinP;
}

link_internal void
ProcessUntexturedQuadAtPush(debug_ui_render_group* Group, ui_render_command_untextured_quad_at *Command, render_state* RenderState)
{
  v2 MaxClip = GetAbsoluteMaxClip(RenderState->Window);
  v2 MinP    = GetAbsoluteAt(&Command->Layout);
  v2 Dim     = Command->QuadDim;
  v3 Color   = SelectColorState(RenderState, &Command->Style);
  r32 Z      = GetZ(Command->zDepth, RenderState->Window);

  BufferUntexturedQuad(Group, &Group->Geo, MinP, Dim, Color, Z, MaxClip);

  AdvanceClip(&Command->Layout, MinP);
  AdvanceClip(&Command->Layout, MinP + Dim);

  return;
}

link_internal void
ProcessUntexturedQuadPush(debug_ui_render_group* Group, ui_render_command_untextured_quad *Command, render_state* RenderState)
{
  v2 MaxClip = GetAbsoluteMaxClip(RenderState->Window);
  v2 MinP    = GetAbsoluteAt(RenderState->Layout);
  v2 Dim     = Command->QuadDim;
  v3 Color   = SelectColorState(RenderState, &Command->Style);
  r32 Z      = GetZ(Command->zDepth, RenderState->Window);

  BufferUntexturedQuad(Group, &Group->Geo, MinP, Dim, Color, Z, MaxClip);

  if (Command->Params & QuadRenderParam_AdvanceClip)
  {
    AdvanceClip(RenderState->Layout, RenderState->Layout->At);
    AdvanceClip(RenderState->Layout, RenderState->Layout->At + Dim);
  }

  if (Command->Params & QuadRenderParam_AdvanceLayout)
  {
    RenderState->Layout->At.x += Dim.x;
    AdvanceClip(RenderState->Layout);
  }

  return;
}

link_internal window_sort_params
GetWindowSortParams(ui_render_command_buffer *CommandBuffer)
{
  window_sort_params Result = {};

  {
    u32 CommandIndex = 0;
    ui_render_command *Command = GetCommand(CommandBuffer, CommandIndex++);
    while (Command)
    {
      switch(Command->Type)
      {
        case type_ui_render_command_window_start:
        {
          ++Result.Count;
          ui_render_command_window_start* WindowStart = RenderCommandAs(window_start, Command);
          Result.LowestInteractionStackIndex = Min(Result.LowestInteractionStackIndex, WindowStart->Window->InteractionStackIndex);
        } break;
        default : {} break;
      }

      Command = GetCommand(CommandBuffer, CommandIndex++);
    }
  }


  Result.SortKeys = Allocate(sort_key, TranArena, Result.Count);

  {
    u32 AtKey = 0;
    u32 CommandIndex = 0;
    ui_render_command *Command = GetCommand(CommandBuffer, CommandIndex++);
    while (Command)
    {
      switch(Command->Type)
      {
        case type_ui_render_command_window_start:
        {
          ui_render_command_window_start* WindowStart = RenderCommandAs(window_start, Command);
          Result.SortKeys[AtKey++] = { .Index = CommandIndex-1, .Value = WindowStart->Window->InteractionStackIndex };
        } break;
        default : {} break;
      }

      Command = GetCommand(CommandBuffer, CommandIndex++);
    }
  }

  return Result;
}

link_internal void
BubbleSort(sort_key* Keys, u32 Count)
{
  for (u32 Ignored = 0;
      Ignored < Count;
      ++Ignored)
  {
    b32 Sorted = True;

    for (u32 Inner = 0;
        Inner < (Count-1);
        ++Inner)
    {
      sort_key* KeyA = Keys+Inner;
      sort_key* KeyB = Keys+Inner+1;

      if (KeyA->Value < KeyB->Value)
      {
        sort_key Temp = *KeyA;
        *KeyA = *KeyB;
        *KeyB = Temp;
        Sorted = False;
      }
    }

    if (Sorted) break;
  }

  return;
}

link_internal void
SetWindowZDepths(ui_render_command_buffer *CommandBuffer)
{
  window_sort_params WindowSortParams = GetWindowSortParams(CommandBuffer);

  BubbleSort(WindowSortParams.SortKeys, WindowSortParams.Count);

  r64 SliceInterval = 1.0/(r64)WindowSortParams.Count;
  SliceInterval -= SliceInterval*0.0001;

  for (u32 SortKeyIndex = 0;
      SortKeyIndex < WindowSortParams.Count;
      ++SortKeyIndex)
  {
    u32 CommandIndex = (u32)WindowSortParams.SortKeys[SortKeyIndex].Index;
    window_layout* Window = CommandBuffer->Commands[CommandIndex].ui_render_command_window_start.Window;

    r64 Basis = 1.0 - (SliceInterval*(r64)SortKeyIndex) - SliceInterval;
    Window->zBackground = (r32)(Basis);
    Window->zTitleBar   = (r32)(Basis + (SliceInterval*0.1));
    Window->zText       = (r32)(Basis + (SliceInterval*0.2));
    Window->zBorder     = (r32)(Basis + (SliceInterval*0.3));
    Assert(Window->zBackground <= 1.0f);
    Assert(Window->zBackground >= 0.0f);

    Assert(Window->zText <= 1.0f);
    Assert(Window->zText >= 0.0f);

    Assert(Window->zTitleBar <= 1.0f);
    Assert(Window->zTitleBar >= 0.0f);

    Assert(Window->zBorder <= 1.0f);
    Assert(Window->zBorder >= 0.0f);
  }

  return;
}

link_internal find_command_result
FindPreviousCommand(ui_render_command_buffer* CommandBuffer, ui_render_command_type CommandType, u32 StartingIndex)
{
  Assert(StartingIndex < CommandBuffer->CommandCount);

  find_command_result Result = {};
  for (u32 CommandIndex = StartingIndex-1;
      ;
      --CommandIndex)
  {
    ui_render_command* Command = GetCommand(CommandBuffer, CommandIndex);
    if (Command->Type == CommandType)
    {
        Result.Command = Command;
        Result.Index = CommandIndex;
        break;
    }

    if (CommandIndex == 0) { break; }
  }

  return Result;
}

link_internal u32
FindPreviousButtonStart(ui_render_command_buffer* CommandBuffer, u32 StartingIndex)
{
  u32 Result = FindPreviousCommand(CommandBuffer, type_ui_render_command_button_start, StartingIndex).Index;
  return Result;
}

link_internal rect2
FindAbsoluteDrawBoundsBetween(ui_render_command_buffer* CommandBuffer, u32 FirstCommand, u32 OnePastLastCommand)
{
  Assert(FirstCommand < CommandBuffer->CommandCount);
  Assert(OnePastLastCommand <= CommandBuffer->CommandCount);

  rect2 Result = InvertedInfinityRectangle();

  for (u32 CommandIndex = FirstCommand;
      CommandIndex < OnePastLastCommand;
      ++CommandIndex)
  {
    ui_render_command* Command = GetCommand(CommandBuffer, CommandIndex);
    switch(Command->Type)
    {

      poof(
        func (ui_render_command RenderCommandDef)
        {
          RenderCommandDef.map_members(Member)
          {
            Member.is_union?
            {
              Member.map_members(UMember).containing(layout)
              {
                case type_(UMember.type):
                {
                  Result.Max = Max(Result.Max, GetAbsoluteDrawBoundsMax(&Command->(UMember.name).Layout));
                  Result.Min = Min(Result.Min, GetAbsoluteDrawBoundsMin(&Command->(UMember.name).Layout));
                } break;
              }
            }
          }
        }
      )
#include <generated/anonymous_function_ui_render_command_RuTTrHiW.h>

      default: {} break;
    }

    continue;
  }

  return Result;
}

link_internal rect2
FindRelativeDrawBoundsBetween(ui_render_command_buffer* CommandBuffer, v2 RelativeTo, u32 FirstCommand, u32 OnePastLastCommand)
{
  rect2 Result = FindAbsoluteDrawBoundsBetween(CommandBuffer, FirstCommand, OnePastLastCommand);

  Result.Min -= RelativeTo;
  Result.Max -= RelativeTo;

  return Result;
}

link_internal u32
PreprocessTable(ui_render_command_buffer* CommandBuffer, u32 StartingIndex)
{
  u32 OnePastTableEnd = StartingIndex;
  u16 ColumnCount = 0;

  {
    u32 NextCommandIndex = StartingIndex;
    u16 NextColumnIndex = 0;

    b32 FoundEnd = False;
    ui_render_command* Command = GetCommand(CommandBuffer, NextCommandIndex++);

    r32 *CurrentWidth = 0;
    while (Command && !FoundEnd)
    {
      switch(Command->Type)
      {
          case type_ui_render_command_column_start:
          {
            ++NextColumnIndex;
            ColumnCount = (u16)Max(ColumnCount, NextColumnIndex);
            Assert(NextColumnIndex <= u16_MAX);

            ui_render_command_column_start* TypedCommand = RenderCommandAs(column_start, Command);
            CurrentWidth = &TypedCommand->Width;
            *CurrentWidth += TypedCommand->Layout.Padding.Left + TypedCommand->Layout.Padding.Right;
          } break;

          case type_ui_render_command_text:
          {
            ui_render_command_text* TypedCommand = RenderCommandAs(text, Command);
            Assert(CurrentWidth);
            *CurrentWidth += (TypedCommand->String.Count * Global_Font.Size.x);
          } break;

          case type_ui_render_command_new_row:
          {
            CurrentWidth = 0;
            NextColumnIndex = 0;
          } break;

          case type_ui_render_command_table_end:
          {
            FoundEnd = True;
            OnePastTableEnd = NextCommandIndex;
          } break;

          default: {} break;
      }

      Command = GetCommand(CommandBuffer, NextCommandIndex++);
    }
  }

  r32* MaxColumnWidths = Allocate(r32, TranArena, ColumnCount);

  {
    u32 NextColumnIndex = 0;
    for (u32 CommandIndex = StartingIndex;
        CommandIndex < OnePastTableEnd;
        ++CommandIndex)
    {
      ui_render_command* Command =  GetCommand(CommandBuffer, CommandIndex);
      switch(Command->Type)
      {
          case type_ui_render_command_column_start:
          {
            Assert(NextColumnIndex < ColumnCount);
            ui_render_command_column_start* TypedCommand = RenderCommandAs(column_start, Command);
            MaxColumnWidths[NextColumnIndex] = Max(MaxColumnWidths[NextColumnIndex], TypedCommand->Width);
            ++NextColumnIndex;
          } break;

          case type_ui_render_command_new_row:
          {
            NextColumnIndex = 0;
          } break;

          case type_ui_render_command_table_end:
          {
            Assert(CommandIndex == OnePastTableEnd-1);
          } break;

          default: {} break;
      }

      continue;
    }
  }

  {
    u32 NextColumnIndex = 0;

    for (u32 CommandIndex = StartingIndex;
        CommandIndex < OnePastTableEnd;
        ++CommandIndex)
    {
      ui_render_command* Command =  GetCommand(CommandBuffer, CommandIndex);
      switch(Command->Type)
      {
          case type_ui_render_command_column_start:
          {
            Assert(NextColumnIndex < ColumnCount);

            ui_render_command_column_start* TypedCommand = RenderCommandAs(column_start, Command);
            TypedCommand->MaxWidth = MaxColumnWidths[NextColumnIndex];
            ++NextColumnIndex;
          } break;

          case type_ui_render_command_new_row:
          {
            NextColumnIndex = 0;
          } break;

          case type_ui_render_command_table_end:
          {
            Assert(CommandIndex == OnePastTableEnd-1);
          } break;

          default: {} break;
      }

      continue;
    }
  }

  return OnePastTableEnd;
}

link_internal v2
GetNextInlineElementBasis(render_state* RenderState)
{
  v2 Result = GetAbsoluteAt(RenderState->Layout);
  return Result;
}

link_internal void
FlushCommandBuffer(debug_ui_render_group *Group, ui_render_command_buffer *CommandBuffer)
{
  layout DefaultLayout = {};
  render_state RenderState = { .Layout = &DefaultLayout };

  SetWindowZDepths(CommandBuffer);

  u32 NextCommandIndex = 0;
  ui_render_command *Command = GetCommand(CommandBuffer, NextCommandIndex++);
  while (Command)
  {
    switch(Command->Type)
    {
      case type_ui_render_command_window_start:
      {
        Assert(!RenderState.Window);
        ui_render_command_window_start* TypedCommand = RenderCommandAs(window_start, Command);
        RenderState.WindowStartCommandIndex = NextCommandIndex-1;
        PushLayout(&RenderState.Layout, &TypedCommand->Layout);
        RenderState.Window = TypedCommand->Window;
      } break;

      case type_ui_render_command_window_end:
      {
        ui_render_command_window_end* TypedCommand = RenderCommandAs(window_end, Command);
        Assert(TypedCommand->Window == RenderState.Window);
        RenderState.Window = 0;
        PopLayout(&RenderState.Layout);
        Assert(RenderState.Layout == &DefaultLayout);
        RenderState.WindowStartCommandIndex = 0;
      } break;

      case type_ui_render_command_table_start:
      {
        u32 OnePastTableEnd = PreprocessTable(CommandBuffer, NextCommandIndex-1);
        if (OnePastTableEnd)
        {
          ui_render_command_table_start* TypedCommand = RenderCommandAs(table_start, Command);
          r32 BasisX = RenderState.Layout->Basis.x;
          r32 BasisY = GetAbsoluteDrawBoundsMax(RenderState.Layout).y;
          TypedCommand->Layout.Basis = V2(BasisX, BasisY);
          PushLayout(&RenderState.Layout, &TypedCommand->Layout);

          if (TypedCommand->Position == Position_RightOf)
          {
            ui_render_command_table_start* RelativeTable = GetCommandAs(table_start, CommandBuffer, TypedCommand->RelativeTo.Index);
            TypedCommand->Layout.Basis = V2(GetAbsoluteDrawBoundsMax(&RelativeTable->Layout).x, RelativeTable->Layout.Basis.y);
          }
        }
        else
        {
          Error("No type_ui_render_command_table_end detected, aborting render.");
          NextCommandIndex = CommandBuffer->CommandCount;
        }
      } break;

      case type_ui_render_command_table_end:
      {
        if (RenderState.Layout->At.x > 0.0f) { NewLine(RenderState.Layout); }
#if DEBUG_UI_OUTLINE_TABLES
        BufferBorder(Group, GetAbsoluteDrawBounds(RenderState.Layout), V3(0,0,1), 0.9f, DISABLE_CLIPPING);
#endif
        PopLayout(&RenderState.Layout);
      } break;

      case type_ui_render_command_column_start:
      {
        ui_render_command_column_start* TypedCommand = RenderCommandAs(column_start, Command);
        TypedCommand->Layout.Basis = GetNextInlineElementBasis(&RenderState);

        PushLayout(&RenderState.Layout, &TypedCommand->Layout);

        if (TypedCommand->Params & ColumnRenderParam_RightAlign)
        {
          v2 Advance = V2(TypedCommand->MaxWidth - TypedCommand->Width, 0);
          AdvanceLayoutStackBy(Advance, RenderState.Layout);
        }

      } break;

      case type_ui_render_command_column_end:
      {
        PopLayout(&RenderState.Layout);
      } break;

      case type_ui_render_command_text:
      {
        ui_render_command_text* TypedCommand = RenderCommandAs(text, Command);
        TypedCommand->Layout.Basis = GetNextInlineElementBasis(&RenderState);

        PushLayout(&RenderState.Layout, &TypedCommand->Layout);
        BufferValue(TypedCommand->String, Group, &RenderState, &TypedCommand->Style);
        PopLayout(&RenderState.Layout);
      } break;

      case type_ui_render_command_textured_quad:
      {
        ui_render_command_textured_quad* TypedCommand = RenderCommandAs(textured_quad, Command);
        /* PushLayout(&RenderState.Layout, &TypedCommand->Layout); */
        ProcessTexturedQuadPush(Group, TypedCommand, &RenderState);
        /* PopLayout(&RenderState.Layout); */
      } break;

      case type_ui_render_command_untextured_quad_at:
      {
        ui_render_command_untextured_quad_at* TypedCommand = RenderCommandAs(untextured_quad_at, Command);
        ProcessUntexturedQuadAtPush(Group, TypedCommand, &RenderState);
      } break;

      case type_ui_render_command_untextured_quad:
      {
        ui_render_command_untextured_quad* TypedCommand = RenderCommandAs(untextured_quad, Command);
        TypedCommand->Layout.Basis = GetAbsoluteAt(RenderState.Layout);
        PushLayout(&RenderState.Layout, &TypedCommand->Layout);
        ProcessUntexturedQuadPush(Group, TypedCommand, &RenderState);
        PopLayout(&RenderState.Layout);
      } break;

      case type_ui_render_command_new_row:
      {
        NewLine(RenderState.Layout);
      } break;

      case type_ui_render_command_text_at:
      {
        ui_render_command_text_at* TextCommand = RenderCommandAs(text_at, Command);
        BufferTextAt(Group, TextCommand->At, TextCommand->Text, Global_Font.Size, V3(1), 1.0f, TextCommand->MaxClip);
      } break;

      case type_ui_render_command_button_start:
      {
        ui_render_command_button_start* TypedCommand = RenderCommandAs(button_start, Command);
        ProcessButtonStart(Group, &RenderState, TypedCommand->ID);
      } break;

      case type_ui_render_command_button_end:
      {
        u32 ButtonStartIndex = FindPreviousButtonStart(CommandBuffer, NextCommandIndex-1);
        rect2 AbsDrawBounds = FindAbsoluteDrawBoundsBetween(CommandBuffer, ButtonStartIndex, NextCommandIndex);
        ui_render_command_button_start* ButtonStart = RenderCommandAs(button_start, CommandBuffer->Commands+ButtonStartIndex);
        ProcessButtonEnd(Group, ButtonStart->ID, &RenderState, AbsDrawBounds, &ButtonStart->Style);
      } break;

      case type_ui_render_command_border:
      {
        ui_render_command_border* Border = RenderCommandAs(border, Command);
        BufferBorder(Group, Border->Bounds, Border->Color, GetZ(zDepth_Border, RenderState.Window), DISABLE_CLIPPING);
      } break;

      InvalidDefaultCase;
    }

    Command = GetCommand(CommandBuffer, NextCommandIndex++);
  }

  CommandBuffer->CommandCount = 0;

  return;
}

link_internal v2
BasisBelow(window_layout* Window, v2 WindowSpacing = V2(0, 50))
{
  v2 Result = V2(Window->Basis.x, GetAbsoluteMaxClip(Window).y) + WindowSpacing;
  return Result;
}

link_internal v2
BasisRightOf(window_layout* Window, v2 WindowSpacing = V2(50, 0))
{
  v2 Result = V2(GetAbsoluteMaxClip(Window).x, Window->Basis.y) + WindowSpacing;
  return Result;
}

#endif
