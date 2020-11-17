
    
    struct meta_func_stream_chunk
    {
      meta_func Element;
      meta_func_stream_chunk* Next;
    };

    
    struct meta_func_stream
    {
      meta_func_stream_chunk* FirstChunk;
      meta_func_stream_chunk* LastChunk;
    };


    
    struct meta_func_iterator
    {
      meta_func_stream* Stream;
      meta_func_stream_chunk* At;
    };

    bonsai_function meta_func_iterator
    Iterator(meta_func_stream* Stream)
    {
      meta_func_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(meta_func_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(meta_func_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    bonsai_function meta_func *
    Push(meta_func_stream* Stream, meta_func Element, memory_arena* Memory)
    {
      meta_func_stream_chunk* NextChunk = (meta_func_stream_chunk*)PushStruct(Memory, sizeof( meta_func_stream_chunk ), 1, 0);
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

      meta_func *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( meta_func_stream *S1, meta_func_stream *S2)
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



