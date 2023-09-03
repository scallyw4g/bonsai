
enum level_edit_mode
{
  LevelEditMode_None,

  LevelEditMode_Select,
  LevelEditMode_Add,
  LevelEditMode_Remove,
  LevelEditMode_Paint,
};

struct level_editor
{
  level_edit_mode Mode;

  interactable SelectedColorSquare;
  s32 SelectedColorIndex;

  u32 SelectPendingClicks;
  picked_voxel SelectionRegion[2];

  face_index SelectionClickedFace;
  v3 SelectionClickedP;
};

struct maybe_v3
{
  maybe_tag Tag;
  v3 V3;
};

