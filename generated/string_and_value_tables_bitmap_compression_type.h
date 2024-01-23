link_internal counted_string
ToString(bitmap_compression_type Type)
{
  counted_string Result = {};
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


