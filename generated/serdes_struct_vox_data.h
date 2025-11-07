// src/engine/serdes.h:563:0
link_internal bonsai_type_info
TypeInfo(vox_data *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("vox_data");
  Result.Version =  0 ;

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
Serialize(u8_cursor_block_array *Bytes, vox_data *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    vox_data *Element = BaseElement + ElementIndex;
                    if (Element->ChunkData) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }





                    if (Element->ChunkData) { Result &= Serialize(Bytes, Element->ChunkData); }





    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, vox_data *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory)
{
  b32 Result = True;
          b64 HadChunkDataPointer = Read_u64(Bytes);
  Assert(HadChunkDataPointer < 2); // Should be 0 or 1





        if (HadChunkDataPointer)
  {
        umm Count = 1;


    if (Element->ChunkData == 0)
    {
      Element->ChunkData = Allocate(chunk_data, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->ChunkData, Memory, Count);
  }




  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


