// src/engine/serdes.h:563:0
link_internal bonsai_type_info
TypeInfo(simple_brush *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("simple_brush");
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
Serialize(u8_cursor_block_array *Bytes, simple_brush *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    simple_brush *Element = BaseElement + ElementIndex;
                                Result &= Serialize(Bytes, &Element->Name); // default






                            Result &= Serialize(Bytes, &Element->Outline); // default






                if (Element->Texture) { Result &= Write(Bytes, Cast(u8*,  &PointerTrue),  sizeof(PointerTrue)); }
    else                        { Result &= Write(Bytes, Cast(u8*, &PointerFalse), sizeof(PointerFalse)); }





            

        

                if (Element->Texture) { Result &= Serialize(Bytes, Element->Texture); }





    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, simple_brush *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, simple_brush *Element, memory_arena *Memory, umm Count = 1);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, simple_brush *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, simple_brush *Element, memory_arena *Memory)
{
  b32 Result = True;
                
  
  Result &= Deserialize(Bytes, &Element->Name, Memory);






              
  
  Result &= Deserialize(Bytes, &Element->Outline, Memory);






        b64 HadTexturePointer = Read_u64(Bytes);
  Assert(HadTexturePointer < 2); // Should be 0 or 1





    
  
      if (HadTexturePointer)
  {
        umm Count = 1;


    if (Element->Texture == 0)
    {
      Element->Texture = Allocate(layered_brush, Memory, Count);
    }

    Result &= Deserialize(Bytes, Element->Texture, Memory, Count);
  }




  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, simple_brush *Element, memory_arena *Memory, umm Count)
{
  Assert(Count > 0);

  b32 Result = True;
  RangeIterator_t(umm, ElementIndex, Count)
  {
        Result &= DeserializeCurrentVersion(Bytes, Element+ElementIndex, Memory);

  }

  return Result;
}


