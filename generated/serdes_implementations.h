// callsite
// src/engine/serdes.cpp:500:0

// def ((builtin.for_datatypes))
// src/engine/serdes.cpp:500:0


























link_internal bonsai_type_info
TypeInfo(layer_settings *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("layer_settings");
  Result.Version =  2 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, layer_settings *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

    Upsert(TypeInfo(BaseElement), &Global_SerializeTypeTable, Global_SerializeTypeTableArena );
  u64 VersionNumber = 2;
  Serialize(Bytes, &VersionNumber);


  RangeIterator_t(umm, ElementIndex, Count)
  {
    layer_settings *Element = BaseElement + ElementIndex;
                            Result &= Serialize(Bytes, (u32*)&Element->Type); // enum





                                Result &= Serialize(Bytes, &Element->Noise); // default







                                Result &= Serialize(Bytes, &Element->Shape); // default







                    if (Element->Brush) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                                Result &= Serialize(Bytes, &Element->Offset); // default







                                Result &= Serialize(Bytes, &Element->Rotation); // default







                                Result &= Serialize(Bytes, &Element->Invert); // default







                                Result &= Serialize(Bytes, &Element->Normalized); // default







                            {
            umm ThisCount = 2;

      Result &= Serialize(Bytes, Element->Reserved, ThisCount);
    }






                                Result &= Serialize(Bytes, &Element->ValueBias); // default







                                Result &= Serialize(Bytes, &Element->Power); // default







                        Result &= Serialize(Bytes, (u32*)&Element->ValueFunc); // enum





                        Result &= Serialize(Bytes, (u32*)&Element->BlendMode); // enum





                                Result &= Serialize(Bytes, &Element->Smoothing); // default







                        Result &= Serialize(Bytes, (u32*)&Element->ColorMode); // enum





                                Result &= Serialize(Bytes, &Element->BasisOffset); // default







                                Result &= Serialize(Bytes, &Element->HSVColor); // default







                                Result &= Serialize(Bytes, &Element->Disabled); // default









            

        

        

                if (Element->Brush) { Result &= Serialize(Bytes, Element->Brush); }



        

        

        

        

        

        

        

        

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, layer_settings *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, layer_settings *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, layer_settings *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layer_settings *Element, memory_arena *Memory);


link_internal b32
DeserializeVersioned(u8_cursor *Bytes, layer_settings *Element, bonsai_type_info *TypeInfo, memory_arena *Memory)
{
  Assert(TypeInfo->Version <= 2);

  b32 Result = True;

    if (TypeInfo->Version == 0)
  {
    layer_settings_0 T0 = {};
    Result &= Deserialize(Bytes, &T0, Memory);
    Marshal(&T0, Element);
  }
  if (TypeInfo->Version == 1)
  {
    layer_settings_1 T1 = {};
    Result &= Deserialize(Bytes, &T1, Memory);
    Marshal(&T1, Element);
  }


  if (TypeInfo->Version == 2)
  {
    Result &= DeserializeCurrentVersion(Bytes, Element, Memory);
  }

  return Result;
}


link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layer_settings *Element, memory_arena *Memory)
{
  b32 Result = True;
              Element->Type = Cast(brush_layer_type, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Noise, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Shape, Memory);







          b64 HadBrushPointer = Read_u64(Bytes);
  Assert(HadBrushPointer < 2); // Should be 0 or 1




                  
  
  Result &= Deserialize(Bytes, &Element->Offset, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->Rotation, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Invert, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Normalized, Memory);







              {
        umm Count = 2;

    Result &= Deserialize(Bytes, Element->Reserved, Memory, Count);
  }






                
  
  Result &= Deserialize(Bytes, &Element->ValueBias, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Power, Memory);







            Element->ValueFunc = Cast(world_edit_blend_mode_modifier, Read_u32(Bytes));





            Element->BlendMode = Cast(world_edit_blend_mode, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Smoothing, Memory);







            Element->ColorMode = Cast(world_edit_color_blend_mode, Read_u32(Bytes));





                  
  
  Result &= Deserialize(Bytes, &Element->BasisOffset, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->HSVColor, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Disabled, Memory);









    
  
  
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
Deserialize(u8_cursor *Bytes, layer_settings *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        maybe_bonsai_type_info MaybeSerializedType = GetByName(&Global_SerializeTypeTable, CSz("layer_settings"));

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
Deserialize(u8_cursor *Bytes, layer_settings *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}



































































































/* serdes_collection(type) */
















link_internal bonsai_type_info
TypeInfo(prefab *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("prefab");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, prefab *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    prefab *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Name); // default







                                Result &= Serialize(Bytes, &Element->Edits); // default







            auto Thing = ToString(Element->SpawnCallback);
    Result &= Serialize(Bytes, &Thing );




            

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, prefab *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, prefab *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, prefab *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, prefab *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, prefab *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->Name, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Edits, Memory);







      {
    cs Thing = {};
    Result &= Deserialize(Bytes, &Thing, Memory);
    Element->SpawnCallback = PrefabSpawnCallback(Thing);
  }




    
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, prefab *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, prefab *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}





































/* serdes_collection(type) */




























































link_internal bonsai_type_info
TypeInfo(smooth_blend_params *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("smooth_blend_params");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, smooth_blend_params *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    smooth_blend_params *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->ValueBlend); // default







                                Result &= Serialize(Bytes, &Element->ColorBlend); // default









            

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, smooth_blend_params *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, smooth_blend_params *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, smooth_blend_params *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, smooth_blend_params *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, smooth_blend_params *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->ValueBlend, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->ColorBlend, Memory);









    
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, smooth_blend_params *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, smooth_blend_params *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}



link_internal bonsai_type_info
TypeInfo(world_edit *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_edit");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_edit *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_edit *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Region); // default







                    if (Element->Brush) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                                Result &= Serialize(Bytes, &Element->Rotation); // default







                                Result &= Serialize(Bytes, &Element->Flags); // default







                                Result &= Serialize(Bytes, &Element->Dirty); // default







                                Result &= Serialize(Bytes, &Element->Selected); // default







                                Result &= Serialize(Bytes, &Element->Ordinal); // default









            

                if (Element->Brush) { Result &= Serialize(Bytes, Element->Brush); }



        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_edit *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->Region, Memory);







          b64 HadBrushPointer = Read_u64(Bytes);
  Assert(HadBrushPointer < 2); // Should be 0 or 1




                  
  
  Result &= Deserialize(Bytes, &Element->Rotation, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Flags, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Dirty, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Selected, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Ordinal, Memory);









    
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
Deserialize(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, world_edit *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}



























































































/* serdes_collection(type) */


















link_internal bonsai_type_info
TypeInfo(world_edit_brush *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_edit_brush");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_edit_brush *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_edit_brush *Element = BaseElement + ElementIndex;
                                {
            umm ThisCount = (256) + 1;

      Result &= Serialize(Bytes, Element->NameBuf, ThisCount);
    }






                        Result &= Serialize(Bytes, (u32*)&Element->BrushBlendMode); // enum





                        Result &= Serialize(Bytes, (u32*)&Element->Modifier); // enum





                                Result &= Serialize(Bytes, &Element->Smoothing); // default







                                Result &= Serialize(Bytes, &Element->LayerCount); // default







                            {
            
      umm ThisCount = umm(Element->LayerCount);

      Result &= Serialize(Bytes, Element->Layers, ThisCount);
    }








            

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_edit_brush *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory)
{
  b32 Result = True;
                {
        umm Count = (256) + 1;

    Result &= Deserialize(Bytes, Element->NameBuf, Memory, Count);
  }






            Element->BrushBlendMode = Cast(world_edit_blend_mode, Read_u32(Bytes));





            Element->Modifier = Cast(world_edit_blend_mode_modifier, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Smoothing, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->LayerCount, Memory);







              {
        // TODO(Jesse): Should this really be a safe cast?
    umm Count = umm(Element->LayerCount);

    Result &= Deserialize(Bytes, Element->Layers, Memory, Count);
  }








    
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, world_edit_brush *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}
















link_internal bonsai_type_info
TypeInfo(layer_settings_1 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("layer_settings_1");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, layer_settings_1 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    layer_settings_1 *Element = BaseElement + ElementIndex;
                            Result &= Serialize(Bytes, (u32*)&Element->Type); // enum





                                Result &= Serialize(Bytes, &Element->Noise); // default







                                Result &= Serialize(Bytes, &Element->Shape); // default







                    if (Element->Brush) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                                Result &= Serialize(Bytes, &Element->Invert); // default







                                Result &= Serialize(Bytes, &Element->Normalized); // default







                            {
            umm ThisCount = 2;

      Result &= Serialize(Bytes, Element->Reserved, ThisCount);
    }






                                Result &= Serialize(Bytes, &Element->ValueBias); // default







                                Result &= Serialize(Bytes, &Element->Power); // default







                        Result &= Serialize(Bytes, (u32*)&Element->ValueFunc); // enum





                        Result &= Serialize(Bytes, (u32*)&Element->BlendMode); // enum





                                Result &= Serialize(Bytes, &Element->Smoothing); // default







                        Result &= Serialize(Bytes, (u32*)&Element->ColorMode); // enum





                                Result &= Serialize(Bytes, &Element->BasisOffset); // default







                                Result &= Serialize(Bytes, &Element->HSVColor); // default







                                Result &= Serialize(Bytes, &Element->Disabled); // default









            

        

        

                if (Element->Brush) { Result &= Serialize(Bytes, Element->Brush); }



        

        

        

        

        

        

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, layer_settings_1 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, layer_settings_1 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, layer_settings_1 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layer_settings_1 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layer_settings_1 *Element, memory_arena *Memory)
{
  b32 Result = True;
              Element->Type = Cast(brush_layer_type, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Noise, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Shape, Memory);







          b64 HadBrushPointer = Read_u64(Bytes);
  Assert(HadBrushPointer < 2); // Should be 0 or 1




                
  
  Result &= Deserialize(Bytes, &Element->Invert, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Normalized, Memory);







              {
        umm Count = 2;

    Result &= Deserialize(Bytes, Element->Reserved, Memory, Count);
  }






                
  
  Result &= Deserialize(Bytes, &Element->ValueBias, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Power, Memory);







            Element->ValueFunc = Cast(world_edit_blend_mode_modifier, Read_u32(Bytes));





            Element->BlendMode = Cast(world_edit_blend_mode, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Smoothing, Memory);







            Element->ColorMode = Cast(world_edit_color_blend_mode, Read_u32(Bytes));





                  
  
  Result &= Deserialize(Bytes, &Element->BasisOffset, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->HSVColor, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Disabled, Memory);









    
  
  
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
Deserialize(u8_cursor *Bytes, layer_settings_1 *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, layer_settings_1 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}






































/* serdes_collection(type) */























link_internal bonsai_type_info
TypeInfo(world_edit_layer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_edit_layer");
  Result.Version =  1 ;

  
  
  
  
  
  
  

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







                                Result &= Serialize(Bytes, &Element->Flags); // default









            

        

        



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







                
  
  Result &= Deserialize(Bytes, &Element->Flags, Memory);









    
  
  


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



link_internal bonsai_type_info
TypeInfo(world_edit_layer_0 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("world_edit_layer_0");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_edit_layer_0 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    world_edit_layer_0 *Element = BaseElement + ElementIndex;
                                {
            umm ThisCount = (256) + 1;

      Result &= Serialize(Bytes, Element->NameBuf, ThisCount);
    }






                                Result &= Serialize(Bytes, &Element->EditIndices); // default









            

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, world_edit_layer_0 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_layer_0 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_layer_0 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit_layer_0 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, world_edit_layer_0 *Element, memory_arena *Memory)
{
  b32 Result = True;
                {
        umm Count = (256) + 1;

    Result &= Deserialize(Bytes, Element->NameBuf, Memory, Count);
  }






                
  
  Result &= Deserialize(Bytes, &Element->EditIndices, Memory);









    
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, world_edit_layer_0 *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, world_edit_layer_0 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}
















































































link_internal bonsai_type_info
TypeInfo(brush_layer *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("brush_layer");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, brush_layer *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    brush_layer *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->Settings); // default







        


            

        


    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, brush_layer *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, brush_layer *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, brush_layer *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, brush_layer *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, brush_layer *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->Settings, Memory);







    


    
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, brush_layer *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, brush_layer *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}































































link_internal bonsai_type_info
TypeInfo(layer_settings_0 *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("layer_settings_0");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, layer_settings_0 *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    layer_settings_0 *Element = BaseElement + ElementIndex;
                            Result &= Serialize(Bytes, (u32*)&Element->Type); // enum





                                Result &= Serialize(Bytes, &Element->Noise); // default







                                Result &= Serialize(Bytes, &Element->Shape); // default







                    if (Element->Brush) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }




                                Result &= Serialize(Bytes, &Element->Invert); // default







                                Result &= Serialize(Bytes, &Element->Normalized); // default







                            {
            umm ThisCount = 2;

      Result &= Serialize(Bytes, Element->Reserved, ThisCount);
    }






                                Result &= Serialize(Bytes, &Element->ValueBias); // default







                        Result &= Serialize(Bytes, (u32*)&Element->ValueFunc); // enum





                        Result &= Serialize(Bytes, (u32*)&Element->BlendMode); // enum





                                Result &= Serialize(Bytes, &Element->Smoothing); // default







                        Result &= Serialize(Bytes, (u32*)&Element->ColorMode); // enum





                                Result &= Serialize(Bytes, &Element->BasisOffset); // default







                                Result &= Serialize(Bytes, &Element->HSVColor); // default







                                Result &= Serialize(Bytes, &Element->Disabled); // default









            

        

        

                if (Element->Brush) { Result &= Serialize(Bytes, Element->Brush); }



        

        

        

        

        

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, layer_settings_0 *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, layer_settings_0 *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, layer_settings_0 *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layer_settings_0 *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, layer_settings_0 *Element, memory_arena *Memory)
{
  b32 Result = True;
              Element->Type = Cast(brush_layer_type, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Noise, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Shape, Memory);







          b64 HadBrushPointer = Read_u64(Bytes);
  Assert(HadBrushPointer < 2); // Should be 0 or 1




                
  
  Result &= Deserialize(Bytes, &Element->Invert, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Normalized, Memory);







              {
        umm Count = 2;

    Result &= Deserialize(Bytes, Element->Reserved, Memory, Count);
  }






                
  
  Result &= Deserialize(Bytes, &Element->ValueBias, Memory);







            Element->ValueFunc = Cast(world_edit_blend_mode_modifier, Read_u32(Bytes));





            Element->BlendMode = Cast(world_edit_blend_mode, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->Smoothing, Memory);







            Element->ColorMode = Cast(world_edit_color_blend_mode, Read_u32(Bytes));





                  
  
  Result &= Deserialize(Bytes, &Element->BasisOffset, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->HSVColor, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Disabled, Memory);









    
  
  
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
Deserialize(u8_cursor *Bytes, layer_settings_0 *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, layer_settings_0 *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}















































