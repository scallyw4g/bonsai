
    
    struct enum_member_stream_chunk
    {
      enum_member Element;
      enum_member_stream_chunk* Next;
    };


    
    struct enum_member_stream
    {
      enum_member_stream_chunk* FirstChunk;
      enum_member_stream_chunk* LastChunk;
    };



    
    struct enum_member_iterator
    {
      enum_member_stream* Stream;
      enum_member_stream_chunk* At;
    };

    function enum_member_iterator
    Iterator(enum_member_stream* Stream)
    {
      enum_member_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(enum_member_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(enum_member_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }



    
    function void
    Push(enum_member_stream* Stream, enum_member Element, memory_arena* Memory)
    {
      enum_member_stream_chunk* NextChunk = (enum_member_stream_chunk*)PushStruct(Memory, sizeof( enum_member_stream_chunk ), 1, 1);
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


