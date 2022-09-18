struct counted_string_cursor
{
  counted_string *Start;
  counted_string *At;
  counted_string *End;
};

link_internal counted_string_cursor
CountedStringCursor(umm ElementCount, memory_arena* Memory)
{
  counted_string *Start = (counted_string*)PushStruct(Memory, sizeof(counted_string), 1, 0);
  counted_string_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
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

  counted_string *Result = &NextChunk->Element;
  return Result;
}



