
    
    struct c_token_buffer_stream_chunk
    {
      c_token_buffer Element;
      c_token_buffer_stream_chunk* Next;
    };

    
    struct c_token_buffer_stream
    {
      c_token_buffer_stream_chunk* FirstChunk;
      c_token_buffer_stream_chunk* LastChunk;
    };


    
    struct c_token_buffer_iterator
    {
      c_token_buffer_stream* Stream;
      c_token_buffer_stream_chunk* At;
    };

    bonsai_function c_token_buffer_iterator
    Iterator(c_token_buffer_stream* Stream)
    {
      c_token_buffer_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(c_token_buffer_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(c_token_buffer_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    bonsai_function c_token_buffer *
    Push(c_token_buffer_stream* Stream, c_token_buffer Element, memory_arena* Memory)
    {
      c_token_buffer_stream_chunk* NextChunk = (c_token_buffer_stream_chunk*)PushStruct(Memory, sizeof( c_token_buffer_stream_chunk ), 1, 0);
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

      c_token_buffer *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( c_token_buffer_stream *S1, c_token_buffer_stream *S2)
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



