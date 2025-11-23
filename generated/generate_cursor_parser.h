// callsite
// external/bonsai_stdlib/src/c_parser.h:93:0

// def (generate_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1569:0

struct parser_cursor
{
  parser *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  parser *At;
  parser *End;
};




link_internal parser_cursor
ParserCursor(umm ElementCount, memory_arena* Memory)
{
  parser *Start = (parser*)PushStruct(Memory, sizeof(parser)*ElementCount, 1, 0);
  parser_cursor Result = {};

  Result.Start = Start;
  Result.End = Start+ElementCount;
  Result.At = Start;

  return Result;
}

link_internal parser*
GetPtr( parser_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  parser *Result = {};
  if (ElementIndex < AtElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal parser*
TryGetPtr( parser_cursor *Cursor, umm ElementIndex)
{
  return GetPtr(Cursor, ElementIndex);
}

link_internal parser*
GetPtrUnsafe( parser_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  parser *Result = {};
  if (ElementIndex < TotalElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal parser
Get( parser_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert(ElementIndex < CurrentCount(Cursor));
  parser Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set( parser_cursor *Cursor, umm ElementIndex, parser Element)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal parser*
Advance( parser_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  parser * Result = {};
  if ( Cursor->At < Cursor->End ) { Result = Cursor->At++; }
  return Result;
}

link_internal parser *
Push( parser_cursor *Cursor, parser Element)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At < Cursor->End );
  parser *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal parser
Pop( parser_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At > Cursor->Start );
  parser Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex( parser_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal parser*
LastElement( parser_cursor *Cursor)
{
  parser *Result = {};
  s32 I = LastIndex(Cursor);
  if (I > -1) { Result = Cursor->Start + I; }
  return Result;
}

link_internal b32
Remove( parser_cursor *Cursor, parser Query)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    parser Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      parser Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}


link_internal b32
ResizeCursor( parser_cursor *Cursor, umm Count, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  umm CurrentSize = TotalSize(Cursor);

  TruncateToElementCount(Cursor, Count);
  umm NewSize = TotalSize(Cursor);

  Assert(NewSize/sizeof(parser) == Count);

  /* Info("Attempting to reallocate CurrentSize(%u), NewSize(%u)", CurrentSize, NewSize); */
  Ensure(Reallocate((u8*)Cursor->Start, Memory, CurrentSize, NewSize));
  return 0;
}

link_internal void
Unshift( parser_cursor *Cursor )
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */
  umm Count = AtElements(Cursor);
  if (Count)
  {
    for (umm Index = 1; Index < Count; ++Index)
    {
      Cursor->Start[Index-1] = Cursor->Start[Index];
    }

    // NOTE(Jesse): This is actually correct, even though it doesn't look
    // like it at first glance.  At is OnePastLast, so decrementing and
    // then clearing overwrites the last value that was set.
    Cursor->At--;
    *Cursor->At = {};
  }
}



