// callsite
// examples/ui_test/game.cpp:32:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:695:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, test_enum *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x3B491E58);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle test_enum", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Foo"), UiId(Window, "enum TestEnum_Foo", Element, ThisHash), Params))
    {
      Result = True;
            *Element = TestEnum_Foo;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Bar"), UiId(Window, "enum TestEnum_Bar", Element, ThisHash), Params))
    {
      Result = True;
            *Element = TestEnum_Bar;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Baz"), UiId(Window, "enum TestEnum_Baz", Element, ThisHash), Params))
    {
      Result = True;
            *Element = TestEnum_Baz;


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);

  }
  else
  {
    PushNewRow(Ui);
  }
  return Result;
}


