// callsite
// external/bonsai_stdlib/src/binary_parser.cpp:441:0

// def (gen_read_primitive_from_native_file)
// external/bonsai_stdlib/src/binary_parser.cpp:426:0
link_internal s8
Read_s8(native_file *File)
{
  s8 Result;
  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result, sizeof(s8)) );
  return Result;
}
link_internal u8
Read_u8(native_file *File)
{
  u8 Result;
  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result, sizeof(u8)) );
  return Result;
}
link_internal s16
Read_s16(native_file *File)
{
  s16 Result;
  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result, sizeof(s16)) );
  return Result;
}
link_internal u16
Read_u16(native_file *File)
{
  u16 Result;
  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result, sizeof(u16)) );
  return Result;
}
link_internal s32
Read_s32(native_file *File)
{
  s32 Result;
  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result, sizeof(s32)) );
  return Result;
}
link_internal u32
Read_u32(native_file *File)
{
  u32 Result;
  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result, sizeof(u32)) );
  return Result;
}
link_internal s64
Read_s64(native_file *File)
{
  s64 Result;
  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result, sizeof(s64)) );
  return Result;
}
link_internal u64
Read_u64(native_file *File)
{
  u64 Result;
  Ensure( ReadBytesIntoBuffer(File, (u8*)&Result, sizeof(u64)) );
  return Result;
}


