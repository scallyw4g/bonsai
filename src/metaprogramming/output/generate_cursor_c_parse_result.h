
struct c_parse_result_cursor
{
  c_parse_result* Start;
  c_parse_result* End;
  c_parse_result* At;
};

function c_parse_result_cursor
CParseResultCursor(umm ElementCount, memory_arena* Memory)
{
  // TODO(Jesse, id: 171, tags: metaprogramming): Can we use Allocate() here instead?
  c_parse_result* Start = (c_parse_result*)PushStruct(Memory, sizeof(c_parse_result), 1, 1);
  c_parse_result_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
};

