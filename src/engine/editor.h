struct level_editor
{
  /* level_edit_mode Mode; */

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

enum world_edit_mode
{
  WorldEditMode_Select          = (1 << 0),
  WorldEditMode_FillSelection   = (1 << 1),
  WorldEditMode_DeleteSelection = (1 << 2),

  WorldEditMode_AddSingle       = (1 << 3),
  WorldEditMode_RemoveSingle    = (1 << 4),
  WorldEditMode_PaintSingle     = (1 << 5),
};

poof(string_and_value_tables(world_edit_mode))
#include <generated/string_and_value_tables_world_edit_mode.h>
