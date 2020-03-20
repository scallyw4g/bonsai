
    struct c_decl_iterator
    {
      c_decl_stream* Stream;
      c_decl_stream_chunk* At;
    };

    function c_decl_iterator
    Iterator(c_decl_stream* Stream)
    {
      c_decl_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(c_decl_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(c_decl_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


