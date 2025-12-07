// callsite
// src/engine/serdes.cpp:298:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(vox_data *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("vox_data");
  Result.Version =  0 ;

  
  
  
  
  
  
  

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
                                    Result &= Serialize(Bytes, &Element->FilledCount); // default







                    if (Element->ChunkData) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                    if (Element->Voxels) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }






            

                if (Element->ChunkData) { Result &= Serialize(Bytes, Element->ChunkData); }



                if (Element->Voxels) { Result &= Serialize(Bytes, Element->Voxels); }





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
Deserialize(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->FilledCount, Memory);







          b64 HadChunkDataPointer = Read_u64(Bytes);
  Assert(HadChunkDataPointer < 2); // Should be 0 or 1




          b64 HadVoxelsPointer = Read_u64(Bytes);
  Assert(HadVoxelsPointer < 2); // Should be 0 or 1






    
      if (HadChunkDataPointer)
  {
        umm Count = 1;


    if (Element->ChunkData == 0)
    {
      Element->ChunkData = Allocate(chunk_data, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->ChunkData, Memory, Count);
  }


      if (HadVoxelsPointer)
  {
        umm Count = 1;


    if (Element->Voxels == 0)
    {
      Element->Voxels = Allocate(voxel, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->Voxels, Memory, Count);
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

link_internal b32
Deserialize(u8_cursor *Bytes, vox_data *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


