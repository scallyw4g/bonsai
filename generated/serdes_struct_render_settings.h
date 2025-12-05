// callsite
// src/engine/serdes.cpp:380:0

// def (serdes_struct)
// src/engine/serdes.h:610:0
link_internal bonsai_type_info
TypeInfo(render_settings *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("render_settings");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, render_settings *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    render_settings *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->UseSsao); // default







                                Result &= Serialize(Bytes, &Element->UseShadowMapping); // default







                                Result &= Serialize(Bytes, &Element->UseLightingBloom); // default







                                Result &= Serialize(Bytes, &Element->BravoilMyersOIT); // default







                                Result &= Serialize(Bytes, &Element->BravoilMcGuireOIT); // default







                                Result &= Serialize(Bytes, &Element->DrawMajorGrid); // default







                                Result &= Serialize(Bytes, &Element->DrawMinorGrid); // default







                                Result &= Serialize(Bytes, &Element->MajorGridDim); // default







                                Result &= Serialize(Bytes, &Element->DrawCameraGhost); // default







                                Result &= Serialize(Bytes, &Element->CameraGhostSize); // default







                                Result &= Serialize(Bytes, &Element->Ignored); // default







                                Result &= Serialize(Bytes, &Element->Headless); // default







                        Result &= Serialize(Bytes, (u32*)&Element->ToneMappingType); // enum





                                Result &= Serialize(Bytes, &Element->GameCameraFOV); // default







                                Result &= Serialize(Bytes, &Element->Lighting); // default







                                Result &= Serialize(Bytes, &Element->ApplicationResolution); // default







                                Result &= Serialize(Bytes, &Element->ShadowMapResolution); // default







                                Result &= Serialize(Bytes, &Element->LuminanceMapResolution); // default







                                Result &= Serialize(Bytes, &Element->iApplicationResolution); // default







                                Result &= Serialize(Bytes, &Element->iShadowMapResolution); // default







                                Result &= Serialize(Bytes, &Element->iLuminanceMapResolution); // default









            

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, render_settings *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory)
{
  b32 Result = True;
                  
  
  Result &= Deserialize(Bytes, &Element->UseSsao, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->UseShadowMapping, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->UseLightingBloom, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->BravoilMyersOIT, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->BravoilMcGuireOIT, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->DrawMajorGrid, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->DrawMinorGrid, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->MajorGridDim, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->DrawCameraGhost, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->CameraGhostSize, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->Ignored, Memory);








                
  
  Result &= Deserialize(Bytes, &Element->Headless, Memory);







            Element->ToneMappingType = Cast(tone_mapping_type, Read_u32(Bytes));





                
  
  Result &= Deserialize(Bytes, &Element->GameCameraFOV, Memory);







                
  
  Result &= Deserialize(Bytes, &Element->Lighting, Memory);







                  
  
  Result &= Deserialize(Bytes, &Element->ApplicationResolution, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->ShadowMapResolution, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->LuminanceMapResolution, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->iApplicationResolution, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->iShadowMapResolution, Memory);








                  
  
  Result &= Deserialize(Bytes, &Element->iLuminanceMapResolution, Memory);










    
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, render_settings *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


