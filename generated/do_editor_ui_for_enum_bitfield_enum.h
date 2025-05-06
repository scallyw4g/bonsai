// examples/ui_test/game.cpp:61:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, bitfield_enum *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("Foo"), UiId(Window, "enum BitfieldEnum_Foo", Element), Params))
    {
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
    if (Button(Ui, CSz("Bar"), UiId(Window, "enum BitfieldEnum_Bar", Element), Params))
    {
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
    if (Button(Ui, CSz("Baz"), UiId(Window, "enum BitfieldEnum_Baz", Element), Params))
    {
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
}


