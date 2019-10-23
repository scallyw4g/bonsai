
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

struct layout
{
  v2 At;
  v2 Basis;
  rect2 Clip;
};

#define MAX_TABLE_COLUMNS 4
struct table
{
  layout Layout;

  table_column Columns[MAX_TABLE_COLUMNS];
  u32 ColumnIndex;
};

struct window_layout
{
  const char* Title;
  table Table;
  v2 MaxClip;
};

struct ui_style
{
  v3 Color;
  v3 HoverColor;
  v3 ActiveColor;
  v3 ClickColor;

  v2 Padding;

  b32 IsActive;
};

function ui_style
StandardStyling(v3 StartingColor, v3 HoverMultiplier = V3(1.1f), v3 ClickMultiplier = V3(1.2f))
{
  ui_style Result = {};
  Result.Color = StartingColor;
  Result.HoverColor = StartingColor*HoverMultiplier;
  Result.ClickColor = StartingColor*ClickMultiplier;

  return Result;
}

window_layout
WindowLayout(v2 Basis = V2(150, 150), v2 MaxClip = V2(1500, 800), const char* Title = 0)
{
  window_layout Window = {};
  Window.Table.Layout.Basis = Basis;
  Window.MaxClip = MaxClip;
  Window.Title = Title;

  return Window;
}

interactable
Interactable(rect2 Rect, umm ID)
{
  interactable Result = {};
  Result.MinP = Rect.Min;
  Result.MaxP = Rect.Max;
  Result.ID = ID;

  return Result;
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
