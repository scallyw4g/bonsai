// callsite
// external/bonsai_stdlib/src/counted_string.h:123:0

// def (stream_and_cursor)
// external/bonsai_stdlib/src/poof_functions.h:2139:0

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
  counted_string_cursor Result = {};

  Result.Start = Start;
  Result.End = Start+ElementCount;
  Result.At = Start;

  return Result;
}

link_internal counted_string*
GetPtr( counted_string_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  counted_string *Result = {};
  if (ElementIndex < AtElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal counted_string*
TryGetPtr( counted_string_cursor *Cursor, umm ElementIndex)
{
  return GetPtr(Cursor, ElementIndex);
}

link_internal counted_string*
GetPtrUnsafe( counted_string_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  counted_string *Result = {};
  if (ElementIndex < TotalElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal counted_string
Get( counted_string_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert(ElementIndex < CurrentCount(Cursor));
  counted_string Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set( counted_string_cursor *Cursor, umm ElementIndex, counted_string Element)
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

link_internal counted_string*
Advance( counted_string_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  counted_string * Result = {};
  if ( Cursor->At < Cursor->End ) { Result = Cursor->At++; }
  return Result;
}

link_internal counted_string *
Push( counted_string_cursor *Cursor, counted_string Element)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At < Cursor->End );
  counted_string *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal counted_string
Pop( counted_string_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At > Cursor->Start );
  counted_string Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex( counted_string_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal counted_string*
LastElement( counted_string_cursor *Cursor)
{
  counted_string *Result = {};
  s32 I = LastIndex(Cursor);
  if (I > -1) { Result = Cursor->Start + I; }
  return Result;
}

link_internal b32
Remove( counted_string_cursor *Cursor, counted_string Query)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

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


link_internal b32
ResizeCursor( counted_string_cursor *Cursor, umm Count, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  umm CurrentSize = TotalSize(Cursor);

  TruncateToElementCount(Cursor, Count);
  umm NewSize = TotalSize(Cursor);

  Assert(NewSize/sizeof(counted_string) == Count);

  /* Info("Attempting to reallocate CurrentSize(%u), NewSize(%u)", CurrentSize, NewSize); */
  Ensure(Reallocate((u8*)Cursor->Start, Memory, CurrentSize, NewSize));
  return 0;
}

link_internal void
Unshift( counted_string_cursor *Cursor )
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

link_internal counted_string_stream
CountedStringStream(memory_arena *Memory)
{
  counted_string_stream Result = {};
  Result.Memory = Memory;
  return Result;
}

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
  Assert(Stream->Memory);

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



