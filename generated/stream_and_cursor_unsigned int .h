struct u32_cursor
{
  u32 *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  u32 *At;
  u32 *End;
};

link_internal u32_cursor
U32Cursor(umm ElementCount, memory_arena* Memory)
{
  u32 *Start = (u32*)PushStruct(Memory, sizeof(u32)*ElementCount, 1, 0);
  u32_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
}

link_internal u32
Get(u32_cursor *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < CurrentCount(Cursor));
  u32 Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(u32_cursor *Cursor, umm ElementIndex, u32 Element)
{
  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal u32 *
Push(u32_cursor *Cursor, u32 Element)
{
  Assert( Cursor->At < Cursor->End );
  u32 *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal u32
Pop(u32_cursor *Cursor)
{
  Assert( Cursor->At > Cursor->Start );
  u32 Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(u32_cursor *Cursor)
{
  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal b32
Remove(u32_cursor *Cursor, u32 Query)
{
  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    u32 Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      u32 Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}

struct u32_stream_chunk
{
  u32 Element;
  u32_stream_chunk* Next;
};

struct u32_stream
{
  memory_arena *Memory;
  u32_stream_chunk* FirstChunk;
  u32_stream_chunk* LastChunk;
  umm ChunkCount;
};

link_internal void
Deallocate(u32_stream *Stream)
{
  Stream->LastChunk = 0;
  Stream->FirstChunk = 0;
  VaporizeArena(Stream->Memory);
}

struct u32_iterator
{
  u32_stream* Stream;
  u32_stream_chunk* At;
};

link_internal u32_iterator
Iterator(u32_stream* Stream)
{
  u32_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

link_internal b32
IsValid(u32_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

link_internal void
Advance(u32_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

link_internal b32
IsLastElement(u32_iterator* Iter)
{
  b32 Result = Iter->At->Next == 0;
  return Result;
}


link_internal u32 *
Push(u32_stream* Stream, u32 Element)
{
  if (Stream->Memory == 0)
  {
    Stream->Memory = AllocateArena();
  }

  /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
  u32_stream_chunk* NextChunk = (u32_stream_chunk*)PushStruct(Stream->Memory, sizeof(u32_stream_chunk), 1, 0);
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

  u32 *Result = &NextChunk->Element;
  return Result;
}



