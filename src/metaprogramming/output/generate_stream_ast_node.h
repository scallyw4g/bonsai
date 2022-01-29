        struct ast_node_stream_chunk
    {
      ast_node Element;
      ast_node_stream_chunk* Next;
    };

        struct ast_node_stream
    {
      ast_node_stream_chunk* FirstChunk;
      ast_node_stream_chunk* LastChunk;
    };


        struct ast_node_iterator
    {
      ast_node_stream* Stream;
      ast_node_stream_chunk* At;
    };

    bonsai_function ast_node_iterator
    Iterator(ast_node_stream* Stream)
    {
      ast_node_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    bonsai_function b32
    IsValid(ast_node_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    bonsai_function void
    Advance(ast_node_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


        bonsai_function ast_node *
    Push(ast_node_stream* Stream, ast_node Element, memory_arena* Memory)
    {
      ast_node_stream_chunk* NextChunk = (ast_node_stream_chunk*)PushStruct(Memory, sizeof( ast_node_stream_chunk ), 1, 0);
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

      ast_node *Result = &NextChunk->Element;
      return Result;
    }

    bonsai_function void
    ConcatStreams( ast_node_stream *S1, ast_node_stream *S2)
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


