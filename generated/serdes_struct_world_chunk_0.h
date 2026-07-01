// callsite
// src/engine/serdes.cpp:52:0

// def (serdes_struct)
// src/engine/serdes.h:619:0
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
  b32 ThisMember;

    ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing world_chunk *Next on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->Dim, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3i Dim on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
          b64 HadVoxelsPointer = Read_u64(Bytes);
  Assert(HadVoxelsPointer < 2); // Should be 0 or 1




  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing voxel *Voxels on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
          b64 HadVoxelLightingPointer = Read_u64(Bytes);
  Assert(HadVoxelLightingPointer < 2); // Should be 0 or 1




  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing voxel_lighting *VoxelLighting on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing lod_element_buffer Meshes on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing voxel_position_cursor StandingSpots on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->WorldP, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3i WorldP on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing s32 FilledCount on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 DrawBoundingVoxels on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing s32 PointsToLeaveRemaining on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u32 TriCount on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing s32 EdgeBoundaryVoxelCount on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u32 _Pad0 on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing entity_ptr_block_array Entities on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing s32 DEBUG_OwnedByThread on world_chunk_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u8 _Pad1 on world_chunk_0");
  }
  Result &= ThisMember;


        

  
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


