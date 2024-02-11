// external/bonsai_stdlib/src/counted_string.cpp:730:0

link_internal cs
ToHex( s64 Value )
{
  cs Result = FSz("%lx", Value);
  return Result;
}
link_internal cs
ToHex( u64 Value )
{
  cs Result = FSz("%lx", Value);
  return Result;
}
link_internal cs
ToHex( r64 Value )
{
  cs Result = FSz("%lx", Value);
  return Result;
}

