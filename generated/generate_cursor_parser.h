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
  parser_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
}

link_internal parser*
GetPtr(parser_cursor *Cursor, umm ElementIndex)
{
  parser *Result = {};
  if (ElementIndex < AtElements(Cursor)) {
    Result = Cursor->Start+ElementIndex;
  }
  return Result;
}

link_internal parser
Get(parser_cursor *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < CurrentCount(Cursor));
  parser Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(parser_cursor *Cursor, umm ElementIndex, parser Element)
{
  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal parser *
Push(parser_cursor *Cursor, parser Element)
{
  Assert( Cursor->At < Cursor->End );
  parser *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal parser
Pop(parser_cursor *Cursor)
{
  Assert( Cursor->At > Cursor->Start );
  parser Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(parser_cursor *Cursor)
{
  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal b32
Remove(parser_cursor *Cursor, parser Query)
{
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


