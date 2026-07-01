// callsite
// src/engine/serdes.cpp:307:0

// def (serdes_struct)
// src/engine/serdes.h:619:0
link_internal bonsai_type_info
TypeInfo(file_traversal_node *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("file_traversal_node");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, file_traversal_node *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    file_traversal_node *Element = BaseElement + ElementIndex;
                            Result &= Serialize(Bytes, (u32*)&Element->Type); // enum





                                Result &= Serialize(Bytes, &Element->Dir); // default







                                Result &= Serialize(Bytes, &Element->Name); // default









            

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, file_traversal_node *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, file_traversal_node *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, file_traversal_node *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, file_traversal_node *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, file_traversal_node *Element, memory_arena *Memory)
{
  b32 Result = True;
  b32 ThisMember;

    ThisMember = 3;
            Element->Type = Cast(file_traversal_type, Read_u32(Bytes));





  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing file_traversal_type Type on file_traversal_node");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Dir, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing cs Dir on file_traversal_node");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->Name, Memory);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing cs Name on file_traversal_node");
  }
  Result &= ThisMember;


    
  
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, file_traversal_node *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, file_traversal_node *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


