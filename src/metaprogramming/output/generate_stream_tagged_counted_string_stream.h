
    struct tagged_counted_string_stream_stream_chunk
    {
      tagged_counted_string_stream Element;
      tagged_counted_string_stream_stream_chunk* Next;
    };

    struct tagged_counted_string_stream_stream
    {
      tagged_counted_string_stream_stream_chunk* FirstChunk;
      tagged_counted_string_stream_stream_chunk* LastChunk;
    };

    function void
    Push(tagged_counted_string_stream_stream* Stream, tagged_counted_string_stream Element, memory_arena* Memory)
    {
      tagged_counted_string_stream_stream_chunk* NextChunk = (tagged_counted_string_stream_stream_chunk*)PushStruct(Memory, sizeof( tagged_counted_string_stream_stream_chunk ), 1, 1);
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

      return;
    }

    struct tagged_counted_string_stream_iterator
    {
      tagged_counted_string_stream_stream* Stream;
      tagged_counted_string_stream_stream_chunk* At;
    };

    function tagged_counted_string_stream_iterator
    Iterator(tagged_counted_string_stream_stream* Stream)
    {
      tagged_counted_string_stream_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(tagged_counted_string_stream_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(tagged_counted_string_stream_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


