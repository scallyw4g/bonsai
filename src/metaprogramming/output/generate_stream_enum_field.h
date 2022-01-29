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

    bonsai_function enum_member_iterator
    Iterator(enum_member_stream* Stream)
    {
      enum_member_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(enum_member_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(enum_member_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


        bonsai_function enum_member *
    Push(enum_member_stream* Stream, enum_member Element, memory_arena* Memory)
    {
      enum_member_stream_chunk* NextChunk = (enum_member_stream_chunk*)PushStruct(Memory, sizeof( enum_member_stream_chunk ), 1, 0);
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

      enum_member *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( enum_member_stream *S1, enum_member_stream *S2)
    {
      if (S1->LastChunk)
      {
        Assert(S1->FirstChunk);

        if (S2->FirstChunk)
        {
          Assert(S2->LastChunk);
          S1->LastChunk->Next = S2->FirstChunk;
          S1->LastChunk = S2->LastChunk;
        }
        else
        {
          Assert(!S2->LastChunk);
        }
      }
      else
      {
        Assert(!S1->FirstChunk);
        Assert(!S1->LastChunk);

        if(S2->FirstChunk)
        {
          Assert(S2->LastChunk);
        }
        else
        {
          Assert(!S2->LastChunk);
        }

        *S1 = *S2;
      }
    }


