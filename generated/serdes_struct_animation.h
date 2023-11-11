link_internal b32
Serialize(native_file *File, animation *Element)
{
  keyframe *xKeyframesPrevValue = Element->xKeyframes;
  if (Element->xKeyframes) { Element->xKeyframes = Cast(keyframe*, 0x1); }

  keyframe *yKeyframesPrevValue = Element->yKeyframes;
  if (Element->yKeyframes) { Element->yKeyframes = Cast(keyframe*, 0x1); }

  keyframe *zKeyframesPrevValue = Element->zKeyframes;
  if (Element->zKeyframes) { Element->zKeyframes = Cast(keyframe*, 0x1); }


  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(animation));

  Element->xKeyframes = xKeyframesPrevValue;

  Element->yKeyframes = yKeyframesPrevValue;

  Element->zKeyframes = zKeyframesPrevValue;


  if (Element->xKeyframes) { Result &= Serialize(File, Element->xKeyframes); }

  if (Element->yKeyframes) { Result &= Serialize(File, Element->yKeyframes); }

  if (Element->zKeyframes) { Result &= Serialize(File, Element->zKeyframes); }


  return Result;
}

link_internal animation *
Deserialize_animation(u8_stream *Bytes)
{
  animation *Result = Cast(animation*, Bytes->At);
  Bytes->At += sizeof(animation);
  Assert(Bytes->At <= Bytes->End);

  if (Result->xKeyframes)
  {
    Result->xKeyframes = Deserialize_keyframe(Bytes);
  }

  if (Result->yKeyframes)
  {
    Result->yKeyframes = Deserialize_keyframe(Bytes);
  }

  if (Result->zKeyframes)
  {
    Result->zKeyframes = Deserialize_keyframe(Bytes);
  }

  return Result;
}


