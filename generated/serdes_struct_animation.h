link_internal b32
Serialize(native_file *File, animation *Element)
{
  u64 PointerMark = True; 
  b32 Result = True;

  Result &= Serialize(File, &Element->t);


  Result &= Serialize(File, &Element->tEnd);


  Result &= Serialize(File, &Element->xKeyframeCount);


  if (Element->xKeyframes) { Result &= WriteToFile(File, Cast(u8*, &PointerMark), sizeof(PointerMark)); }

  Result &= Serialize(File, &Element->yKeyframeCount);


  if (Element->yKeyframes) { Result &= WriteToFile(File, Cast(u8*, &PointerMark), sizeof(PointerMark)); }

  Result &= Serialize(File, &Element->zKeyframeCount);


  if (Element->zKeyframes) { Result &= WriteToFile(File, Cast(u8*, &PointerMark), sizeof(PointerMark)); }


  if (Element->xKeyframes) { Result &= Serialize(File, Element->xKeyframes); }

  if (Element->yKeyframes) { Result &= Serialize(File, Element->yKeyframes); }

  if (Element->zKeyframes) { Result &= Serialize(File, Element->zKeyframes); }

  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, animation *Element, memory_arena *Memory)
{
  b32 Result = True;
  Result &= Deserialize(Bytes, &Element->t);



  Result &= Deserialize(Bytes, &Element->tEnd);



  Result &= Deserialize(Bytes, &Element->xKeyframeCount);



  b64 HadxKeyframesPointer = Read_u64(Bytes);

  Result &= Deserialize(Bytes, &Element->yKeyframeCount);



  b64 HadyKeyframesPointer = Read_u64(Bytes);

  Result &= Deserialize(Bytes, &Element->zKeyframeCount);



  b64 HadzKeyframesPointer = Read_u64(Bytes);


  if (HadxKeyframesPointer)
  {
    if (Element->xKeyframes == 0) { Element->xKeyframes = Allocate(keyframe, Memory, 1); }
    Result &= Deserialize(Bytes, Element->xKeyframes, Memory);
  }

  if (HadyKeyframesPointer)
  {
    if (Element->yKeyframes == 0) { Element->yKeyframes = Allocate(keyframe, Memory, 1); }
    Result &= Deserialize(Bytes, Element->yKeyframes, Memory);
  }

  if (HadzKeyframesPointer)
  {
    if (Element->zKeyframes == 0) { Element->zKeyframes = Allocate(keyframe, Memory, 1); }
    Result &= Deserialize(Bytes, Element->zKeyframes, Memory);
  }

  return Result;
}


