// callsite
// external/bonsai_stdlib/src/counted_string.h:131:0

// def (generate_stream)
// external/bonsai_stdlib/src/poof_functions.h:2119:0
struct tagged_counted_string_stream_stream_chunk
{
  tagged_counted_string_stream Element;
  tagged_counted_string_stream_stream_chunk* Next;
};

struct tagged_counted_string_stream_stream
{
  memory_arena *Memory;
  tagged_counted_string_stream_stream_chunk* FirstChunk;
  tagged_counted_string_stream_stream_chunk* LastChunk;
  umm ChunkCount;
};

link_internal tagged_counted_string_stream_stream
TaggedCountedStringStreamStream(memory_arena *Memory)
{
  tagged_counted_string_stream_stream Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal void
Deallocate(tagged_counted_string_stream_stream *Stream)
{
  Stream->LastChunk = 0;
  Stream->FirstChunk = 0;
  VaporizeArena(Stream->Memory);
}

struct tagged_counted_string_stream_iterator
{
  tagged_counted_string_stream_stream* Stream;
  tagged_counted_string_stream_stream_chunk* At;
};

link_internal tagged_counted_string_stream_iterator
Iterator(tagged_counted_string_stream_stream* Stream)
{
  tagged_counted_string_stream_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

link_internal b32
IsValid(tagged_counted_string_stream_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

link_internal void
Advance(tagged_counted_string_stream_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

link_internal b32
IsLastElement(tagged_counted_string_stream_iterator* Iter)
{
  b32 Result = Iter->At->Next == 0;
  return Result;
}


link_internal tagged_counted_string_stream *
Push(tagged_counted_string_stream_stream* Stream, tagged_counted_string_stream Element)
{
  Assert(Stream->Memory);

  /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
  tagged_counted_string_stream_stream_chunk* NextChunk = (tagged_counted_string_stream_stream_chunk*)PushStruct(Stream->Memory, sizeof(tagged_counted_string_stream_stream_chunk), 1, 0);
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

  tagged_counted_string_stream *Result = &NextChunk->Element;
  return Result;
}


