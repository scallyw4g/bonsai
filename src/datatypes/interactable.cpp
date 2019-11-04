function b32
Hover(debug_ui_render_group* Group, rect2 Bounds)
{
  b32 Result = IsInsideRect(Bounds, *Group->MouseP);
  return Result;
}

function b32
Hover(debug_ui_render_group* Group, interactable *Interaction)
{
  b32 HotWindowMatchesInteractionWindow = (Group->HighestWindow == Interaction->Window);
  b32 Result = HotWindowMatchesInteractionWindow && Hover( Group, Rect2(Interaction) );
  return Result;
}

function b32
IsCurrentInteraction(debug_ui_render_group* Group, interactable *Interaction)
{
  umm CurrentInteraction = Group->PressedInteraction.ID;
  b32 CurrentInteractionMatches = CurrentInteraction == Interaction->ID;

  return CurrentInteractionMatches;
}

function b32
Pressed(debug_ui_render_group* Group, interactable *Interaction)
{
  umm CurrentInteraction = Group->PressedInteraction.ID;
  b32 CurrentInteractionMatches = IsCurrentInteraction(Group, Interaction);
  b32 MouseDepressed = Group->Input->LMB.Pressed || Group->Input->RMB.Pressed;

  b32 Result = False;
  if (MouseDepressed && CurrentInteractionMatches)
  {
    Result = True;
  }
  else if (MouseDepressed && !CurrentInteraction && Hover(Group, Interaction))
  {
    Group->PressedInteraction = *Interaction;
    Result = True;
  }

  return Result;
}

function b32
Clicked(debug_ui_render_group* Group, interactable *Interaction)
{
  umm CurrentInteraction = Group->PressedInteraction.ID;
  b32 Clicked = Group->Input->LMB.Clicked || Group->Input->RMB.Clicked;

  b32 Result = False;
  if (Clicked &&
      CurrentInteraction == 0 &&
      Hover(Group, Interaction))
  {
    Group->PressedInteraction = *Interaction;
    Result = True;
  }

  return Result;
}

function b32
Clicked(debug_ui_render_group* Group, interactable Interaction)
{
  b32 Result = Clicked(Group, &Interaction);
  return Result;
}

function void
EndInteractable(layout* Layout, interactable *Interaction)
{
  Interaction->MaxP = GetAbsoluteDrawBoundsMax(Layout);
  return;
}

function void
EndInteractable(table* Table, interactable *Interaction)
{
  EndInteractable(&Table->Layout, Interaction);
  return;
}

function void
EndInteractable(window_layout* Window, interactable *Interaction)
{
  EndInteractable(&Window->Table.Layout, Interaction);
  return;
}
