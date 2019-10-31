
// TODO(Jesse): Can this be consolidated with the layout struct?
struct table_column
{
  u32 Max;
};

struct layout
{
  v2 At;
  v2 Basis;
  rect2 DrawBounds;
};

#define MAX_TABLE_COLUMNS 4
struct table
{
  layout Layout;

  table_column Columns[MAX_TABLE_COLUMNS];
  u32 ColumnIndex;
};

// TODO(Jesse): This should probably be something more like:
//
// table *Tables;
// u32 TableCount;
//
// Such that we don't have to pass clipping information around to routines that
// need to do clipping.
//
struct window_layout
{
  const char* Title;
  table Table;
  v2 MaxClip;

  u64 InteractionStackIndex;

  window_layout* NextHotWindow;
};

struct interactable
{
  umm ID;
  v2 MinP;
  v2 MaxP;

  window_layout* Window;
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

struct button_interaction_result
{
  b32 Pressed;
  b32 Clicked;
  b32 Hover;
  v3 Color;
};

function ui_style
StandardStyling(v3 StartingColor, v3 HoverMultiplier = V3(1.3f), v3 ClickMultiplier = V3(1.2f))
{
  ui_style Result = {};
  Result.Color = StartingColor;
  Result.HoverColor = StartingColor*HoverMultiplier;
  Result.ClickColor = StartingColor*ClickMultiplier;

  return Result;
}

window_layout
WindowLayout(const char* Title, v2 Basis = V2(150, 150), v2 MaxClip = V2(1500, 800))
{
  window_layout Window = {};
  Window.Table.Layout.Basis = Basis;
  Window.MaxClip = MaxClip;
  Window.Title = Title;

  return Window;
}

rect2
GetWindowBounds(window_layout *Window)
{
  v2 TopLeft = Window->Table.Layout.Basis;
  v2 BottomRight = Window->Table.Layout.Basis + Window->MaxClip;
  rect2 Result = RectMinMax(TopLeft, BottomRight);
  return Result;
}

interactable
Interactable(v2 MinP, v2 MaxP, umm ID, window_layout *Window)
{
  interactable Result = {};
  Result.MinP = MinP;
  Result.MaxP = MaxP;
  Result.ID = ID;
  Result.Window = Window;

  return Result;
}

interactable
Interactable(rect2 Rect, umm ID, window_layout *Window)
{
  interactable Result = Interactable(Rect.Min, Rect.Max, ID, Window);
  return Result;
}

inline interactable
StartInteractable(layout* Layout, umm ID, window_layout *Window)
{
  v2 StartingAt = Layout->At + Layout->Basis;
  interactable Result = Interactable(StartingAt, StartingAt, ID, Window);
  return Result;
}

rect2
Rect2(s32 Flood)
{
  rect2 Result = RectMinMax(V2(Flood), V2(Flood));
  return Result;
}

rect2
Rect2(interactable Interaction)
{
  rect2 Result = RectMinMax(Interaction.MinP, Interaction.MaxP);
  return Result;
}

rect2
Rect2(interactable *Interaction)
{
  rect2 Result = Rect2(*Interaction);
  return Result;
}
