struct counted_string_cursor
{
  counted_string *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  counted_string *At;
  counted_string *End;
};

link_internal counted_string_cursor
CountedStringCursor(umm ElementCount, memory_arena* Memory)
{
  counted_string *Start = (counted_string*)PushStruct(Memory, sizeof(counted_string)*ElementCount, 1, 0);
  counted_string_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
}

link_internal counted_string*
GetPtr(counted_string_cursor *Cursor, umm ElementIndex)
{
  counted_string *Result = {};
  if (ElementIndex < AtElements(Cursor)) {
    Result = Cursor->Start+ElementIndex;
  }
  return Result;
}

link_internal counted_string
Get(counted_string_cursor *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < CurrentCount(Cursor));
  counted_string Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(counted_string_cursor *Cursor, umm ElementIndex, counted_string Element)
{
  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal counted_string *
Push(counted_string_cursor *Cursor, counted_string Element)
{
  Assert( Cursor->At < Cursor->End );
  counted_string *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal counted_string
Pop(counted_string_cursor *Cursor)
{
  Assert( Cursor->At > Cursor->Start );
  counted_string Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(counted_string_cursor *Cursor)
{
  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal b32
Remove(counted_string_cursor *Cursor, counted_string Query)
{
  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    counted_string Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      counted_string Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}

link_internal void
DeepCopy(counted_string_cursor *Src, counted_string_cursor *Dest)
{
  umm SrcAt = AtElements(Src);
  Assert(SrcAt <= TotalElements(Dest));

  IterateOver(Src, Element, ElementIndex)
  {
    DeepCopy(Element, Dest->Start+ElementIndex);
  }

  Dest->At = Dest->Start+SrcAt;
  Assert(Dest->At < Dest->End);
}

struct counted_string_stream_chunk
{
  counted_string Element;
  counted_string_stream_chunk* Next;
};

struct counted_string_stream
{
  memory_arena *Memory;
  counted_string_stream_chunk* FirstChunk;
  counted_string_stream_chunk* LastChunk;
  umm ChunkCount;
};

link_internal void
Deallocate(counted_string_stream *Stream)
{
  Stream->LastChunk = 0;
  Stream->FirstChunk = 0;
  VaporizeArena(Stream->Memory);
}

struct counted_string_iterator
{
  counted_string_stream* Stream;
  counted_string_stream_chunk* At;
};

link_internal counted_string_iterator
Iterator(counted_string_stream* Stream)
{
  counted_string_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

link_internal b32
IsValid(counted_string_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

link_internal void
Advance(counted_string_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

link_internal b32
IsLastElement(counted_string_iterator* Iter)
{
  b32 Result = Iter->At->Next == 0;
  return Result;
}


link_internal counted_string *
Push(counted_string_stream* Stream, counted_string Element)
{
  if (Stream->Memory == 0)
  {
    Stream->Memory = AllocateArena();
  }

  /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
  counted_string_stream_chunk* NextChunk = (counted_string_stream_chunk*)PushStruct(Stream->Memory, sizeof(counted_string_stream_chunk), 1, 0);
  NextChunk->Element = Element;

  if (!Stream->FirstChunk)
  {
    Assert(!Stream->LastChunk);
    Stream->FirstChunk = NextChunk;
    Stream->LastChunk = NextChunk;
  }
  else
  {
    Stream->LastChunk->Next = NextChunk;
    Stream->LastChunk = NextChunk;
  }

  Assert(NextChunk->Next == 0);
  Assert(Stream->LastChunk->Next == 0);

  Stream->ChunkCount += 1;

  counted_string *Result = &NextChunk->Element;
  return Result;
}



