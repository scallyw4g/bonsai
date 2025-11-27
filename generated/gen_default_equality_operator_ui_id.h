// callsite
// external/bonsai_stdlib/src/ui/interactable.h:28:0

// def (gen_default_equality_operator)
// external/bonsai_stdlib/src/poof_functions.h:96:0
link_internal b32
operator==( ui_id E1, ui_id E2 )
{
  b32 Reuslt =  E1.WindowBits == E2.WindowBits && E1.InteractionBits == E2.InteractionBits && E1.ElementBits == E2.ElementBits && E1.HashBits == E2.HashBits ;
  return Reuslt;
}

link_internal b32
operator!=( ui_id E1, ui_id E2 )
{
  b32 Reuslt = !(E1 == E2);
  return Reuslt;
}

