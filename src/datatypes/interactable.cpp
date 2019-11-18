

function b32
Hover(debug_ui_render_group* Group, interactable_handle *Interaction)
{
  b32 Result = Group->HoverInteractionId == Interaction->Id;
  return Result;
}

function b32
Clicked(debug_ui_render_group* Group, interactable_handle *Interaction)
{
  b32 Result = Group->ClickedInteractionId == Interaction->Id;
  return Result;
}

function b32
Pressed(debug_ui_render_group* Group, interactable_handle *Interaction)
{
  b32 Result = Group->PressedInteractionId == Interaction->Id;
  return Result;
}


#if 0
function b32
Hover(debug_ui_render_group* Group, rect2 Bounds)
{
  b32 Result = IsInsideRect(Bounds, *Group->MouseP);
  return Result;
}
#endif

rect2 GetClippingBounds(window_layout* Window);

function b32
Hover(debug_ui_render_group* Group, interactable *Interaction)
{
  v2 MouseP = *Group->MouseP;

  b32 HotWindowMatchesInteractionWindow = (Group->HighestWindow == Interaction->Window);
  b32 Result = HotWindowMatchesInteractionWindow && IsInsideRect(Rect2(Interaction), MouseP);

  if (Interaction->Window)
  {
    Result = Result && IsInsideRect( GetClippingBounds(Interaction->Window), MouseP );
  }

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
  if ( Clicked &&
       !CurrentInteraction &&
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

function void BufferBorder(debug_ui_render_group *Group, interactable* PickerListInteraction, v3 Color, r32 Z, v2 MaxClip);

function void
EndInteractable(layout* Layout, interactable *Interaction)
{
  Interaction->MaxP = GetAbsoluteDrawBoundsMax(Interaction->Window, Layout);
  BufferBorder(&GetDebugState()->UiGroup, Interaction, V3(0,1,0), 1.0f, DISABLE_CLIPPING);
  return;
}
