// src/engine/editor.h:548:0

link_internal counted_string
ToString(world_edit_brush_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case WorldEditTool_BrushType_Asset: { Result = CSz("WorldEditTool_BrushType_Asset"); } break;
    case WorldEditTool_BrushType_Entity: { Result = CSz("WorldEditTool_BrushType_Entity"); } break;
    case WorldEditTool_BrushType_Noise: { Result = CSz("WorldEditTool_BrushType_Noise"); } break;

    
  }
  return Result;
}

link_internal world_edit_brush_type
WorldEditBrushType(counted_string S)
{
  world_edit_brush_type Result = {};

  if (StringsMatch(S, CSz("WorldEditTool_BrushType_Asset"))) { return WorldEditTool_BrushType_Asset; }
  if (StringsMatch(S, CSz("WorldEditTool_BrushType_Entity"))) { return WorldEditTool_BrushType_Entity; }
  if (StringsMatch(S, CSz("WorldEditTool_BrushType_Noise"))) { return WorldEditTool_BrushType_Noise; }

  return Result;
}


