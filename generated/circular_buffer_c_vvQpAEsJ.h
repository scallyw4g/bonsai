// callsite
// external/bonsai_stdlib/src/framebuffer.cpp:5:0

// def (circular_buffer_c)
// external/bonsai_stdlib/src/poof_functions.h:1785:0
/* generate_cursor_functions(element_t) */



link_internal s32
Find( rtt_framebuffer_static_cursor_3 *Array, rtt_framebuffer *Query)
{
  s32 Result = -1;
  IterateOver(Array, E, Index)
  {
    if (AreEqual( E, Query ))
    {
      Result = s32(Index);
      break;
    }
  }
  return Result;
}


link_internal s32
AdvanceIndex( rtt_framebuffer_static_cursor_3 *Array, s32 Index, s32 AdvanceCount)
{
  s32 Total = s32(TotalElements(Array));
  Assert(Index >= 0);
  Assert(Index < Total );
  s32 Result = (Index + AdvanceCount) % Total;
  return Result;
}


