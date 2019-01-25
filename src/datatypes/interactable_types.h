
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
struct table_layout
{
  layout Layout;

  table_column Columns[MAX_TABLE_COLUMNS];
  u32 ColumnIndex;
};

struct window_layout
{
  table_layout Table;

  v2 MaxClip;
};


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
StartInteractable(table_layout* Table, umm ID)
{
  v2 StartingAt = Table->Layout.At + Table->Layout.Basis;
  interactable Result = Interactable(StartingAt, StartingAt, ID);
  return Result;
}

rect2
Rect2(interactable *Interaction)
{
  rect2 Result = RectMinMax(Interaction->MinP, Interaction->MaxP);
  return Result;
}
