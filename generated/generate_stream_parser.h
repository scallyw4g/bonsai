// callsite
// external/bonsai_stdlib/src/c_parser.h:96:0

// def (generate_stream)
// external/bonsai_stdlib/src/poof_functions.h:2119:0
struct parser_stream_chunk
{
  parser Element;
  parser_stream_chunk* Next;
};

struct parser_stream
{
  memory_arena *Memory;
  parser_stream_chunk* FirstChunk;
  parser_stream_chunk* LastChunk;
  umm ChunkCount;
};

link_internal parser_stream
ParserStream(memory_arena *Memory)
{
  parser_stream Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal void
Deallocate(parser_stream *Stream)
{
  Stream->LastChunk = 0;
  Stream->FirstChunk = 0;
  VaporizeArena(Stream->Memory);
}

struct parser_iterator
{
  parser_stream* Stream;
  parser_stream_chunk* At;
};

link_internal parser_iterator
Iterator(parser_stream* Stream)
{
  parser_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

link_internal b32
IsValid(parser_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

link_internal void
Advance(parser_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

link_internal b32
IsLastElement(parser_iterator* Iter)
{
  b32 Result = Iter->At->Next == 0;
  return Result;
}


link_internal parser *
Push(parser_stream* Stream, parser Element)
{
  Assert(Stream->Memory);

  /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
  parser_stream_chunk* NextChunk = (parser_stream_chunk*)PushStruct(Stream->Memory, sizeof(parser_stream_chunk), 1, 0);
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

  parser *Result = &NextChunk->Element;
  return Result;
}


