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
DoEngineDebugMenu(graphics *Graphics, renderer_2d *Ui, engine_debug *EngineDebug)
{

  local_persist ui_element_toggle_button Buttons[] = {
    {CSz("World Chunks"), False},
    {CSz("Textures"), False},
    {CSz("RenderSettings"), False},
  };

  ui_element_toggle_button_group ButtonGroup = {
    .Buttons = Buttons,
    .Count = ArrayCount(Buttons),
  };

  DrawToggleButtonGroup(Ui, &ButtonGroup);

  if (ToggledOn(&ButtonGroup, CSz("World Chunks")))
  {
    local_persist window_layout WorldChunkWindow = WindowLayout("World Chunks", DefaultWindowBasis(*Ui->ScreenDim));
    PushWindowStart(Ui, &WorldChunkWindow);
    PushWindowEnd(Ui,   &WorldChunkWindow);
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

#if 1
  if (ToggledOn(&ButtonGroup, CSz("RenderSettings")))
  {
    v2 WindowDim = {{1200.f, 250.f}};
    local_persist window_layout RenderSettingsWindow = WindowLayout("RenderSettings", DefaultWindowBasis(*Ui->ScreenDim, WindowDim), WindowDim);

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
#endif

}
