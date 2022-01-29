    struct struct_member_iterator
    {
      struct_member_stream* Stream;
      struct_member_stream_chunk* At;
    };

    bonsai_function struct_member_iterator
    Iterator(struct_member_stream* Stream)
    {
      struct_member_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(struct_member_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(struct_member_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


