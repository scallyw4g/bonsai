struct counted_string_stream_chunk
{
  counted_string Element;
  counted_string_stream_chunk* Next;
  counted_string_stream_chunk* Prev;
};

struct counted_string_stream
{
  counted_string_stream_chunk Sentinel;
};

struct counted_string_iterator
{
  counted_string_stream* Stream;
  counted_string_stream_chunk* At;
};

function void
Push(counted_string_stream* Stream, counted_string Element, memory_arena* Memory)
{
  // TODO(Jesse): Can we use Allocate() here instead?
  counted_string_stream_chunk* Push = (counted_string_stream_chunk*)PushStruct(Memory, sizeof(counted_string_stream_chunk), 1, 1);
  Push->Element = Element;

  Push->Prev = &Stream->Sentinel;
  Push->Next = Stream->Sentinel.Next;
  Stream->Sentinel.Next->Prev = Push;
  Stream->Sentinel.Next = Push;
}

function counted_string_iterator
Iterator(counted_string_stream* Stream)
{
  counted_string_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->Sentinel.Next
  };
  return Iterator;
}

function b32
IsValid(counted_string_iterator* Iter)
{
  b32 Result = (Iter->At != &Iter->Stream->Sentinel);
  return Result;
}

function void
Advance(counted_string_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}


