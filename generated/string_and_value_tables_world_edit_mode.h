// src/engine/editor.h:538:0

link_internal counted_string
ToString(world_edit_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case WorldEditMode_Paint: { Result = CSz("WorldEditMode_Paint"); } break;
    case WorldEditMode_Attach: { Result = CSz("WorldEditMode_Attach"); } break;
    case WorldEditMode_Remove: { Result = CSz("WorldEditMode_Remove"); } break;

    
  }
  return Result;
}

link_internal world_edit_mode
WorldEditMode(counted_string S)
{
  world_edit_mode Result = {};

  if (StringsMatch(S, CSz("WorldEditMode_Paint"))) { return WorldEditMode_Paint; }
  if (StringsMatch(S, CSz("WorldEditMode_Attach"))) { return WorldEditMode_Attach; }
  if (StringsMatch(S, CSz("WorldEditMode_Remove"))) { return WorldEditMode_Remove; }

  return Result;
}


