// callsite
// external/bonsai_stdlib/src/ui/ui.h:108:0

// def (is_valid)
// external/bonsai_stdlib/src/poof_functions.h:2872:0
link_internal b32
IsValid(window_layout_flags Value)
{
  u32 MaskBits =   WindowLayoutFlag_None  |  WindowLayoutFlag_Size_Dynamic  |  WindowLayoutFlag_StartupSize_InferHeight  |  WindowLayoutFlag_StartupSize_InferWidth  |  WindowLayoutFlag_StartupSize_Infer  |  WindowLayoutFlag_Align_Right  |  WindowLayoutFlag_Align_Bottom  |  WindowLayoutFlag_Align_BottomRight  |  WindowLayoutFlag_Default  |  WindowLayoutFlag_DeferFree  ;
  u32 ExtraBits = (MaskBits | Value) ^ MaskBits;

  b32 Result = ExtraBits == 0;
  return Result;
}



