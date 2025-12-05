// callsite
// src/engine/serdes.cpp:391:0

// def (serialize_struct)
// src/engine/serdes.h:153:0
link_internal bonsai_type_info
TypeInfo(level_header *Ignored)
{
  bonsai_type_info Result = {};

  Result.Name = CSz("level_header");
  Result.Version =  0 ;

  
  
  
  
  
  
  

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, level_header *BaseElement, umm Count)
{
  Assert(Count > 0);

  u64 PointerTrue  = True;
  u64 PointerFalse = False;

  b32 Result = True;

  

  RangeIterator_t(umm, ElementIndex, Count)
  {
    level_header *Element = BaseElement + ElementIndex;
                                    Result &= Serialize(Bytes, &Element->LayerCount); // default







                                Result &= Serialize(Bytes, &Element->EditCount); // default







                                Result &= Serialize(Bytes, &Element->EntityCount); // default







                                Result &= Serialize(Bytes, &Element->SelectedLayerIndex); // default







                                Result &= Serialize(Bytes, &Element->WorldCenter); // default







                        Result &= Serialize(Bytes, (u32*)&Element->VisibleRegionSize); // enum





                                Result &= Serialize(Bytes, &Element->Camera); // default







                                Result &= Serialize(Bytes, &Element->RenderSettings); // default







                            {
            umm ThisCount = (256);

      Result &= Serialize(Bytes, Element->TerrainShapingShader, ThisCount);
    }






                            {
            umm ThisCount = (256);

      Result &= Serialize(Bytes, Element->TerrainDecorationShader, ThisCount);
    }








            

        

        

        

        

        

        

        

        

        



    MAYBE_WRITE_DEBUG_OBJECT_DELIM();
  }

  return Result;
}

link_internal b32
Serialize(u8_cursor_block_array *Bytes, level_header *BaseElement)
{
  return Serialize(Bytes, BaseElement, 1);
}


