
    
    struct variable_decl_stream_chunk
    {
      variable_decl Element;
      variable_decl_stream_chunk* Next;
    };

    
    struct variable_decl_stream
    {
      variable_decl_stream_chunk* FirstChunk;
      variable_decl_stream_chunk* LastChunk;
    };


    
    struct variable_decl_iterator
    {
      variable_decl_stream* Stream;
      variable_decl_stream_chunk* At;
    };

    bonsai_function variable_decl_iterator
    Iterator(variable_decl_stream* Stream)
    {
      variable_decl_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(variable_decl_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(variable_decl_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    bonsai_function variable_decl *
    Push(variable_decl_stream* Stream, variable_decl Element, memory_arena* Memory)
    {
      variable_decl_stream_chunk* NextChunk = (variable_decl_stream_chunk*)PushStruct(Memory, sizeof( variable_decl_stream_chunk ), 1, 0);
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

      variable_decl *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( variable_decl_stream *S1, variable_decl_stream *S2)
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



