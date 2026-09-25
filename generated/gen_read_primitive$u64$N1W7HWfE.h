// callsite
// external/bonsai_stdlib/src/binary_parser.cpp:575:0

// def (gen_read_primitive)
// external/bonsai_stdlib/src/binary_parser.cpp:555:0
link_internal bool
Read_u64(u8_cursor *Cursor, u64 *Dest)
{
  b32 Result = False;
  u8 *Bytes = {};
  if (Read_bytes(Cursor, &Bytes, sizeof(u64)))
  {
    *Dest = *(u64*)Bytes;
    Result = True;
  }
  return Result;
}

