// callsite
// examples/ui_test/game.cpp:60:0

// def (do_editor_ui_for_enum)
// src/engine/editor.h:695:0
link_internal b32
DoEditorUi(renderer_2d *Ui, window_layout *Window, bitfield_enum *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  b32 Result = False;
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x23F6F95E);

  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "toggle bitfield_enum", Element, ThisHash);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Foo"), UiId(Window, "enum BitfieldEnum_Foo", Element, ThisHash), Params))
    {
      Result = True;
            if (BitfieldEnum_Foo == bitfield_enum(0))
      {
        *Element = bitfield_enum(0);
      }
      else
      {
        if ((*Element & BitfieldEnum_Foo) == BitfieldEnum_Foo)
        {
          *Element = bitfield_enum(*Element&~BitfieldEnum_Foo);
        }
        else
        {
          *Element = bitfield_enum(*Element|BitfieldEnum_Foo);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Bar"), UiId(Window, "enum BitfieldEnum_Bar", Element, ThisHash), Params))
    {
      Result = True;
            if (BitfieldEnum_Bar == bitfield_enum(0))
      {
        *Element = bitfield_enum(0);
      }
      else
      {
        if ((*Element & BitfieldEnum_Bar) == BitfieldEnum_Bar)
        {
          *Element = bitfield_enum(*Element&~BitfieldEnum_Bar);
        }
        else
        {
          *Element = bitfield_enum(*Element|BitfieldEnum_Bar);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Baz"), UiId(Window, "enum BitfieldEnum_Baz", Element, ThisHash), Params))
    {
      Result = True;
            if (BitfieldEnum_Baz == bitfield_enum(0))
      {
        *Element = bitfield_enum(0);
      }
      else
      {
        if ((*Element & BitfieldEnum_Baz) == BitfieldEnum_Baz)
        {
          *Element = bitfield_enum(*Element&~BitfieldEnum_Baz);
        }
        else
        {
          *Element = bitfield_enum(*Element|BitfieldEnum_Baz);
        }
      }


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


