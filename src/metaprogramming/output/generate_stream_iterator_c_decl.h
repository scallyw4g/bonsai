
    struct struct_member_iterator
    {
      struct_member_stream* Stream;
      struct_member_stream_chunk* At;
    };

    function struct_member_iterator
    Iterator(struct_member_stream* Stream)
    {
      struct_member_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(struct_member_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(struct_member_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


