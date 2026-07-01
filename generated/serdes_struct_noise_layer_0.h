// callsite
// src/engine/serdes.cpp:418:0

// def (serdes_struct)
// src/engine/serdes.h:619:0
link_internal bonsai_type_info
TypeInfo(noise_layer_0 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("noise_layer_0");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, noise_layer_0 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    noise_layer_0 *Element = BaseElement + ElementIndex;
                            Result &= Serialize(Bytes, (u32*)&Element->Type); // enum





                                Result &= Serialize(Bytes, &Element->Perlin); // default







                                Result &= Serialize(Bytes, &Element->Voronoi); // default









            

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, noise_layer_0 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer_0 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer_0 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, noise_layer_0 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, noise_layer_0 *Element, memory_arena *Memory)
{
  b32 Result = True;
  b32 ThisMember;

    ThisMember = 3;
            Element->Type = Cast(brush_noise_type, Read_u32(Bytes));





  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing brush_noise_type Type on noise_layer_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Perlin, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing perlin_noise_params Perlin on noise_layer_0");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Voronoi, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing voronoi_noise_params Voronoi on noise_layer_0");
  }
  Result &= ThisMember;


    
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer_0 *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, noise_layer_0 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


