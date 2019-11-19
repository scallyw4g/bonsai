
struct layout
{
  v2 At;
  rect2 DrawBounds;
};

struct window_layout
{
  const char* Title;

  v2 Basis;
  v2 MaxClip;

  u64 InteractionStackIndex;

  window_layout* NextHotWindow;
};

struct interactable_handle
{
  umm Id;
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
  v3 PressedColor;
  v3 ClickedColor;

  v3 ActiveColor;

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
  Result.ClickedColor = StartingColor*ClickMultiplier;

  return Result;
}

function window_layout
WindowLayout(const char* Title, v2 Basis = V2(150, 150), v2 MaxClip = V2(1500, 800))
{
  window_layout Window = {};
  Window.Basis = Basis;
  Window.MaxClip = MaxClip;
  Window.Title = Title;

  return Window;
}

function rect2
GetWindowBounds(window_layout *Window)
{
  v2 TopLeft = Window->Basis;
  v2 BottomRight = Window->Basis + Window->MaxClip;
  rect2 Result = RectMinMax(TopLeft, BottomRight);
  return Result;
}

function interactable
Interactable(v2 MinP, v2 MaxP, umm ID, window_layout *Window)
{
  interactable Result = {};
  Result.MinP = MinP;
  Result.MaxP = MaxP;
  Result.ID = ID;
  Result.Window = Window;

  return Result;
}

function interactable
Interactable(rect2 Rect, umm ID, window_layout *Window)
{
  interactable Result = Interactable(Rect.Min, Rect.Max, ID, Window);
  return Result;
}

v2
GetAbsoluteAt(window_layout* Window, layout* Layout);

function interactable
StartInteractable(layout* Layout, umm ID, window_layout *Window)
{
  v2 StartingAt = GetAbsoluteAt(Window, Layout);
  interactable Result = Interactable(StartingAt, StartingAt, ID, Window);
  return Result;
}

function rect2
Rect2(s32 Flood)
{
  rect2 Result = RectMinMax(V2(Flood), V2(Flood));
  return Result;
}

function rect2
Rect2(interactable Interaction)
{
  rect2 Result = RectMinMax(Interaction.MinP, Interaction.MaxP);
  return Result;
}

function rect2
Rect2(interactable *Interaction)
{
  rect2 Result = Rect2(*Interaction);
  return Result;
}

function ui_style
UiStyleFromLightestColor(v3 Color, v2 Padding = V2(10))
{
  ui_style Style  = {
    .Color        = Color*0.8f,
    .HoverColor   = Color*0.65f,
    .PressedColor = Color*0.8f,
    .ClickedColor = Color,
    .ActiveColor  = Color,
    .Padding      = Padding,
    .IsActive     = False,
  };

  return Style;
}

