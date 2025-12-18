// callsite
// src/engine/serdes.cpp:63:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(world_chunk *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_chunk");
  Result.Version =  1 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_chunk *BaseElement, umm Count)
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
    world_chunk *Element = BaseElement + ElementIndex;
            
                                Result &= Serialize(Bytes, &Element->CollisionVolume); // default







                                Result &= Serialize(Bytes, &Element->Dim); // default







                    if (Element->Occupancy) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                    if (Element->xOccupancyBorder) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                    if (Element->FaceMasks) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                                Result &= Serialize(Bytes, &Element->IsOnFreelist); // default







        
                                Result &= Serialize(Bytes, &Element->OcclusionQueryId); // default







                                Result &= Serialize(Bytes, &Element->QueryActive); // default







                                Result &= Serialize(Bytes, &Element->OcclusionFrames); // default







        
                                Result &= Serialize(Bytes, &Element->DimInChunks); // default







                                Result &= Serialize(Bytes, &Element->WorldP); // default







        
        


            
        

        

                if (Element->Occupancy) { Result &= Serialize(Bytes, Element->Occupancy); }



                if (Element->xOccupancyBorder) { Result &= Serialize(Bytes, Element->xOccupancyBorder); }



                if (Element->FaceMasks) { Result &= Serialize(Bytes, Element->FaceMasks); }



        

        
        

        

        

        
        

        

        
        


    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_chunk *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, world_chunk *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <= 1);

  b32 Result = True;

    if (TypeInfo->Version == 0)
  {
    world_chunk_0 T0 = {};
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
DeserializeCurrentVersion(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory)
{
  b32 Result = True;
      
                  
  
  Result &= Deserialize(Bytes, &Element->CollisionVolume, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->Dim, Memory);








          b64 HadOccupancyPointer = Read_u64(Bytes);
  Assert(HadOccupancyPointer < 2); // Should be 0 or 1




          b64 HadxOccupancyBorderPointer = Read_u64(Bytes);
  Assert(HadxOccupancyBorderPointer < 2); // Should be 0 or 1




          b64 HadFaceMasksPointer = Read_u64(Bytes);
  Assert(HadFaceMasksPointer < 2); // Should be 0 or 1




                
  
  Result &= Deserialize(Bytes, &Element->IsOnFreelist, Memory);







    
                
  
  Result &= Deserialize(Bytes, &Element->OcclusionQueryId, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->QueryActive, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->OcclusionFrames, Memory);







    
                  
  
  Result &= Deserialize(Bytes, &Element->DimInChunks, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->WorldP, Memory);








    
    


        

  
  
      if (HadOccupancyPointer)
  {
        umm Count = 1;


    if (Element->Occupancy == 0)
    {
      Element->Occupancy = Allocate(u64, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->Occupancy, Memory, Count);
  }


      if (HadxOccupancyBorderPointer)
  {
        umm Count = 1;


    if (Element->xOccupancyBorder == 0)
    {
      Element->xOccupancyBorder = Allocate(u64, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->xOccupancyBorder, Memory, Count);
  }


      if (HadFaceMasksPointer)
  {
        umm Count = 1;


    if (Element->FaceMasks == 0)
    {
      Element->FaceMasks = Allocate(u64, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->FaceMasks, Memory, Count);
  }


  
  
  
  
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("world_chunk"));

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
Deserialize(u8_cursor *Bytes, world_chunk *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


