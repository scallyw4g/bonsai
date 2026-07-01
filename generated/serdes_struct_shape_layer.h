// callsite
// src/engine/serdes.cpp:452:0

// def (serdes_struct)
// src/engine/serdes.h:619:0
link_internal bonsai_type_info
TypeInfo(shape_layer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("shape_layer");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, shape_layer *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    shape_layer *Element = BaseElement + ElementIndex;
                            Result &= Serialize(Bytes, (u32*)&Element->Type); // enum





                                Result &= Serialize(Bytes, &Element->Rect); // default







                                Result &= Serialize(Bytes, &Element->Sphere); // default







                                Result &= Serialize(Bytes, &Element->Line); // default







                                Result &= Serialize(Bytes, &Element->Cylinder); // default







                                Result &= Serialize(Bytes, &Element->Plane); // default







                                Result &= Serialize(Bytes, &Element->Torus); // default







                                Result &= Serialize(Bytes, &Element->Pyramid); // default







                                Result &= Serialize(Bytes, &Element->Advanced); // default









            

        

        

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, shape_layer *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, shape_layer *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, shape_layer *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, shape_layer *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, shape_layer *Element, memory_arena *Memory)
{
  b32 Result = True;
  b32 ThisMember;

    ThisMember = 3;
            Element->Type = Cast(brush_shape_type, Read_u32(Bytes));





  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing brush_shape_type Type on shape_layer");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Rect, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing world_update_op_shape_params_rect Rect on shape_layer");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Sphere, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing world_update_op_shape_params_sphere Sphere on shape_layer");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Line, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing world_update_op_shape_params_line Line on shape_layer");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Cylinder, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing world_update_op_shape_params_cylinder Cylinder on shape_layer");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Plane, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing world_update_op_shape_params_plane Plane on shape_layer");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Torus, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing world_update_op_shape_params_torus Torus on shape_layer");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Pyramid, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing world_update_op_shape_params_pyramid Pyramid on shape_layer");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Advanced, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing shape_layer_advanced_params Advanced on shape_layer");
  }
  Result &= ThisMember;


    
  
  
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, shape_layer *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, shape_layer *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


