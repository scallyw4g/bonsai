// callsite
// external/bonsai_stdlib/src/ui/ui.h:82:0

// def (is_valid)
// external/bonsai_stdlib/src/poof_functions.h:2747:0
link_internal b32
IsValid(window_layout_flags Value)
{
  b32 Result = False;
  switch (Value)
  {
        case WindowLayoutFlag_None:
    case WindowLayoutFlag_Size_Dynamic:
    case WindowLayoutFlag_StartupSize_InferHeight:
    case WindowLayoutFlag_StartupSize_InferWidth:
    case WindowLayoutFlag_StartupSize_Infer:
    case WindowLayoutFlag_Align_Right:
    case WindowLayoutFlag_Align_Bottom:
    case WindowLayoutFlag_Align_BottomRight:
    case WindowLayoutFlag_Default:
    case WindowLayoutFlag_DeferFree:

    {
      Result = True;
    }
  }
  return Result;
}


