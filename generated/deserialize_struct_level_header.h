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
  b32 ThisMember;

    ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->LayerCount, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u32 LayerCount on level_header");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->EditCount, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u32 EditCount on level_header");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->EntityCount, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u32 EntityCount on level_header");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->SelectedLayerIndex, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u32 SelectedLayerIndex on level_header");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->WorldCenter, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3i WorldCenter on level_header");
  }
  Result &= ThisMember;
  ThisMember = 3;
            Element->VisibleRegionSize = Cast(visible_region_size, Read_u32(Bytes));





  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing visible_region_size VisibleRegionSize on level_header");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Camera, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing camera Camera on level_header");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->RenderSettings, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing render_settings RenderSettings on level_header");
  }
  Result &= ThisMember;
  ThisMember = 3;
              {
        umm Count = (256);

    ThisMember = Deserialize(Bytes, Element->TerrainShapingShader, Memory, Count);
  }






  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing char  TerrainShapingShader on level_header");
  }
  Result &= ThisMember;
  ThisMember = 3;
              {
        umm Count = (256);

    ThisMember = Deserialize(Bytes, Element->TerrainDecorationShader, Memory, Count);
  }






  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing char  TerrainDecorationShader on level_header");
  }
  Result &= ThisMember;


    
  
  
  
  
  
  
  
  
  


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

