struct u64_cursor
{
  u64 *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  u64 *At;
  u64 *End;
  /* OWNED_BY_THREAD_MEMBER(); */
};



link_internal u64_cursor
U64Cursor(umm ElementCount, memory_arena* Memory)
{
  u64 *Start = (u64*)PushStruct(Memory, sizeof(u64)*ElementCount, 1, 0);
  u64_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal u64*
GetPtr(u64_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  u64 *Result = {};
  if (ElementIndex < AtElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal u64*
GetPtrUnsafe(u64_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  u64 *Result = {};
  if (ElementIndex < TotalElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal u64
Get(u64_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert(ElementIndex < CurrentCount(Cursor));
  u64 Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(u64_cursor *Cursor, umm ElementIndex, u64 Element)
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

link_internal u64*
Advance(u64_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  u64 * Result = {};
  if ( Cursor->At < Cursor->End ) { Result = Cursor->At++; }
  return Result;
}

link_internal u64 *
Push(u64_cursor *Cursor, u64 Element)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At < Cursor->End );
  u64 *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal u64
Pop(u64_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At > Cursor->Start );
  u64 Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(u64_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal b32
Remove(u64_cursor *Cursor, u64 Query)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    u64 Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      u64 Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}


link_internal b32
ResizeCursor(u64_cursor *Cursor, umm Count, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  umm CurrentSize = TotalSize(Cursor);

  TruncateToElementCount(Cursor, Count);
  umm NewSize = TotalSize(Cursor);

  Assert(NewSize/sizeof(u64) == Count);

  /* Info("Attempting to reallocate CurrentSize(%u), NewSize(%u)", CurrentSize, NewSize); */
  Ensure(Reallocate((u8*)Cursor->Start, Memory, CurrentSize, NewSize));
  return 0;
}

link_internal void
Unshift( u64_cursor *Cursor )
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


struct u64_stream_chunk
{
  u64 Element;
  u64_stream_chunk* Next;
};

struct u64_stream
{
  memory_arena *Memory;
  u64_stream_chunk* FirstChunk;
  u64_stream_chunk* LastChunk;
  umm ChunkCount;
};

link_internal void
Deallocate(u64_stream *Stream)
{
  Stream->LastChunk = 0;
  Stream->FirstChunk = 0;
  VaporizeArena(Stream->Memory);
}

struct u64_iterator
{
  u64_stream* Stream;
  u64_stream_chunk* At;
};

link_internal u64_iterator
Iterator(u64_stream* Stream)
{
  u64_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

link_internal b32
IsValid(u64_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

link_internal void
Advance(u64_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

link_internal b32
IsLastElement(u64_iterator* Iter)
{
  b32 Result = Iter->At->Next == 0;
  return Result;
}


link_internal u64 *
Push(u64_stream* Stream, u64 Element)
{
  if (Stream->Memory == 0)
  {
    Stream->Memory = AllocateArena();
  }

  /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
  u64_stream_chunk* NextChunk = (u64_stream_chunk*)PushStruct(Stream->Memory, sizeof(u64_stream_chunk), 1, 0);
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

  u64 *Result = &NextChunk->Element;
  return Result;
}



