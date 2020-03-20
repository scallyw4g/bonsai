
    
    struct enum_field_stream_chunk
    {
      enum_field Element;
      enum_field_stream_chunk* Next;
    };


    
    struct enum_field_stream
    {
      enum_field_stream_chunk* FirstChunk;
      enum_field_stream_chunk* LastChunk;
    };



    
    struct enum_field_iterator
    {
      enum_field_stream* Stream;
      enum_field_stream_chunk* At;
    };

    function enum_field_iterator
    Iterator(enum_field_stream* Stream)
    {
      enum_field_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(enum_field_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(enum_field_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }



    
    function void
    Push(enum_field_stream* Stream, enum_field Element, memory_arena* Memory)
    {
      enum_field_stream_chunk* NextChunk = (enum_field_stream_chunk*)PushStruct(Memory, sizeof( enum_field_stream_chunk ), 1, 1);
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


