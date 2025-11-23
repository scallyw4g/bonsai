// callsite
// external/bonsai_stdlib/src/counted_string.cpp:43:0

// def (generate_stream)
// external/bonsai_stdlib/src/poof_functions.h:2119:0
struct tuple_cs_cs_stream_chunk
{
  tuple_cs_cs Element;
  tuple_cs_cs_stream_chunk* Next;
};

struct tuple_cs_cs_stream
{
  memory_arena *Memory;
  tuple_cs_cs_stream_chunk* FirstChunk;
  tuple_cs_cs_stream_chunk* LastChunk;
  umm ChunkCount;
};

link_internal tuple_cs_cs_stream
TupleCsCsStream(memory_arena *Memory)
{
  tuple_cs_cs_stream Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal void
Deallocate(tuple_cs_cs_stream *Stream)
{
  Stream->LastChunk = 0;
  Stream->FirstChunk = 0;
  VaporizeArena(Stream->Memory);
}

struct tuple_cs_cs_iterator
{
  tuple_cs_cs_stream* Stream;
  tuple_cs_cs_stream_chunk* At;
};

link_internal tuple_cs_cs_iterator
Iterator(tuple_cs_cs_stream* Stream)
{
  tuple_cs_cs_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

link_internal b32
IsValid(tuple_cs_cs_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

link_internal void
Advance(tuple_cs_cs_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

link_internal b32
IsLastElement(tuple_cs_cs_iterator* Iter)
{
  b32 Result = Iter->At->Next == 0;
  return Result;
}


link_internal tuple_cs_cs *
Push(tuple_cs_cs_stream* Stream, tuple_cs_cs Element)
{
  Assert(Stream->Memory);

  /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
  tuple_cs_cs_stream_chunk* NextChunk = (tuple_cs_cs_stream_chunk*)PushStruct(Stream->Memory, sizeof(tuple_cs_cs_stream_chunk), 1, 0);
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

  tuple_cs_cs *Result = &NextChunk->Element;
  return Result;
}


