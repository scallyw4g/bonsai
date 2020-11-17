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

struct button_interaction_result
{
  b32 Pressed;
  b32 Clicked;
  b32 Hover;
};

bonsai_function interactable
Interactable(v2 MinP, v2 MaxP, umm ID, window_layout *Window)
{
  interactable Result = {};
  Result.MinP = MinP;
  Result.MaxP = MaxP;
  Result.ID = ID;
  Result.Window = Window;

  return Result;
}

bonsai_function interactable
Interactable(rect2 Rect, umm ID, window_layout *Window)
{
  interactable Result = Interactable(Rect.Min, Rect.Max, ID, Window);
  return Result;
}

v2 GetAbsoluteAt(layout* Layout);

bonsai_function interactable
StartInteractable(layout* Layout, umm ID, window_layout *Window)
{
  v2 StartingAt = GetAbsoluteAt(Layout);
  interactable Result = Interactable(StartingAt, StartingAt, ID, Window);
  return Result;
}

bonsai_function rect2
Rect2(s32 Flood)
{
  rect2 Result = RectMinMax(V2(Flood), V2(Flood));
  return Result;
}

bonsai_function rect2
Rect2(interactable Interaction)
{
  rect2 Result = RectMinMax(Interaction.MinP, Interaction.MaxP);
  return Result;
}

bonsai_function rect2
Rect2(interactable *Interaction)
{
  rect2 Result = Rect2(*Interaction);
  return Result;
}

