
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

  u32 SelectionClicks;
  v3 SelectionRegion[2];

  // Recorded when shift-clicking on the selection to manipulate it
  face_index SelectionShiftClickedFace;
  v3 SelectionShiftClickedP[2];
};

struct maybe_v3
{
  maybe_tag Tag;
  v3 V3;
};


enum selection_mode
{
  SelectionMode_Noop,

  SelectionMode_Resize,
  SelectionMode_Translate,
};
