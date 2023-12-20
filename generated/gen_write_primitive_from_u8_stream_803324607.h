link_internal b32
Write(u8_stream *Dest, s8 *Src)
{
  b32 Result = Write(Dest, (u8*)Src, sizeof(s8));
  return Result;
}
link_internal b32
Write(u8_stream *Dest, u8 *Src)
{
  b32 Result = Write(Dest, (u8*)Src, sizeof(u8));
  return Result;
}
link_internal b32
Write(u8_stream *Dest, s16 *Src)
{
  b32 Result = Write(Dest, (u8*)Src, sizeof(s16));
  return Result;
}
link_internal b32
Write(u8_stream *Dest, u16 *Src)
{
  b32 Result = Write(Dest, (u8*)Src, sizeof(u16));
  return Result;
}
link_internal b32
Write(u8_stream *Dest, s32 *Src)
{
  b32 Result = Write(Dest, (u8*)Src, sizeof(s32));
  return Result;
}
link_internal b32
Write(u8_stream *Dest, u32 *Src)
{
  b32 Result = Write(Dest, (u8*)Src, sizeof(u32));
  return Result;
}
link_internal b32
Write(u8_stream *Dest, s64 *Src)
{
  b32 Result = Write(Dest, (u8*)Src, sizeof(s64));
  return Result;
}
link_internal b32
Write(u8_stream *Dest, u64 *Src)
{
  b32 Result = Write(Dest, (u8*)Src, sizeof(u64));
  return Result;
}

