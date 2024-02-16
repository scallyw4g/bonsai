// src/engine/editor.h:381:0

link_internal counted_string
ToString(world_edit_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case WorldEditMode_Select: { Result = CSz("WorldEditMode_Select"); } break;
    case WorldEditMode_FillSelection: { Result = CSz("WorldEditMode_FillSelection"); } break;
    case WorldEditMode_PaintSelection: { Result = CSz("WorldEditMode_PaintSelection"); } break;
    case WorldEditMode_DeleteSelection: { Result = CSz("WorldEditMode_DeleteSelection"); } break;
    case WorldEditMode_Eyedropper: { Result = CSz("WorldEditMode_Eyedropper"); } break;
    case WorldEditMode_AddSingle: { Result = CSz("WorldEditMode_AddSingle"); } break;
    case WorldEditMode_RemoveSingle: { Result = CSz("WorldEditMode_RemoveSingle"); } break;
    case WorldEditMode_PaintSingle: { Result = CSz("WorldEditMode_PaintSingle"); } break;
    case WorldEditMode_AssetBrush: { Result = CSz("WorldEditMode_AssetBrush"); } break;
    case WorldEditMode_EntityBrush: { Result = CSz("WorldEditMode_EntityBrush"); } break;
    case WorldEditMode_BlitEntity: { Result = CSz("WorldEditMode_BlitEntity"); } break;
    case WorldEditMode_RecomputeStandingSpots: { Result = CSz("WorldEditMode_RecomputeStandingSpots"); } break;

    
  }
  return Result;
}

link_internal world_edit_mode
WorldEditMode(counted_string S)
{
  world_edit_mode Result = {};

  if (StringsMatch(S, CSz("WorldEditMode_Select"))) { return WorldEditMode_Select; }
  if (StringsMatch(S, CSz("WorldEditMode_FillSelection"))) { return WorldEditMode_FillSelection; }
  if (StringsMatch(S, CSz("WorldEditMode_PaintSelection"))) { return WorldEditMode_PaintSelection; }
  if (StringsMatch(S, CSz("WorldEditMode_DeleteSelection"))) { return WorldEditMode_DeleteSelection; }
  if (StringsMatch(S, CSz("WorldEditMode_Eyedropper"))) { return WorldEditMode_Eyedropper; }
  if (StringsMatch(S, CSz("WorldEditMode_AddSingle"))) { return WorldEditMode_AddSingle; }
  if (StringsMatch(S, CSz("WorldEditMode_RemoveSingle"))) { return WorldEditMode_RemoveSingle; }
  if (StringsMatch(S, CSz("WorldEditMode_PaintSingle"))) { return WorldEditMode_PaintSingle; }
  if (StringsMatch(S, CSz("WorldEditMode_AssetBrush"))) { return WorldEditMode_AssetBrush; }
  if (StringsMatch(S, CSz("WorldEditMode_EntityBrush"))) { return WorldEditMode_EntityBrush; }
  if (StringsMatch(S, CSz("WorldEditMode_BlitEntity"))) { return WorldEditMode_BlitEntity; }
  if (StringsMatch(S, CSz("WorldEditMode_RecomputeStandingSpots"))) { return WorldEditMode_RecomputeStandingSpots; }

  return Result;
}


