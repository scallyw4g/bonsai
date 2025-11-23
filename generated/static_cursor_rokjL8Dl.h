// callsite
// external/bonsai_stdlib/src/framebuffer.h:15:0

// def (static_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1262:0


struct rtt_framebuffer_static_cursor_3
poof(@do_editor_ui)
{
  rtt_framebuffer Start[3]; poof(@array_length(Element->At))
  u32 At;
};

link_inline rtt_framebuffer*
GetPtr( rtt_framebuffer_static_cursor_3 *Buf, umm Index)
{
  rtt_framebuffer *Result = {};
  if ( Index < umm(3) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline rtt_framebuffer*
TryGetPtr( rtt_framebuffer_static_cursor_3 *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline rtt_framebuffer
Get( rtt_framebuffer_static_cursor_3 *Buf, umm Index)
{
  Assert(Index < umm(3));
  Assert(Index < umm((Buf->At)));
  rtt_framebuffer Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( rtt_framebuffer_static_cursor_3  *Buf)
{
  return Buf->At;
}

link_internal umm
TotalElements( rtt_framebuffer_static_cursor_3 *Buf)
{
  return 3;
}

link_inline void
Push( rtt_framebuffer_static_cursor_3 *Buf, rtt_framebuffer *E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = *E;
}

link_inline void
Push( rtt_framebuffer_static_cursor_3 *Buf, rtt_framebuffer E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = E;
}

