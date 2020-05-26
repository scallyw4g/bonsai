
    function void
    Push(struct_member_stream* Stream, struct_member Element, memory_arena* Memory)
    {
      struct_member_stream_chunk* NextChunk = (struct_member_stream_chunk*)PushStruct(Memory, sizeof( struct_member_stream_chunk ), 1, 0);
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

    function void
    ConcatStreams( struct_member_stream *S1, struct_member_stream *S2)
    {
      if (S1->LastChunk)
      {
        S1->LastChunk->Next = S2->FirstChunk;
        S1->LastChunk = S2->LastChunk;
      }
      else
      {
        *S1 = *S2;
      }
    }

