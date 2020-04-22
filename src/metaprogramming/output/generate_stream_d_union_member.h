
    
    struct d_union_member_stream_chunk
    {
      d_union_member Element;
      d_union_member_stream_chunk* Next;
    };

    
    struct d_union_member_stream
    {
      d_union_member_stream_chunk* FirstChunk;
      d_union_member_stream_chunk* LastChunk;
    };


    
    struct d_union_member_iterator
    {
      d_union_member_stream* Stream;
      d_union_member_stream_chunk* At;
    };

    function d_union_member_iterator
    Iterator(d_union_member_stream* Stream)
    {
      d_union_member_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(d_union_member_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(d_union_member_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function void
    Push(d_union_member_stream* Stream, d_union_member Element, memory_arena* Memory)
    {
      d_union_member_stream_chunk* NextChunk = (d_union_member_stream_chunk*)PushStruct(Memory, sizeof( d_union_member_stream_chunk ), 1, 1);
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


