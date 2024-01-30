// src/engine/serdes.cpp:244:0

link_internal bonsai_type_info
TypeInfo(file_traversal_node *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("file_traversal_node");
  Result.Version = 0 ;

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
Serialize(u8_cursor_block_array *Bytes, file_traversal_node *Element)
{
  u64 PointerTrue = True; 
  u64 PointerFalse = False; 

  b32 Result = True;

  

  Result &= Serialize(Bytes, (u32*)&Element->Type);




  Result &= Serialize(Bytes, &Element->Dir);





  Result &= Serialize(Bytes, &Element->Name);

  

  MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, file_traversal_node *Element, memory_arena *Memory);

link_internal b32
DeserializeUnversioned(u8_cursor *Bytes, file_traversal_node *Element, memory_arena *Memory)
{
  b32 Result = True;
  Element->Type = Cast(file_traversal_type, Read_u32(Bytes));




  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Dir, Memory);





  // NOTE(Jesse): Unfortunately we can't check for primitives because
  // strings are considered primitive, but need memory to deserialize
  Result &= Deserialize(Bytes, &Element->Name, Memory);

  
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, file_traversal_node *Element, memory_arena *Memory)
{
  b32 Result = True;

  Result &= DeserializeUnversioned(Bytes, Element, Memory);
  MAYBE_READ_DEBUG_OBJECT_DELIM();


  return Result;
}


