
    
    struct struct_def_cursor
    {
      struct_def* Start;
      struct_def* End;
      struct_def* At;
    };

    bonsai_function struct_def_cursor
    StructDefCursor(umm ElementCount, memory_arena* Memory)
    {
      struct_def* Start = (struct_def*)PushStruct(Memory, sizeof( struct_def ), 1, 0);
      struct_def_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    }

    
    
    struct struct_def_stream_chunk
    {
      struct_def Element;
      struct_def_stream_chunk* Next;
    };

    
    struct struct_def_stream
    {
      struct_def_stream_chunk* FirstChunk;
      struct_def_stream_chunk* LastChunk;
    };


    
    struct struct_def_iterator
    {
      struct_def_stream* Stream;
      struct_def_stream_chunk* At;
    };

    bonsai_function struct_def_iterator
    Iterator(struct_def_stream* Stream)
    {
      struct_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(struct_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(struct_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    bonsai_function struct_def *
    Push(struct_def_stream* Stream, struct_def Element, memory_arena* Memory)
    {
      struct_def_stream_chunk* NextChunk = (struct_def_stream_chunk*)PushStruct(Memory, sizeof( struct_def_stream_chunk ), 1, 0);
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

      struct_def *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( struct_def_stream *S1, struct_def_stream *S2)
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




