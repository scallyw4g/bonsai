link_internal b32
operator==( ui_id E1, ui_id E2 )
{
  b32 Reuslt = E1._Reserved == E2._Reserved && E1.WindowBits == E2.WindowBits && E1.InteractionBits == E2.InteractionBits && E1.ElementBits == E2.ElementBits ;
  return Reuslt;
}

link_internal b32
operator!=( ui_id E1, ui_id E2 )
{
  b32 Reuslt = !(E1 == E2);
  return Reuslt;
}

