
    function void
    Push(c_decl_stream* Stream, c_decl Element, memory_arena* Memory)
    {
      c_decl_stream_chunk* NextChunk = (c_decl_stream_chunk*)PushStruct(Memory, sizeof( c_decl_stream_chunk ), 1, 1);
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

