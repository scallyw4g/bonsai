// src/engine/editor.cpp:334:0

link_internal counted_string
ToStringPrefixless(ui_brush_layer_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case UiBrushLayerAction_NoAction: { Result = CSz("NoAction"); } break;
    case UiBrushLayerAction_MoveUp: { Result = CSz("MoveUp"); } break;
    case UiBrushLayerAction_MoveDown: { Result = CSz("MoveDown"); } break;
    case UiBrushLayerAction_Duplicate: { Result = CSz("Duplicate"); } break;
    case UiBrushLayerAction_Delete: { Result = CSz("Delete"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_brush_layer_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case UiBrushLayerAction_NoAction: { Result = CSz("UiBrushLayerAction_NoAction"); } break;
    case UiBrushLayerAction_MoveUp: { Result = CSz("UiBrushLayerAction_MoveUp"); } break;
    case UiBrushLayerAction_MoveDown: { Result = CSz("UiBrushLayerAction_MoveDown"); } break;
    case UiBrushLayerAction_Duplicate: { Result = CSz("UiBrushLayerAction_Duplicate"); } break;
    case UiBrushLayerAction_Delete: { Result = CSz("UiBrushLayerAction_Delete"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal ui_brush_layer_actions
UiBrushLayerActions(counted_string S)
{
  ui_brush_layer_actions Result = {};

    if (StringsMatch(S, CSz("UiBrushLayerAction_NoAction"))) { return UiBrushLayerAction_NoAction; }
  if (StringsMatch(S, CSz("UiBrushLayerAction_MoveUp"))) { return UiBrushLayerAction_MoveUp; }
  if (StringsMatch(S, CSz("UiBrushLayerAction_MoveDown"))) { return UiBrushLayerAction_MoveDown; }
  if (StringsMatch(S, CSz("UiBrushLayerAction_Duplicate"))) { return UiBrushLayerAction_Duplicate; }
  if (StringsMatch(S, CSz("UiBrushLayerAction_Delete"))) { return UiBrushLayerAction_Delete; }

  return Result;
}


