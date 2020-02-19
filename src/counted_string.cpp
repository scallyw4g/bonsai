function counted_string
CountedString(ansi_stream Stream)
{
  counted_string Result = {
    .Start = Stream.Start,
    .Count = (umm)(Stream.End-Stream.Start)
  };
  return Result;
}

r32
StringToFloat(counted_string* String)
{
  const char* Temp = FormatString(TranArena, "%.*s", String->Count, String->Start);
  r32 Result = (r32)atof(Temp);
  return Result;
}

s32
StringToInt(counted_string String)
{
  const char* Temp = FormatString(TranArena, "%.*s", String.Count,  String.Start);
  s32 Result = atoi(Temp);
  return Result;
}
s32
StringToInt(counted_string* String)
{
  s32 Result = StringToInt(*String);
  return Result;
}

u32
StringToUInt(counted_string *String)
{
  u32 Result = (u32)StringToInt(String);
  return Result;
}

