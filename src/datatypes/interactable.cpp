inline b32
Hover(debug_ui_render_group* Group, interactable *Interaction)
{
  b32 HotWindowMatchesInteractionWindow = (Group->HighestWindow == Interaction->Window);
  b32 Result = HotWindowMatchesInteractionWindow && IsInsideRect( Rect2(Interaction), *Group->MouseP);
  return Result;
}

inline b32
Pressed(debug_ui_render_group* Group, interactable *Interaction)
{
  umm CurrentInteraction = Group->PressedInteraction.ID;
  b32 CurrentInteractionMatches = CurrentInteraction == Interaction->ID;
  b32 Clicked = Group->Input->LMB.IsDown || Group->Input->RMB.IsDown;

  b32 Result = False;
  if (Clicked && CurrentInteractionMatches)
  {
    Result = True;
  }
  else if (Clicked && !CurrentInteraction && Hover(Group, Interaction))
  {
    Group->PressedInteraction = *Interaction;
    Result = True;
  }

  return Result;
}

inline b32
Clicked(debug_ui_render_group* Group, interactable *Interaction)
{
  b32 Result = Hover(Group, Interaction) &&
               (Group->Input->LMB.WasPressed || Group->Input->RMB.WasPressed);

  return Result;
}

inline b32
Clicked(debug_ui_render_group* Group, interactable Interaction)
{
  b32 Result = Clicked(Group, &Interaction);
  return Result;
}

inline void
EndInteractable(window_layout* Window, interactable *Interaction)
{
  Interaction->MaxP = Window->Table.Layout.Basis + Window->Table.Layout.DrawBounds.Max;
  return;
}
