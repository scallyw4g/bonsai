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

link_internal void
DebugValue_(renderer_2d *Ui, r32 *Value, const char* Name)
{
  PushTableStart(Ui);
    PushColumn(Ui, CS(Name));
    PushColumn(Ui, CS(*Value));
    PushNewRow(Ui);
  PushTableEnd(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, bool *Value, const char* Name)
{
  PushTableStart(Ui);

    if (Button(Ui, CS(Name), (umm)Value ^ (umm)"toggle" )) { *Value = !(*Value); }
    counted_string Display = CSz("False");
    if (*Value) { Display = CSz("True"); }

    Text(Ui, Display);

    PushNewRow(Ui);
  PushTableEnd(Ui);
}

link_internal void
DebugValue_(renderer_2d *Ui, u32 *Value, const char* Name)
{
  PushTableStart(Ui);
    PushColumn(Ui, CS(Name));

    if (Button(Ui, CSz("-"), (umm)Value ^ (umm)"decrement" )) { *Value = *Value - 1; }
    PushColumn(Ui, CS(*Value));
    if (Button(Ui, CSz("+"), (umm)Value ^ (umm)"increment" )) { *Value = *Value + 1; }

    PushNewRow(Ui);
  PushTableEnd(Ui);
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
DoEngineDebugMenu(renderer_2d *Ui, render_settings *Settings, engine_debug *EngineDebug)
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
    local_persist window_layout RenderSettingsWindow = WindowLayout("RenderSettings", DefaultWindowBasis(*Ui->ScreenDim));
    PushWindowStart(Ui, &RenderSettingsWindow);

    DebugValue(Ui, &Settings->UseSsao);

    PushWindowEnd(Ui, &RenderSettingsWindow);
  }
#endif

}
