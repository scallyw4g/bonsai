// src/engine/editor.cpp:2416:0

link_internal counted_string
ToStringPrefixless(layer_toolbar_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case LayerToolbarActions_New: { Result = CSz("New"); } break;
    case LayerToolbarActions_Delete: { Result = CSz("Delete"); } break;
    case LayerToolbarActions_Rename: { Result = CSz("Rename"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(layer_toolbar_actions Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case LayerToolbarActions_New: { Result = CSz("LayerToolbarActions_New"); } break;
    case LayerToolbarActions_Delete: { Result = CSz("LayerToolbarActions_Delete"); } break;
    case LayerToolbarActions_Rename: { Result = CSz("LayerToolbarActions_Rename"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal layer_toolbar_actions
LayerToolbarActions(counted_string S)
{
  layer_toolbar_actions Result = {};

    if (StringsMatch(S, CSz("LayerToolbarActions_New"))) { return LayerToolbarActions_New; }
  if (StringsMatch(S, CSz("LayerToolbarActions_Delete"))) { return LayerToolbarActions_Delete; }
  if (StringsMatch(S, CSz("LayerToolbarActions_Rename"))) { return LayerToolbarActions_Rename; }

  return Result;
}


