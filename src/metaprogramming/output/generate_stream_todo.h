
    
    struct todo_stream_chunk
    {
      todo Element;
      todo_stream_chunk* Next;
    };

    
    struct todo_stream
    {
      todo_stream_chunk* FirstChunk;
      todo_stream_chunk* LastChunk;
    };


    
    struct todo_iterator
    {
      todo_stream* Stream;
      todo_stream_chunk* At;
    };

    bonsai_function todo_iterator
    Iterator(todo_stream* Stream)
    {
      todo_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(todo_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(todo_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    bonsai_function todo *
    Push(todo_stream* Stream, todo Element, memory_arena* Memory)
    {
      todo_stream_chunk* NextChunk = (todo_stream_chunk*)PushStruct(Memory, sizeof( todo_stream_chunk ), 1, 0);
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

      todo *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( todo_stream *S1, todo_stream *S2)
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



