inline b32
Hover(debug_ui_render_group* Group, interactable *Interaction)
{
  b32 Result = IsInsideRect(Rect2(Interaction), Group->MouseP);
  return Result;
}

inline b32
Pressed(debug_ui_render_group* Group, interactable *Interaction)
{
  b32 Result = Group->PressedInteraction.ID == Interaction->ID ||
               (Hover(Group, Interaction) && Group->Input->LMB.IsDown);

  if (Result) Group->PressedInteraction = *Interaction;

  return Result;
}

inline b32
Clicked(debug_ui_render_group* Group, interactable *Interaction)
{
  b32 Result = Hover(Group, Interaction) && Group->Input->LMB.WasPressed;
  return Result;
}

inline void
EndInteractable(debug_ui_render_group* Group, table_layout* Table, interactable *Interaction)
{
  Interaction->MaxP = Table->Layout.Basis + Table->Layout.At;

  if (Interaction->MinP.y == Interaction->MaxP.y)
  {
    Interaction->MaxP.y += Group->Font.LineHeight;
  }

  return;
}
