// callsite
// src/engine/serdes.cpp:388:0

// def (deserialize_struct)
// src/engine/serdes.h:312:0
link_internal b32
Deserialize(u8_cursor *Bytes, level_header *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, level_header *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, level_header *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, level_header *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->LayerCount, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->EditCount, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->EntityCount, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->SelectedLayerIndex, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->WorldCenter, Memory);








            Element->VisibleRegionSize = Cast(visible_region_size, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Camera, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->RenderSettings, Memory);







              {
        umm Count = (256);

    Result &= Deserialize(Bytes, Element->TerrainShapingShader, Memory, Count);
  }






              {
        umm Count = (256);

    Result &= Deserialize(Bytes, Element->TerrainDecorationShader, Memory, Count);
  }








    
  
  
  
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, level_header *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, level_header *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}

