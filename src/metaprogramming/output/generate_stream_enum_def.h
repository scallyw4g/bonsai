
    struct enum_def_stream_chunk
    {
      enum_def Element;
      enum_def_stream_chunk* Next;
    };

    struct enum_def_stream
    {
      enum_def_stream_chunk* FirstChunk;
      enum_def_stream_chunk* LastChunk;
    };

    function void
    Push(enum_def_stream* Stream, enum_def Element, memory_arena* Memory)
    {
      enum_def_stream_chunk* NextChunk = (enum_def_stream_chunk*)PushStruct(Memory, sizeof(enum_def_stream_chunk), 1, 1);
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

    struct enum_def_iterator
    {
      enum_def_stream* Stream;
      enum_def_stream_chunk* At;
    };

    function enum_def_iterator
    Iterator(enum_def_stream* Stream)
    {
      enum_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(enum_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(enum_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


