
struct c_token_cursor;
struct c_token_cursor_up
{
  c_token *Start;
  c_token *End;
  c_token *At;
};

    struct c_token_cursor
    {
      token_cursor_source Source;

      c_token* Start;
      c_token* End;
      c_token* At;

      c_token_cursor_up Up;

      counted_string Filename;
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

