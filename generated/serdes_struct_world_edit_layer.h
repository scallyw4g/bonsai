// callsite
// src/engine/serdes.cpp:502:0

// def (serdes_struct)
// src/engine/serdes.h:593:0
link_internal bonsai_type_info
TypeInfo(world_edit_layer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_edit_layer");
  Result.Version =  1 ;

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
Serialize(u8_cursor_block_array *Bytes, world_edit_layer *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

    Upsert(TypeInfo(BaseElement), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber = 1;
  Serialize(Bytes, &VersionNumber);


  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_edit_layer *Element = BaseElement + ElementIndex;
                            {
            umm ThisCount = (256) + 1;

      Result &= Serialize(Bytes, Element->NameBuf, ThisCount);
    }





                            Result &= Serialize(Bytes, &Element->EditIndices); // default






                    Result &= Serialize(Bytes, (u32*)&Element->Flags); // enum






            

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_edit_layer *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_layer *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_layer *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit_layer *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, world_edit_layer *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <= 1);

  b32 Result = True;

    if (TypeInfo->Version == 0)
  {
    world_edit_layer_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }


  if (TypeInfo->Version == 1)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element, Memory);
  }

  return Result;
}


link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit_layer *Element, memory_arena *Memory)
{
  b32 Result = True;
              {
        umm Count = (256) + 1;

    Result &= Deserialize(Bytes, Element->NameBuf, Memory, Count);
  }





              
  
  Result &= Deserialize(Bytes, &Element->EditIndices, Memory);






          Element->Flags = Cast(world_edit_layer_flags, Read_u32(Bytes));






    
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_layer *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("world_edit_layer"));

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
Deserialize(u8_cursor *Bytes, world_edit_layer *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


