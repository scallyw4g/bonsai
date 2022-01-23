
    struct c_token_cursor
    {
      c_token* Start;
      c_token* End;
      c_token* At;

      c_token_cursor *Next;
      c_token_cursor *Prev;

      counted_string Filename;

      token_cursor_source Source;
      u32 StartLine;
      u32 EndLine;
    };

    bonsai_function c_token_cursor
    CTokenCursor(umm ElementCount, memory_arena* Memory)
    {
      c_token* Start = (c_token*)PushStruct(Memory, sizeof( c_token ), 1, 0);
      c_token_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    }

