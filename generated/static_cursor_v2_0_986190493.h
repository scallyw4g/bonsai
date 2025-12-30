// callsite
// external/bonsai_stdlib/src/vector.h:1199:0

// def (static_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1262:0


struct v2_static_cursor_16
poof(@do_editor_ui)
{
  v2 Start[16]; poof(@array_length(Element->At))
  u32 At;
};

link_inline v2*
GetPtr( v2_static_cursor_16 *Buf, umm Index)
{
  v2 *Result = {};
  if ( Index < umm(16) )
  {
    Result = Buf->Start+Index;
  }
  return Result;
}

link_inline v2*
TryGetPtr( v2_static_cursor_16 *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline v2
Get( v2_static_cursor_16 *Buf, umm Index)
{
  Assert(Index < umm(16));
  Assert(Index < umm((Buf->At)));
  v2 Result = Buf->Start[Index];
  return Result;
}

link_internal umm
AtElements( v2_static_cursor_16  *Buf)
{
  return Buf->At;
}

link_internal umm
TotalElements( v2_static_cursor_16 *Buf)
{
  return 16;
}

link_inline void
Push( v2_static_cursor_16 *Buf, v2 *E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = *E;
}

link_inline void
Push( v2_static_cursor_16 *Buf, v2 E )
{
  Assert(AtElements(Buf) < TotalElements(Buf));
  Buf->Start[Buf->At++] = E;
}

