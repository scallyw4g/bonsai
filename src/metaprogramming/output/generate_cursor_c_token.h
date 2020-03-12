
struct c_token_cursor
{
  c_token* Start;
  c_token* End;
  c_token* At;
};

function c_token_cursor
CTokenCursor(umm ElementCount, memory_arena* Memory)
{
  // TODO(Jesse, id: 162, tags: metaprogramming): Can we use Allocate() here instead?
  c_token* Start = (c_token*)PushStruct(Memory, sizeof(c_token), 1, 1);
  c_token_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
};

