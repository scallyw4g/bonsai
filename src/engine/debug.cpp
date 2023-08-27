
link_internal engine_debug *
GetEngineDebug()
{
  return &Global_EngineResources->EngineDebug;
}

ui_debug *
GetUiDebug()
{
  return &Global_EngineResources->EngineDebug.UiDebug;
}


struct ui_element_toggle_button
{
  counted_string Text;
  b32 On;
};

struct ui_element_toggle_button_group
{
  ui_element_toggle_button *Buttons;
  s32 Count;
};

global_variable v4 DefaultToggleButtonPadding = V4(15,0,15,0);

link_internal void
DrawToggleButtonGroup(renderer_2d *Ui, ui_element_toggle_button_group *Group)
{
  RangeIterator(ButtonIndex, Group->Count)
  {
    ui_element_toggle_button *UiButton = Group->Buttons + ButtonIndex;

    ui_style *Style = (UiButton->On) ? &DefaultSelectedStyle : &DefaultStyle;
    if (Button(Ui, UiButton->Text, (umm)UiButton->Text.Start, Style, DefaultToggleButtonPadding))
    {
      UiButton->On = !UiButton->On;
    }
  }
}

link_internal b32
ToggledOn(ui_element_toggle_button_group *Group, cs ButtonName)
{
  b32 Result = False;
  RangeIterator(ButtonIndex, Group->Count)
  {
    auto Button = Group->Buttons + ButtonIndex;
    if (StringsMatch(Button->Text, ButtonName))
    {
      Result = Button->On;
      break;
    }
  }
  return Result;
}

#define DebugValue(Ui, Value) DebugValue_(Ui, Value, STRINGIZE(Value))
#define DebugSlider(Ui, Value, Min, Max) DebugSlider_(Ui, Value, STRINGIZE(Value), Min, Max)

link_internal void
DebugValue_(renderer_2d *Ui, r32 *Value, const char* Name)
{
  PushColumn(Ui, CS(Name));
  PushColumn(Ui, CS(*Value));
  PushNewRow(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, b8 *Value, const char* Name)
{
  if (Button(Ui, CS(Name), (umm)Value + (umm)"toggle" )) { *Value = !(*Value); }

  counted_string Display = *Value ? CSz("True") : CSz("False");
  Text(Ui, Display);

  PushNewRow(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, s32 *Value, const char* Name)
{
  PushColumn(Ui, CS(Name));

  if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
  PushColumn(Ui, CS(*Value));
  if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
  PushNewRow(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, u32 *Value, const char* Name)
{
  PushColumn(Ui, CS(Name));

  if (Button(Ui, CSz("-"), (umm)Value + (umm)"decrement" )) { *Value = *Value - 1; }
  PushColumn(Ui, CS(*Value));
  if (Button(Ui, CSz("+"), (umm)Value + (umm)"increment" )) { *Value = *Value + 1; }
  PushNewRow(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, u64 *Value, const char* Name)
{
  PushTableStart(Ui);
    PushColumn(Ui, CS(Name));
    PushColumn(Ui, CS(*Value));
    PushNewRow(Ui);
  PushTableEnd(Ui);
}


link_internal void
DebugSlider_(renderer_2d *Ui, r32 *Value, const char* Name, r32 Min, r32 Max)
{
  PushTableStart(Ui);
    PushColumn(Ui, CS(Name));
    PushColumn(Ui, CS(*Value));

    auto Range = Max-Min;
    r32 PercFilled = ((*Value)-Min)/Range;

    r32 Width = 125.f;
    interactable_handle BargraphButton = PushButtonStart(Ui, (umm)(umm("DebugSlider") ^ umm(Value)) );
      PushBargraph(Ui, PercFilled, V3(0.75f), V3(0.4f), Width);
    PushButtonEnd(Ui);

    v2 Offset = {};
    if (Pressed(Ui, &BargraphButton, &Offset))
    {
      r32 NewPerc = Clamp01(Offset.x / Width);
      r32 NewValue = (Range*NewPerc) + Min;
      *Value = NewValue;
    }

    PushNewRow(Ui);
  PushTableEnd(Ui);
}


link_internal void
DebugValue_(renderer_2d *Ui, world_position *Value, const char* Name)
{
  Text(Ui, CS(Name));
  PushNewRow(Ui);

  DebugValue(Ui, &Value->x);
  DebugValue(Ui, &Value->y);
  DebugValue(Ui, &Value->z);
}

link_internal void
DebugChunkUi(engine_resources *Engine, cs Name, world_chunk *Value)
{
  UNPACK_ENGINE_RESOURCES(Engine);
  auto Ui = GameUi;

  Text(Ui, Name);
  PushNewRow(Ui);

  world_position WorldP = Value->WorldP;
  DebugValue(Ui, &WorldP);

  if (WorldP != Value->WorldP)
  {
    EngineDebug->PickedChunk = GetWorldChunkFromHashtable(World, WorldP);
  }

  world_chunk *Chunk = EngineDebug->PickedChunk;

  auto Flags = Value->Flags;

  Text(Ui, CSz("Flags : "));
  Text(Ui, CS(Flags));
  Text(Ui, CSz(" ("));

  while (u32 Flag = UnsetLeastSignificantSetBit((u32*)&Flags))
  {
    Text(Ui, ToString((chunk_flag)Flag));
    if (Flags != 0) { Text(Ui, CSz("|")); }
  }
  Text(Ui, CSz(")"));
}




link_internal void
DoEngineDebugMenu(engine_resources *Engine)
{
  UNPACK_ENGINE_RESOURCES(Engine);

  renderer_2d *Ui = GameUi;

  local_persist ui_element_toggle_button Buttons[] = {
    {CSz("Edit"), False},
    {CSz("WorldChunks"), False},
    {CSz("Textures"), False},
    {CSz("RenderSettings"), False},
    {CSz("EngineDebug"), False},
  };

  ui_element_toggle_button_group ButtonGroup = {
    .Buttons = Buttons,
    .Count = ArrayCount(Buttons),
  };

  DrawToggleButtonGroup(Ui, &ButtonGroup);

  if (ToggledOn(&ButtonGroup, CSz("Edit")))
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout Window = WindowLayout("Edit", DefaultWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);
    PushWindowStart(Ui, &Window);

    RangeIterator(ColorIndex, s32(u8_MAX)+1 )
    {
      v3 Color = GetColorData(DefaultPalette, u32(ColorIndex)).rgb;
      ui_style Style = UiStyleFromLightestColor(Color);

      v2 QuadDim = V2(20);
      v4 Padding = V4(5);
      interactable_handle ColorPickerButton = PushButtonStart(Ui, (umm)"ColorPicker" + (umm)ColorIndex);
        PushUntexturedQuad(Ui, {}, QuadDim, zDepth_Text, &Style, Padding );
      PushButtonEnd(Ui);

      if (Hover(Ui, &ColorPickerButton))
      {
        v3 BorderColor = V3(0.5f, 0.5f, 0.1f);
        PushBorder(Ui, RectMinMax(Ui->Hover.MinP+Padding.xy, Ui->Hover.MinP+Padding.xy+QuadDim), BorderColor);
      }

      if ( (ColorIndex+1) % 8 == 0 ) { PushNewRow(Ui); }
    }

    PushWindowEnd(Ui, &Window);

  }

  if (ToggledOn(&ButtonGroup, CSz("WorldChunks")))
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout WorldChunkWindow = WindowLayout("World Chunks", DefaultWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);
    PushWindowStart(Ui, &WorldChunkWindow);

    if (Button(Ui, CSz("Pick"), (umm)"Pick"))
    {
      EngineDebug->PickedChunkState = PickedChunkState_Hover;
    }
    PushNewRow(Ui);

    if (EngineDebug->PickedChunkState == PickedChunkState_Hover)
    {
      picked_world_chunk_static_buffer AllChunksBuffer = {};
      EngineDebug->PickedChunk = GetChunksFromMouseP(Engine, &AllChunksBuffer);

      if (Input->LMB.Clicked) { EngineDebug->PickedChunkState = PickedChunkState_None; }
    }

    if (EngineDebug->PickedChunk)
    {
      DebugChunkUi(Engine, CSz("PickedChunk"), EngineDebug->PickedChunk );
    }

    PushWindowEnd(Ui, &WorldChunkWindow);
  }

#if 0
  if (ToggledOn(&ButtonGroup, CSz("Textures")))
  {
    local_persist window_layout TexturesWindow = WindowLayout("Textures", DefaultWindowBasis(*Ui->ScreenDim));
    PushWindowStart(Ui, &TexturesWindow);

    IterateOver(&EngineDebug->Textures, Texture, TextureIndex)
    {
      PushTexturedQuad(Ui, DebugTextureArraySlice_Font, V2(120), zDepth_Text);
    }

    PushWindowEnd(Ui, &TexturesWindow);
  }
#endif

  if (ToggledOn(&ButtonGroup, CSz("RenderSettings")))
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout RenderSettingsWindow = WindowLayout("Render Settings", DefaultWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &RenderSettingsWindow);

      PushTableStart(Ui);
        /* DebugValue(Ui, &Ui->Pressed.ID); */

        DebugValue(Ui, (b8*)&Settings->UseSsao);
        DebugValue(Ui, (b8*)&Settings->UseShadowMapping);
        DebugValue(Ui, (b8*)&Settings->UseLightingBloom);

        // TODO(Jesse): Make a slider for time of day

        DebugSlider(Ui, &Graphics->Exposure, 0.0f, 5.f);

      PushTableEnd(Ui);

    PushWindowEnd(Ui, &RenderSettingsWindow);
  }

  if (ToggledOn(&ButtonGroup, CSz("EngineDebug")))
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout Window = WindowLayout("Engine Debug", DefaultWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);

    render_settings *Settings = &Graphics->Settings;
    PushWindowStart(Ui, &Window);
      PushTableStart(Ui);

        DebugValue(Ui, &EngineDebug->DrawEntityCollisionVolumes);
        DebugValue(Ui, &EngineDebug->DrawWorldAxies);

        DebugValue(Ui, &EngineDebug->UiDebug.OutlineUiValues);
        DebugValue(Ui, &EngineDebug->UiDebug.OutlineUiButtons);
        DebugValue(Ui, &EngineDebug->UiDebug.OutlineUiTables);

      PushTableEnd(Ui);
    PushWindowEnd(Ui, &Window);
  }
}
