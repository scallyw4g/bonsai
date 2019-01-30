
struct interactable
{
  umm ID;
  v2 MinP;
  v2 MaxP;
};

// TODO(Jesse): Can this be consolidated with the layout struct?
struct table_column
{
  u32 Max;
};

struct clip_rect
{
  v2 Min;
  v2 Max;
};

struct layout
{
  v2 At;
  v2 Basis;
  clip_rect Clip;
};

#define MAX_TABLE_COLUMNS 4
struct table
{
  table_column Columns[MAX_TABLE_COLUMNS];
  u32 ColumnIndex;
};

struct window_layout
{
  const char* Title;
  layout Layout;
  table Table;
  v2 MaxClip;
};


window_layout
WindowLayout(v2 Basis = V2(150, 150), v2 MaxClip = V2(150, 150))
{
  window_layout Window = {};
  Window.Layout.Basis = Basis;
  Window.MaxClip = MaxClip;

  return Window;
}

interactable
Interactable(v2 MinP, v2 MaxP, umm ID)
{
  interactable Result = {};
  Result.MinP = MinP;
  Result.MaxP = MaxP;
  Result.ID = ID;

  return Result;
}

inline interactable
StartInteractable(layout* Layout, umm ID)
{
  v2 StartingAt = Layout->At + Layout->Basis;
  interactable Result = Interactable(StartingAt, StartingAt, ID);
  return Result;
}

rect2
Rect2(interactable *Interaction)
{
  rect2 Result = RectMinMax(Interaction->MinP, Interaction->MaxP);
  return Result;
}