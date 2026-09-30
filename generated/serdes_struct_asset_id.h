// callsite
// src/engine/serdes.cpp:314:0

// def (serdes_struct)
// src/engine/serdes.h:617:0
link_internal bonsai_type_info
TypeInfo(asset_id *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("asset_id");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, asset_id *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    asset_id *Element = BaseElement + ElementIndex;
            
                                Result &= Serialize(Bytes, &Element->FileNode); // default









            
        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, asset_id *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


link_internal b32
Deserialize(u8_cursor *Bytes, asset_id *Element, memory_arena *Memory);

link_internal b32
Deserialize(u8_cursor *Bytes, asset_id *Element, memory_arena *Memory, umm Count);

link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, asset_id *Element, memory_arena *Memory);




link_internal b32
DeserializeCurrentVersion(u8_cursor *Bytes, asset_id *Element, memory_arena *Memory)
{
  b32 Result = True;
  b32 ThisMember;

    ThisMember = 3;
    
  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing u16 Index on asset_id");
  }
  Result &= ThisMember;
  ThisMember = 3;
                
  
  ThisMember = Deserialize(Bytes, &Element->FileNode, Memory);

  Assert(Element->Index == INVALID_ASSET_INDEX);







  /* Assert(ThisMember != 3); */
  if (ThisMember == False)
  {
    SoftError("Deserializing file_traversal_node FileNode on asset_id");
  }
  Result &= ThisMember;


    
  


  MAYBE_READ_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, asset_id *Element, memory_arena *Memory, umm Count)
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
Deserialize(u8_cursor *Bytes, asset_id *Element, memory_arena *Memory)
{
  return Deserialize(Bytes, Element, Memory, 1);
}


