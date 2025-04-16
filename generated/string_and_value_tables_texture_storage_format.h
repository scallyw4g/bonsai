// external/bonsai_stdlib/src/to_string.cpp:62:0

link_internal counted_string
ToStringPrefixless(texture_storage_format Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case TextureStorageFormat_Undefined: { Result = CSz("Undefined"); } break;
    case TextureStorageFormat_R16I: { Result = CSz("R16I"); } break;
    case TextureStorageFormat_R16F: { Result = CSz("R16F"); } break;
    case TextureStorageFormat_R32F: { Result = CSz("R32F"); } break;
    case TextureStorageFormat_RG16F: { Result = CSz("RG16F"); } break;
    case TextureStorageFormat_RG32F: { Result = CSz("RG32F"); } break;
    case TextureStorageFormat_RGB16F: { Result = CSz("RGB16F"); } break;
    case TextureStorageFormat_RGB32F: { Result = CSz("RGB32F"); } break;
    case TextureStorageFormat_RGBA8: { Result = CSz("RGBA8"); } break;
    case TextureStorageFormat_RGBA16F: { Result = CSz("RGBA16F"); } break;
    case TextureStorageFormat_RGBA32F: { Result = CSz("RGBA32F"); } break;
    case TextureStorageFormat_Depth32: { Result = CSz("Depth32"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(texture_storage_format Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case TextureStorageFormat_Undefined: { Result = CSz("TextureStorageFormat_Undefined"); } break;
    case TextureStorageFormat_R16I: { Result = CSz("TextureStorageFormat_R16I"); } break;
    case TextureStorageFormat_R16F: { Result = CSz("TextureStorageFormat_R16F"); } break;
    case TextureStorageFormat_R32F: { Result = CSz("TextureStorageFormat_R32F"); } break;
    case TextureStorageFormat_RG16F: { Result = CSz("TextureStorageFormat_RG16F"); } break;
    case TextureStorageFormat_RG32F: { Result = CSz("TextureStorageFormat_RG32F"); } break;
    case TextureStorageFormat_RGB16F: { Result = CSz("TextureStorageFormat_RGB16F"); } break;
    case TextureStorageFormat_RGB32F: { Result = CSz("TextureStorageFormat_RGB32F"); } break;
    case TextureStorageFormat_RGBA8: { Result = CSz("TextureStorageFormat_RGBA8"); } break;
    case TextureStorageFormat_RGBA16F: { Result = CSz("TextureStorageFormat_RGBA16F"); } break;
    case TextureStorageFormat_RGBA32F: { Result = CSz("TextureStorageFormat_RGBA32F"); } break;
    case TextureStorageFormat_Depth32: { Result = CSz("TextureStorageFormat_Depth32"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal texture_storage_format
TextureStorageFormat(counted_string S)
{
  texture_storage_format Result = {};

    if (StringsMatch(S, CSz("TextureStorageFormat_Undefined"))) { return TextureStorageFormat_Undefined; }
  if (StringsMatch(S, CSz("TextureStorageFormat_R16I"))) { return TextureStorageFormat_R16I; }
  if (StringsMatch(S, CSz("TextureStorageFormat_R16F"))) { return TextureStorageFormat_R16F; }
  if (StringsMatch(S, CSz("TextureStorageFormat_R32F"))) { return TextureStorageFormat_R32F; }
  if (StringsMatch(S, CSz("TextureStorageFormat_RG16F"))) { return TextureStorageFormat_RG16F; }
  if (StringsMatch(S, CSz("TextureStorageFormat_RG32F"))) { return TextureStorageFormat_RG32F; }
  if (StringsMatch(S, CSz("TextureStorageFormat_RGB16F"))) { return TextureStorageFormat_RGB16F; }
  if (StringsMatch(S, CSz("TextureStorageFormat_RGB32F"))) { return TextureStorageFormat_RGB32F; }
  if (StringsMatch(S, CSz("TextureStorageFormat_RGBA8"))) { return TextureStorageFormat_RGBA8; }
  if (StringsMatch(S, CSz("TextureStorageFormat_RGBA16F"))) { return TextureStorageFormat_RGBA16F; }
  if (StringsMatch(S, CSz("TextureStorageFormat_RGBA32F"))) { return TextureStorageFormat_RGBA32F; }
  if (StringsMatch(S, CSz("TextureStorageFormat_Depth32"))) { return TextureStorageFormat_Depth32; }

  return Result;
}


