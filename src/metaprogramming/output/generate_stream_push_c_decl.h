
    function void
    Push(struct_member_stream* Stream, struct_member Element, memory_arena* Memory)
    {
      struct_member_stream_chunk* NextChunk = (struct_member_stream_chunk*)PushStruct(Memory, sizeof( struct_member_stream_chunk ), 1, 1);
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

