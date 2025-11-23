// callsite
// external/bonsai_stdlib/src/bitmap.cpp:76:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(bitmap_compression_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case BitmapCompressionType_RGB:
    case BitmapCompressionType_RLE8:
    case BitmapCompressionType_RLE4:
    case BitmapCompressionType_BITFIELDS:
    case BitmapCompressionType_JPEG:
    case BitmapCompressionType_PNG:
    case BitmapCompressionType_ALPHABITFIELDS:
    case BitmapCompressionType_CMYK:
    case BitmapCompressionType_CMYKRLE8:
    case BitmapCompressionType_CMYKRLE4:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(bitmap_compression_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case BitmapCompressionType_RGB: { Result = CSz("RGB"); } break;
      case BitmapCompressionType_RLE8: { Result = CSz("RLE8"); } break;
      case BitmapCompressionType_RLE4: { Result = CSz("RLE4"); } break;
      case BitmapCompressionType_BITFIELDS: { Result = CSz("BITFIELDS"); } break;
      case BitmapCompressionType_JPEG: { Result = CSz("JPEG"); } break;
      case BitmapCompressionType_PNG: { Result = CSz("PNG"); } break;
      case BitmapCompressionType_ALPHABITFIELDS: { Result = CSz("ALPHABITFIELDS"); } break;
      case BitmapCompressionType_CMYK: { Result = CSz("CMYK"); } break;
      case BitmapCompressionType_CMYKRLE8: { Result = CSz("CMYKRLE8"); } break;
      case BitmapCompressionType_CMYKRLE4: { Result = CSz("CMYKRLE4"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(bitmap_compression_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case BitmapCompressionType_RGB: { Result = CSz("BitmapCompressionType_RGB"); } break;
    case BitmapCompressionType_RLE8: { Result = CSz("BitmapCompressionType_RLE8"); } break;
    case BitmapCompressionType_RLE4: { Result = CSz("BitmapCompressionType_RLE4"); } break;
    case BitmapCompressionType_BITFIELDS: { Result = CSz("BitmapCompressionType_BITFIELDS"); } break;
    case BitmapCompressionType_JPEG: { Result = CSz("BitmapCompressionType_JPEG"); } break;
    case BitmapCompressionType_PNG: { Result = CSz("BitmapCompressionType_PNG"); } break;
    case BitmapCompressionType_ALPHABITFIELDS: { Result = CSz("BitmapCompressionType_ALPHABITFIELDS"); } break;
    case BitmapCompressionType_CMYK: { Result = CSz("BitmapCompressionType_CMYK"); } break;
    case BitmapCompressionType_CMYKRLE8: { Result = CSz("BitmapCompressionType_CMYKRLE8"); } break;
    case BitmapCompressionType_CMYKRLE4: { Result = CSz("BitmapCompressionType_CMYKRLE4"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal bitmap_compression_type
BitmapCompressionType(counted_string S)
{
  bitmap_compression_type Result = {};

    if (StringsMatch(S, CSz("BitmapCompressionType_RGB"))) { return BitmapCompressionType_RGB; }
  if (StringsMatch(S, CSz("BitmapCompressionType_RLE8"))) { return BitmapCompressionType_RLE8; }
  if (StringsMatch(S, CSz("BitmapCompressionType_RLE4"))) { return BitmapCompressionType_RLE4; }
  if (StringsMatch(S, CSz("BitmapCompressionType_BITFIELDS"))) { return BitmapCompressionType_BITFIELDS; }
  if (StringsMatch(S, CSz("BitmapCompressionType_JPEG"))) { return BitmapCompressionType_JPEG; }
  if (StringsMatch(S, CSz("BitmapCompressionType_PNG"))) { return BitmapCompressionType_PNG; }
  if (StringsMatch(S, CSz("BitmapCompressionType_ALPHABITFIELDS"))) { return BitmapCompressionType_ALPHABITFIELDS; }
  if (StringsMatch(S, CSz("BitmapCompressionType_CMYK"))) { return BitmapCompressionType_CMYK; }
  if (StringsMatch(S, CSz("BitmapCompressionType_CMYKRLE8"))) { return BitmapCompressionType_CMYKRLE8; }
  if (StringsMatch(S, CSz("BitmapCompressionType_CMYKRLE4"))) { return BitmapCompressionType_CMYKRLE4; }


  return Result;
}


