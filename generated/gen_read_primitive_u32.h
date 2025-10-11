// external/bonsai_stdlib/src/binary_parser.cpp:572:0

link_internal bool
Read_u32(u8_cursor *Cursor, u32 *Dest)
{
  b32 Result = False;
  u8 *Bytes = {};
  if (Read_bytes(Cursor, &Bytes, sizeof(u32)))
  {
    *Dest = *(u32*)Bytes;
    Result = True;
  }
  return Result;
}

