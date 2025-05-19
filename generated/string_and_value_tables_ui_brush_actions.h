// src/engine/editor.cpp:354:0

link_internal counted_string
ToStringPrefixless(ui_brush_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case UiBrushAction_NoAction: { Result = CSz("NoAction"); } break;
    case UiBrushAction_New: { Result = CSz("New"); } break;
    case UiBrushAction_Save: { Result = CSz("Save"); } break;
    case UiBrushAction_Duplicate: { Result = CSz("Duplicate"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_brush_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case UiBrushAction_NoAction: { Result = CSz("UiBrushAction_NoAction"); } break;
    case UiBrushAction_New: { Result = CSz("UiBrushAction_New"); } break;
    case UiBrushAction_Save: { Result = CSz("UiBrushAction_Save"); } break;
    case UiBrushAction_Duplicate: { Result = CSz("UiBrushAction_Duplicate"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal ui_brush_actions
UiBrushActions(counted_string S)
{
  ui_brush_actions Result = {};

    if (StringsMatch(S, CSz("UiBrushAction_NoAction"))) { return UiBrushAction_NoAction; }
  if (StringsMatch(S, CSz("UiBrushAction_New"))) { return UiBrushAction_New; }
  if (StringsMatch(S, CSz("UiBrushAction_Save"))) { return UiBrushAction_Save; }
  if (StringsMatch(S, CSz("UiBrushAction_Duplicate"))) { return UiBrushAction_Duplicate; }

  return Result;
}


