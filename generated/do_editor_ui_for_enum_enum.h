// src/engine/editor.cpp:91:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, visible_region_size *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Name.Count) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToStringPrefixless(*Element);
  ui_id ToggleButtonId = UiId(Window, "enum value.type value.name", Element);
  if (ToggleButton(Ui, ElementName, ElementName, ToggleButtonId, Params))
  {
    PushNewRow(Ui);
        if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1"), UiId(Window, "enum VisibleRegionSize_1", Element), Params))
    {
            if (VisibleRegionSize_1 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_1) == VisibleRegionSize_1)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_1);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_1);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2"), UiId(Window, "enum VisibleRegionSize_2", Element), Params))
    {
            if (VisibleRegionSize_2 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_2) == VisibleRegionSize_2)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_2);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_2);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4"), UiId(Window, "enum VisibleRegionSize_4", Element), Params))
    {
            if (VisibleRegionSize_4 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_4) == VisibleRegionSize_4)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_4);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_4);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8"), UiId(Window, "enum VisibleRegionSize_8", Element), Params))
    {
            if (VisibleRegionSize_8 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_8) == VisibleRegionSize_8)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_8);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_8);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16"), UiId(Window, "enum VisibleRegionSize_16", Element), Params))
    {
            if (VisibleRegionSize_16 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_16) == VisibleRegionSize_16)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_16);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_16);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("32"), UiId(Window, "enum VisibleRegionSize_32", Element), Params))
    {
            if (VisibleRegionSize_32 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_32) == VisibleRegionSize_32)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_32);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_32);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("64"), UiId(Window, "enum VisibleRegionSize_64", Element), Params))
    {
            if (VisibleRegionSize_64 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_64) == VisibleRegionSize_64)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_64);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_64);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("128"), UiId(Window, "enum VisibleRegionSize_128", Element), Params))
    {
            if (VisibleRegionSize_128 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_128) == VisibleRegionSize_128)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_128);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_128);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("256"), UiId(Window, "enum VisibleRegionSize_256", Element), Params))
    {
            if (VisibleRegionSize_256 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_256) == VisibleRegionSize_256)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_256);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_256);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("512"), UiId(Window, "enum VisibleRegionSize_512", Element), Params))
    {
            if (VisibleRegionSize_512 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_512) == VisibleRegionSize_512)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_512);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_512);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("1024"), UiId(Window, "enum VisibleRegionSize_1024", Element), Params))
    {
            if (VisibleRegionSize_1024 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_1024) == VisibleRegionSize_1024)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_1024);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_1024);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("2048"), UiId(Window, "enum VisibleRegionSize_2048", Element), Params))
    {
            if (VisibleRegionSize_2048 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_2048) == VisibleRegionSize_2048)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_2048);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_2048);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("4096"), UiId(Window, "enum VisibleRegionSize_4096", Element), Params))
    {
            if (VisibleRegionSize_4096 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_4096) == VisibleRegionSize_4096)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_4096);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_4096);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("8192"), UiId(Window, "enum VisibleRegionSize_8192", Element), Params))
    {
            if (VisibleRegionSize_8192 == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_8192) == VisibleRegionSize_8192)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_8192);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_8192);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("16k"), UiId(Window, "enum VisibleRegionSize_16k", Element), Params))
    {
            if (VisibleRegionSize_16k == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_16k) == VisibleRegionSize_16k)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_16k);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_16k);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("32k"), UiId(Window, "enum VisibleRegionSize_32k", Element), Params))
    {
            if (VisibleRegionSize_32k == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_32k) == VisibleRegionSize_32k)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_32k);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_32k);
        }
      }


      SetToggleButton(Ui, ToggleButtonId, False);
    }
    PushNewRow(Ui);
    if (Name.Count) { PushColumn(Ui, CSz("|")); } // Skip the first Name column
    if (Button(Ui, CSz("64k"), UiId(Window, "enum VisibleRegionSize_64k", Element), Params))
    {
            if (VisibleRegionSize_64k == visible_region_size(0))
      {
        *Element = visible_region_size(0);
      }
      else
      {
        if ((*Element & VisibleRegionSize_64k) == VisibleRegionSize_64k)
        {
          *Element = visible_region_size(*Element&~VisibleRegionSize_64k);
        }
        else
        {
          *Element = visible_region_size(*Element|VisibleRegionSize_64k);
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

