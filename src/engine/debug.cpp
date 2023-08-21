
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

link_internal void
DoEngineDebugMenu(renderer_2d *Ui, engine_debug *EngineDebug)
{
  local_persist ui_element_toggle_button Buttons[] = {
    {CSz("World Chunks"), False},
    {CSz("Textures"), False},
    {CSz("Baz"), False},
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
    PushWindowEnd(Ui, &WorldChunkWindow);
  }

  if (ToggledOn(&ButtonGroup, CSz("Textures")))
  {
    local_persist window_layout TexturesWindow = WindowLayout("Textures", DefaultWindowBasis(*Ui->ScreenDim));
    PushWindowStart(Ui, &TexturesWindow);

    IterateOver(&EngineDebug->Textures, Texture, TextureIndex)
    {
      PushTexturedQuad(Ui, DebugTextureArraySlice_Viewport, V2(120), zDepth_Text);
    }

    PushWindowEnd(Ui, &TexturesWindow);
  }


}
