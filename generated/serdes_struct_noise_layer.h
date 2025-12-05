// callsite
// src/engine/serdes.cpp:424:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(noise_layer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("noise_layer");
  Result.Version =  3 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, noise_layer *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

    Upsert(TypeInfo(BaseElement), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber = 3;
  Serialize(Bytes, &VersionNumber);


  RangeIterator_t(umm, ElementIndex, Count)
  {
    noise_layer *Element = BaseElement + ElementIndex;
                            Result &= Serialize(Bytes, (u32*)&Element->Type); // enum





                                Result &= Serialize(Bytes, &Element->White); // default







                                Result &= Serialize(Bytes, &Element->Perlin); // default







                                Result &= Serialize(Bytes, &Element->Voronoi); // default







                                Result &= Serialize(Bytes, &Element->RectLattice); // default







                                Result &= Serialize(Bytes, &Element->Power); // default









            

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, noise_layer *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, noise_layer *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, noise_layer *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <= 3);

  b32 Result = True;

    if (TypeInfo->Version == 0)
  {
    noise_layer_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }
  if (TypeInfo->Version == 1)
  {
    noise_layer_1 T1 = {};
    Result &= Deserialize(Bytes, &T1, Memory);
    Marshal(&T1, Element);
  }
  if (TypeInfo->Version == 2)
  {
    noise_layer_2 T2 = {};
    Result &= Deserialize(Bytes, &T2, Memory);
    Marshal(&T2, Element);
  }


  if (TypeInfo->Version == 3)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element, Memory);
  }

  return Result;
}


link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, noise_layer *Element, memory_arena *Memory)
{
  b32 Result = True;
              Element->Type = Cast(brush_noise_type, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->White, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Perlin, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Voronoi, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->RectLattice, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Power, Memory);









    
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("noise_layer"));

    if (MaybeSerializedType.Tag)
    {
      u64 OldIgnoredVersionNumber;
      if (MaybeSerializedType.Value.Version > 0)
      {
        Deserialize(Bytes, &OldIgnoredVersionNumber, Memory);
      }
      Result &= DeserializeVersioned(Bytes, Element+ElementIndex, &MaybeSerializedType.Value, Memory);
    }
    else
    {
      bonsai_type_info T0TypeInfo = {};
      Result &= DeserializeVersioned(Bytes, Element+ElementIndex, &T0TypeInfo, Memory);
    }

  }

  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


