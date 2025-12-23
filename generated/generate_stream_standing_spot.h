// callsite
// src/engine/world_chunk.h:543:0

// def (generate_stream)
// external/bonsai_stdlib/src/poof_functions.h:2119:0
struct standing_spot_stream_chunk
{
  standing_spot Element;
  standing_spot_stream_chunk* Next;
};

struct standing_spot_stream
{
  memory_arena *Memory;
  standing_spot_stream_chunk* FirstChunk;
  standing_spot_stream_chunk* LastChunk;
  umm ChunkCount;
};

link_internal standing_spot_stream
StandingSpotStream(memory_arena *Memory)
{
  standing_spot_stream Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal void
Deallocate(standing_spot_stream *Stream)
{
  Stream->LastChunk = 0;
  Stream->FirstChunk = 0;
  VaporizeArena(Stream->Memory);
}

struct standing_spot_iterator
{
  standing_spot_stream* Stream;
  standing_spot_stream_chunk* At;
};

link_internal standing_spot_iterator
Iterator(standing_spot_stream* Stream)
{
  standing_spot_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

link_internal b32
IsValid(standing_spot_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

link_internal void
Advance(standing_spot_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

link_internal b32
IsLastElement(standing_spot_iterator* Iter)
{
  b32 Result = Iter->At->Next == 0;
  return Result;
}


link_internal standing_spot *
Push(standing_spot_stream* Stream, standing_spot Element)
{
  Assert(Stream->Memory);

  /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
  standing_spot_stream_chunk* NextChunk = (standing_spot_stream_chunk*)PushStruct(Stream->Memory, sizeof(standing_spot_stream_chunk), 1, 0);
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

  standing_spot *Result = &NextChunk->Element;
  return Result;
}


