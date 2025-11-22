// callsite
// src/engine/serdes.cpp:417:0

// def (serdes_struct)
// src/engine/serdes.h:593:0
link_internal bonsai_type_info
TypeInfo(noise_layer_1 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("noise_layer_1");
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
Serialize(u8_cursor_block_array *Bytes, noise_layer_1 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    noise_layer_1 *Element = BaseElement + ElementIndex;
                        Result &= Serialize(Bytes, (u32*)&Element->Type); // enum




                            Result &= Serialize(Bytes, &Element->White); // default






                            Result &= Serialize(Bytes, &Element->Perlin); // default






                            Result &= Serialize(Bytes, &Element->Voronoi); // default








            

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, noise_layer_1 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer_1 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer_1 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, noise_layer_1 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, noise_layer_1 *Element, memory_arena *Memory)
{
  b32 Result = True;
            Element->Type = Cast(ui_noise_type, Read_u32(Bytes));




              
  
  Result &= Deserialize(Bytes, &Element->White, Memory);






              
  
  Result &= Deserialize(Bytes, &Element->Perlin, Memory);






              
  
  Result &= Deserialize(Bytes, &Element->Voronoi, Memory);








    
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, noise_layer_1 *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, noise_layer_1 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


