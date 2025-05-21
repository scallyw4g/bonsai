// src/engine/editor.cpp:372:0

link_internal counted_string
ToStringPrefixless(ui_layer_edit_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case UiLayerEditAction_SetBrush: { Result = CSz("SetBrush"); } break;
    case UiLayerEditAction_Duplicate: { Result = CSz("Duplicate"); } break;
    case UiLayerEditAction_Delete: { Result = CSz("Delete"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_layer_edit_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case UiLayerEditAction_SetBrush: { Result = CSz("UiLayerEditAction_SetBrush"); } break;
    case UiLayerEditAction_Duplicate: { Result = CSz("UiLayerEditAction_Duplicate"); } break;
    case UiLayerEditAction_Delete: { Result = CSz("UiLayerEditAction_Delete"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal ui_layer_edit_actions
UiLayerEditActions(counted_string S)
{
  ui_layer_edit_actions Result = {};

    if (StringsMatch(S, CSz("UiLayerEditAction_SetBrush"))) { return UiLayerEditAction_SetBrush; }
  if (StringsMatch(S, CSz("UiLayerEditAction_Duplicate"))) { return UiLayerEditAction_Duplicate; }
  if (StringsMatch(S, CSz("UiLayerEditAction_Delete"))) { return UiLayerEditAction_Delete; }

  return Result;
}


