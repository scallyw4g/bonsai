// src/engine/editor.h:543:0

link_internal counted_string
ToString(world_edit_tool Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case WorldEditTool_Select: { Result = CSz("WorldEditTool_Select"); } break;
    case WorldEditTool_Eyedropper: { Result = CSz("WorldEditTool_Eyedropper"); } break;
    case WorldEditTool_Brush: { Result = CSz("WorldEditTool_Brush"); } break;
    case WorldEditTool_BlitEntity: { Result = CSz("WorldEditTool_BlitEntity"); } break;
    case WorldEditTool_RecomputeStandingSpots: { Result = CSz("WorldEditTool_RecomputeStandingSpots"); } break;

    
  }
  return Result;
}

link_internal world_edit_tool
WorldEditTool(counted_string S)
{
  world_edit_tool Result = {};

  if (StringsMatch(S, CSz("WorldEditTool_Select"))) { return WorldEditTool_Select; }
  if (StringsMatch(S, CSz("WorldEditTool_Eyedropper"))) { return WorldEditTool_Eyedropper; }
  if (StringsMatch(S, CSz("WorldEditTool_Brush"))) { return WorldEditTool_Brush; }
  if (StringsMatch(S, CSz("WorldEditTool_BlitEntity"))) { return WorldEditTool_BlitEntity; }
  if (StringsMatch(S, CSz("WorldEditTool_RecomputeStandingSpots"))) { return WorldEditTool_RecomputeStandingSpots; }

  return Result;
}


