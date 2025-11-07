// callsite
// src/engine/serdes.cpp:447:0

// def (serdes_struct)
// src/engine/serdes.h:563:0
link_internal bonsai_type_info
TypeInfo(brush_settings *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("brush_settings");
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
Serialize(u8_cursor_block_array *Bytes, brush_settings *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    brush_settings *Element = BaseElement + ElementIndex;
                        Result &= Serialize(Bytes, (u32*)&Element->Type); // enum




                            Result &= Serialize(Bytes, &Element->Noise); // default






                            Result &= Serialize(Bytes, &Element->Shape); // default






                if (Element->Brush) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }



                            Result &= Serialize(Bytes, &Element->Power); // default






                            Result &= Serialize(Bytes, &Element->ValueBias); // default






                            Result &= Serialize(Bytes, &Element->Threshold); // default






                    Result &= Serialize(Bytes, (u32*)&Element->ValueModifier); // enum




                    Result &= Serialize(Bytes, (u32*)&Element->BlendMode); // enum




                    Result &= Serialize(Bytes, (u32*)&Element->ColorMode); // enum




                            Result &= Serialize(Bytes, &Element->Invert); // default






                            Result &= Serialize(Bytes, &Element->SelectionModifier); // default






                            Result &= Serialize(Bytes, &Element->BasisOffset); // default






                            Result &= Serialize(Bytes, &Element->HSVColor); // default








            

        

        

                if (Element->Brush) { Result &= Serialize(Bytes, Element->Brush); }



        

        

        

        

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, brush_settings *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, brush_settings *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, brush_settings *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, brush_settings *Element, memory_arena *Memory)
{
  b32 Result = True;
            Element->Type = Cast(brush_layer_type, Read_u32(Bytes));




              
  
  Result &= Deserialize(Bytes, &Element->Noise, Memory);






              
  
  Result &= Deserialize(Bytes, &Element->Shape, Memory);






        b64 HadBrushPointer = Read_u64(Bytes);
  Assert(HadBrushPointer < 2); // Should be 0 or 1



              
  
  Result &= Deserialize(Bytes, &Element->Power, Memory);






              
  
  Result &= Deserialize(Bytes, &Element->ValueBias, Memory);






              
  
  Result &= Deserialize(Bytes, &Element->Threshold, Memory);






          Element->ValueModifier = Cast(world_edit_blend_mode_modifier, Read_u32(Bytes));




          Element->BlendMode = Cast(world_edit_blend_mode, Read_u32(Bytes));




          Element->ColorMode = Cast(world_edit_color_blend_mode, Read_u32(Bytes));




              
  
  Result &= Deserialize(Bytes, &Element->Invert, Memory);






              
  
  Result &= Deserialize(Bytes, &Element->SelectionModifier, Memory);






                
  
  Result &= Deserialize(Bytes, &Element->BasisOffset, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->HSVColor, Memory);









    
  
  
      if (HadBrushPointer)
  {
        umm Count = 1;


    if (Element->Brush == 0)
    {
      Element->Brush = Allocate(world_edit_brush, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->Brush, Memory, Count);
  }


  
  
  
  
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, brush_settings *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


