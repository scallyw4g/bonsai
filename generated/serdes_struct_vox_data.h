// src/engine/serdes.cpp:0:235

link_internal bonsai_type_info
TypeInfo(vox_data *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("vox_data");
  Result.Version = 0 ;

  /* type.map(member) */
  /* { */
  /*   { */
  /*     member_info Member = {CSz("member.name"), CSz("member.name"), 0x(member.hash)}; */
  /*     Push(&Result.Members, &Member); */
  /*   } */
  /* } */

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, vox_data *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  if (Element->ChunkData) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
  else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }

  if (Element->ChunkData) { Result &= Serialize(Bytes, Element->ChunkData); }

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory)
{
  b32 Result = True;
  b64 HadChunkDataPointer = Read_u64(Bytes);
  Assert(HadChunkDataPointer < 2); // Should be 0 or 1

  if (HadChunkDataPointer)
  {
    if (Element->ChunkData == 0) { Element->ChunkData = Allocate(chunk_data, Memory, 1); }
    Result &= Deserialize(Bytes, Element->ChunkData, Memory);
  }
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


