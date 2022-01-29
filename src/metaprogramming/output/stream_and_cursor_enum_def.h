        struct enum_def_cursor
    {
      enum_def* Start;
      enum_def* At;
      enum_def* End;
    };

    bonsai_function enum_def_cursor
    EnumDefCursor(umm ElementCount, memory_arena* Memory)
    {
      enum_def* Start = (enum_def*)PushStruct(Memory, sizeof( enum_def ), 1, 0);
      enum_def_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    }

            struct enum_def_stream_chunk
    {
      enum_def Element;
      enum_def_stream_chunk* Next;
    };

        struct enum_def_stream
    {
      enum_def_stream_chunk* FirstChunk;
      enum_def_stream_chunk* LastChunk;
    };


        struct enum_def_iterator
    {
      enum_def_stream* Stream;
      enum_def_stream_chunk* At;
    };

    bonsai_function enum_def_iterator
    Iterator(enum_def_stream* Stream)
    {
      enum_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(enum_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(enum_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


        bonsai_function enum_def *
    Push(enum_def_stream* Stream, enum_def Element, memory_arena* Memory)
    {
      enum_def_stream_chunk* NextChunk = (enum_def_stream_chunk*)PushStruct(Memory, sizeof( enum_def_stream_chunk ), 1, 0);
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

      enum_def *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( enum_def_stream *S1, enum_def_stream *S2)
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



