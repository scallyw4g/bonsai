// callsite
// src/engine/serdes.cpp:52:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(world_chunk_0 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_chunk_0");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_chunk_0 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_chunk_0 *Element = BaseElement + ElementIndex;
            
                                Result &= Serialize(Bytes, &Element->Dim); // default







                    if (Element->Voxels) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                    if (Element->VoxelLighting) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




        
        
                                Result &= Serialize(Bytes, &Element->WorldP); // default







        
        
        
        
        
        
        
        
        


            
        

                if (Element->Voxels) { Result &= Serialize(Bytes, Element->Voxels,  Cast(umm, Volume(Element->Dim)) ); }



                if (Element->VoxelLighting) { Result &= Serialize(Bytes, Element->VoxelLighting,  Cast(umm, Volume(Element->Dim)) ); }



        
        
        

        
        
        
        
        
        
        
        
        


    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_chunk_0 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, world_chunk_0 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, world_chunk_0 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_chunk_0 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_chunk_0 *Element, memory_arena *Memory)
{
  b32 Result = True;
      
                  
  
  Result &= Deserialize(Bytes, &Element->Dim, Memory);








          b64 HadVoxelsPointer = Read_u64(Bytes);
  Assert(HadVoxelsPointer < 2); // Should be 0 or 1




          b64 HadVoxelLightingPointer = Read_u64(Bytes);
  Assert(HadVoxelLightingPointer < 2); // Should be 0 or 1




    
    
                  
  
  Result &= Deserialize(Bytes, &Element->WorldP, Memory);








    
    
    
    
    
    
    
    
    


        

  
      if (HadVoxelsPointer)
  {
        umm Count =  Cast(umm, Volume(Element->Dim));


    if (Element->Voxels == 0)
    {
      Element->Voxels = Allocate(voxel, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->Voxels, Memory, Count);
  }


      if (HadVoxelLightingPointer)
  {
        umm Count =  Cast(umm, Volume(Element->Dim));


    if (Element->VoxelLighting == 0)
    {
      Element->VoxelLighting = Allocate(voxel_lighting, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->VoxelLighting, Memory, Count);
  }


  
  
  
  
  
  
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_chunk_0 *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, world_chunk_0 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


