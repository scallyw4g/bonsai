// callsite
// external/bonsai_stdlib/src/binary_parser.cpp:513:0

// def (gen_read_primitive_from_u8_cursor_little_endian)
// external/bonsai_stdlib/src/binary_parser.cpp:449:0
link_internal s8
Read_s8(u8 *Source)
{
  s8 Result = *(s8*)Source;
  return Result;
}

link_internal s8
Read_s8(s8 *Source)
{
  s8 Result = *Source;
  return Result;
}

link_internal s8
Read_s8(u8_stream *Source)
{
  s8 Result = Read_s8(Source->At);
  Source->At += sizeof(s8);
  Assert(Source->At <= Source->End);
  return Result;
}

link_internal s8*
ReadArray_s8(u8_stream *Source, u32 Count)
{
  s8 *Result = (s8*)Source->At;
  Source->At += (Count * sizeof(s8));
  Assert(Source->At <= Source->End);
  return Result;
}
link_internal s16
Read_s16(u8 *Source)
{
  s16 Result = *(s16*)Source;
  return Result;
}

link_internal s16
Read_s16(s16 *Source)
{
  s16 Result = *Source;
  return Result;
}

link_internal s16
Read_s16(u8_stream *Source)
{
  s16 Result = Read_s16(Source->At);
  Source->At += sizeof(s16);
  Assert(Source->At <= Source->End);
  return Result;
}

link_internal s16*
ReadArray_s16(u8_stream *Source, u32 Count)
{
  s16 *Result = (s16*)Source->At;
  Source->At += (Count * sizeof(s16));
  Assert(Source->At <= Source->End);
  return Result;
}
link_internal u16
Read_u16(u8 *Source)
{
  u16 Result = *(u16*)Source;
  return Result;
}

link_internal u16
Read_u16(u16 *Source)
{
  u16 Result = *Source;
  return Result;
}

link_internal u16
Read_u16(u8_stream *Source)
{
  u16 Result = Read_u16(Source->At);
  Source->At += sizeof(u16);
  Assert(Source->At <= Source->End);
  return Result;
}

link_internal u16*
ReadArray_u16(u8_stream *Source, u32 Count)
{
  u16 *Result = (u16*)Source->At;
  Source->At += (Count * sizeof(u16));
  Assert(Source->At <= Source->End);
  return Result;
}
link_internal s32
Read_s32(u8 *Source)
{
  s32 Result = *(s32*)Source;
  return Result;
}

link_internal s32
Read_s32(s32 *Source)
{
  s32 Result = *Source;
  return Result;
}

link_internal s32
Read_s32(u8_stream *Source)
{
  s32 Result = Read_s32(Source->At);
  Source->At += sizeof(s32);
  Assert(Source->At <= Source->End);
  return Result;
}

link_internal s32*
ReadArray_s32(u8_stream *Source, u32 Count)
{
  s32 *Result = (s32*)Source->At;
  Source->At += (Count * sizeof(s32));
  Assert(Source->At <= Source->End);
  return Result;
}
link_internal u32
Read_u32(u8 *Source)
{
  u32 Result = *(u32*)Source;
  return Result;
}

link_internal u32
Read_u32(u32 *Source)
{
  u32 Result = *Source;
  return Result;
}

link_internal u32
Read_u32(u8_stream *Source)
{
  u32 Result = Read_u32(Source->At);
  Source->At += sizeof(u32);
  Assert(Source->At <= Source->End);
  return Result;
}

link_internal u32*
ReadArray_u32(u8_stream *Source, u32 Count)
{
  u32 *Result = (u32*)Source->At;
  Source->At += (Count * sizeof(u32));
  Assert(Source->At <= Source->End);
  return Result;
}
link_internal s64
Read_s64(u8 *Source)
{
  s64 Result = *(s64*)Source;
  return Result;
}

link_internal s64
Read_s64(s64 *Source)
{
  s64 Result = *Source;
  return Result;
}

link_internal s64
Read_s64(u8_stream *Source)
{
  s64 Result = Read_s64(Source->At);
  Source->At += sizeof(s64);
  Assert(Source->At <= Source->End);
  return Result;
}

link_internal s64*
ReadArray_s64(u8_stream *Source, u32 Count)
{
  s64 *Result = (s64*)Source->At;
  Source->At += (Count * sizeof(s64));
  Assert(Source->At <= Source->End);
  return Result;
}
link_internal u64
Read_u64(u8 *Source)
{
  u64 Result = *(u64*)Source;
  return Result;
}

link_internal u64
Read_u64(u64 *Source)
{
  u64 Result = *Source;
  return Result;
}

link_internal u64
Read_u64(u8_stream *Source)
{
  u64 Result = Read_u64(Source->At);
  Source->At += sizeof(u64);
  Assert(Source->At <= Source->End);
  return Result;
}

link_internal u64*
ReadArray_u64(u8_stream *Source, u32 Count)
{
  u64 *Result = (u64*)Source->At;
  Source->At += (Count * sizeof(u64));
  Assert(Source->At <= Source->End);
  return Result;
}


