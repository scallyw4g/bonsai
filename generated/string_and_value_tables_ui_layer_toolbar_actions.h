// src/engine/editor.cpp:362:0

link_internal counted_string
ToStringPrefixless(ui_layer_toolbar_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case LayerToolbarActions_NoAction: { Result = CSz("NoAction"); } break;
    case LayerToolbarActions_Rename: { Result = CSz("Rename"); } break;
    case LayerToolbarActions_Duplicate: { Result = CSz("Duplicate"); } break;
    case LayerToolbarActions_Delete: { Result = CSz("Delete"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_layer_toolbar_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case LayerToolbarActions_NoAction: { Result = CSz("LayerToolbarActions_NoAction"); } break;
    case LayerToolbarActions_Rename: { Result = CSz("LayerToolbarActions_Rename"); } break;
    case LayerToolbarActions_Duplicate: { Result = CSz("LayerToolbarActions_Duplicate"); } break;
    case LayerToolbarActions_Delete: { Result = CSz("LayerToolbarActions_Delete"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal ui_layer_toolbar_actions
UiLayerToolbarActions(counted_string S)
{
  ui_layer_toolbar_actions Result = {};

    if (StringsMatch(S, CSz("LayerToolbarActions_NoAction"))) { return LayerToolbarActions_NoAction; }
  if (StringsMatch(S, CSz("LayerToolbarActions_Rename"))) { return LayerToolbarActions_Rename; }
  if (StringsMatch(S, CSz("LayerToolbarActions_Duplicate"))) { return LayerToolbarActions_Duplicate; }
  if (StringsMatch(S, CSz("LayerToolbarActions_Delete"))) { return LayerToolbarActions_Delete; }

  return Result;
}


