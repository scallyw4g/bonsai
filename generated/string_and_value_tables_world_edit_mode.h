// src/engine/editor.h:691:0

link_internal counted_string
ToString(world_edit_mode Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case WorldEdit_Mode_Attach: { Result = CSz("WorldEdit_Mode_Attach"); } break;
    case WorldEdit_Mode_Remove: { Result = CSz("WorldEdit_Mode_Remove"); } break;
    case WorldEdit_Mode_Paint: { Result = CSz("WorldEdit_Mode_Paint"); } break;

    
  }
  return Result;
}

link_internal world_edit_mode
WorldEditMode(counted_string S)
{
  world_edit_mode Result = {};

  if (StringsMatch(S, CSz("WorldEdit_Mode_Attach"))) { return WorldEdit_Mode_Attach; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Remove"))) { return WorldEdit_Mode_Remove; }
  if (StringsMatch(S, CSz("WorldEdit_Mode_Paint"))) { return WorldEdit_Mode_Paint; }

  return Result;
}


