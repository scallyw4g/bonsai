

bonsai_function b32
Hover(debug_ui_render_group* Group, interactable_handle *Interaction)
{
  b32 Result = Group->HoverInteractionId == Interaction->Id;
  return Result;
}

bonsai_function b32
Clicked(debug_ui_render_group* Group, interactable_handle *Interaction)
{
  b32 Result = Group->ClickedInteractionId == Interaction->Id;
  return Result;
}

bonsai_function b32
Pressed(debug_ui_render_group* Group, interactable_handle *Interaction)
{
  b32 Result = Group->PressedInteractionId == Interaction->Id;
  return Result;
}

bonsai_function b32
Hover(debug_ui_render_group* Group, interactable *Interaction)
{
  v2 MouseP = *Group->MouseP;

  b32 HotWindowMatchesInteractionWindow = (Group->HighestWindow == Interaction->Window);
  b32 Result = HotWindowMatchesInteractionWindow && IsInsideRect(Rect2(Interaction), MouseP);

  if (Interaction->Window)
  {
    Result = Result && IsInsideRect( GetBounds(Interaction->Window), MouseP );
  }

  return Result;
}

bonsai_function b32
Clicked(debug_ui_render_group* Group, interactable *Interaction)
{
  b32 MouseButtonClicked = Group->Input->LMB.Clicked || Group->Input->RMB.Clicked;

  b32 Result = False;
  if ( !Group->PressedInteractionId &&
       MouseButtonClicked && Hover(Group, Interaction))
  {
    Group->PressedInteractionId = Interaction->ID;
    Result = True;
  }

  return Result;
}

bonsai_function b32
Clicked(debug_ui_render_group* Group, interactable Interaction)
{
  b32 Result = Clicked(Group, &Interaction);
  return Result;
}

bonsai_function b32
Pressed(debug_ui_render_group* Group, interactable *Interaction)
{
  umm CurrentInteraction = Group->PressedInteractionId;
  b32 CurrentInteractionMatches = CurrentInteraction == Interaction->ID;
  b32 MouseDepressed = Group->Input->LMB.Pressed || Group->Input->RMB.Pressed;

  b32 Result = False;
  if (MouseDepressed && CurrentInteractionMatches)
  {
    Result = True;
  }
  else if (MouseDepressed && !CurrentInteraction && Hover(Group, Interaction))
  {
    Group->PressedInteractionId = Interaction->ID;
    Result = True;
  }

  return Result;
}
