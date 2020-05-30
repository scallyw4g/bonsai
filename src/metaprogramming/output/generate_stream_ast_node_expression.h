
    
    struct ast_node_expression_stream_chunk
    {
      ast_node_expression Element;
      ast_node_expression_stream_chunk* Next;
    };

    
    struct ast_node_expression_stream
    {
      ast_node_expression_stream_chunk* FirstChunk;
      ast_node_expression_stream_chunk* LastChunk;
    };


    
    struct ast_node_expression_iterator
    {
      ast_node_expression_stream* Stream;
      ast_node_expression_stream_chunk* At;
    };

    function ast_node_expression_iterator
    Iterator(ast_node_expression_stream* Stream)
    {
      ast_node_expression_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(ast_node_expression_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(ast_node_expression_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function ast_node_expression *
    Push(ast_node_expression_stream* Stream, ast_node_expression Element, memory_arena* Memory)
    {
      ast_node_expression_stream_chunk* NextChunk = (ast_node_expression_stream_chunk*)PushStruct(Memory, sizeof( ast_node_expression_stream_chunk ), 1, 0);
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

      ast_node_expression *Result = &NextChunk->Element;
      return Result;
    }

    function void
    ConcatStreams( ast_node_expression_stream *S1, ast_node_expression_stream *S2)
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



