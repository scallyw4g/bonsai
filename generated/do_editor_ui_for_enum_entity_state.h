// src/engine/editor.cpp:98:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_state *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Free"), UiId(Window, "enum EntityState_Free", Element), Params))
    {
      *Element = EntityState_Free;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Spawned"), UiId(Window, "enum EntityState_Spawned", Element), Params))
    {
      *Element = EntityState_Spawned;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Destroyed"), UiId(Window, "enum EntityState_Destroyed", Element), Params))
    {
      *Element = EntityState_Destroyed;

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Reserved"), UiId(Window, "enum EntityState_Reserved", Element), Params))
    {
      *Element = EntityState_Reserved;

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

