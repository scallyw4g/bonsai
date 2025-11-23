// callsite
// external/bonsai_stdlib/src/vector.cpp:8:0

// def (generate_stream)
// external/bonsai_stdlib/src/poof_functions.h:2119:0
struct v3i_stream_chunk
{
  v3i Element;
  v3i_stream_chunk* Next;
};

struct v3i_stream
{
  memory_arena *Memory;
  v3i_stream_chunk* FirstChunk;
  v3i_stream_chunk* LastChunk;
  umm ChunkCount;
};

link_internal v3i_stream
V3iStream(memory_arena *Memory)
{
  v3i_stream Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal void
Deallocate(v3i_stream *Stream)
{
  Stream->LastChunk = 0;
  Stream->FirstChunk = 0;
  VaporizeArena(Stream->Memory);
}

struct v3i_iterator
{
  v3i_stream* Stream;
  v3i_stream_chunk* At;
};

link_internal v3i_iterator
Iterator(v3i_stream* Stream)
{
  v3i_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

link_internal b32
IsValid(v3i_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

link_internal void
Advance(v3i_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

link_internal b32
IsLastElement(v3i_iterator* Iter)
{
  b32 Result = Iter->At->Next == 0;
  return Result;
}


link_internal v3i *
Push(v3i_stream* Stream, v3i Element)
{
  Assert(Stream->Memory);

  /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
  v3i_stream_chunk* NextChunk = (v3i_stream_chunk*)PushStruct(Stream->Memory, sizeof(v3i_stream_chunk), 1, 0);
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

  v3i *Result = &NextChunk->Element;
  return Result;
}


