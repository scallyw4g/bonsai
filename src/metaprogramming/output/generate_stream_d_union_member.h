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

    bonsai_function d_union_member_iterator
    Iterator(d_union_member_stream* Stream)
    {
      d_union_member_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(d_union_member_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(d_union_member_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


        bonsai_function d_union_member *
    Push(d_union_member_stream* Stream, d_union_member Element, memory_arena* Memory)
    {
      d_union_member_stream_chunk* NextChunk = (d_union_member_stream_chunk*)PushStruct(Memory, sizeof( d_union_member_stream_chunk ), 1, 0);
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

      d_union_member *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( d_union_member_stream *S1, d_union_member_stream *S2)
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


