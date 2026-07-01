// callsite
// src/engine/serdes.cpp:380:0

// def (serdes_struct)
// src/engine/serdes.h:619:0
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
  b32 ThisMember;

    ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->UseSsao, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 UseSsao on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->UseShadowMapping, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 UseShadowMapping on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->UseLightingBloom, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 UseLightingBloom on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->BravoilMyersOIT, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 BravoilMyersOIT on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->BravoilMcGuireOIT, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 BravoilMcGuireOIT on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->DrawMajorGrid, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 DrawMajorGrid on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->DrawMinorGrid, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 DrawMinorGrid on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->MajorGridDim, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 MajorGridDim on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->DrawCameraGhost, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 DrawCameraGhost on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->CameraGhostSize, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing r32 CameraGhostSize on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->Ignored, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v3 Ignored on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Headless, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing b32 Headless on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
            Element->ToneMappingType = Cast(tone_mapping_type, Read_u32(Bytes));





  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing tone_mapping_type ToneMappingType on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->GameCameraFOV, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing f32 GameCameraFOV on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Lighting, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing lighting_settings Lighting on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->ApplicationResolution, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v2 ApplicationResolution on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->ShadowMapResolution, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v2 ShadowMapResolution on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->LuminanceMapResolution, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v2 LuminanceMapResolution on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->iApplicationResolution, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v2i iApplicationResolution on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->iShadowMapResolution, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v2i iShadowMapResolution on render_settings");
  }
  Result &= ThisMember;
  ThisMember = 3;
                  
  
  ThisMember = Deserialize(Bytes, &Element->iLuminanceMapResolution, Memory);








  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing v2i iLuminanceMapResolution on render_settings");
  }
  Result &= ThisMember;


    
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  


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


