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



