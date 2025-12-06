// callsite
// src/engine/editor.cpp:212:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(visible_region_size Value)
{
  b32 Result = False;
  switch (Value)
  {
        case VisibleRegionSize_1:
    case VisibleRegionSize_2:
    case VisibleRegionSize_4:
    case VisibleRegionSize_8:
    case VisibleRegionSize_16:
    case VisibleRegionSize_32:
    case VisibleRegionSize_64:
    case VisibleRegionSize_128:
    case VisibleRegionSize_256:
    case VisibleRegionSize_512:
    case VisibleRegionSize_1k:
    case VisibleRegionSize_2k:
    case VisibleRegionSize_4k:
    case VisibleRegionSize_8k:
    case VisibleRegionSize_16k:
    case VisibleRegionSize_32k:
    case VisibleRegionSize_64k:
    case VisibleRegionSize_128k:
    case VisibleRegionSize_256k:
    case VisibleRegionSize_512k:
    case VisibleRegionSize_1024k:
    case VisibleRegionSize_2048k:
    case VisibleRegionSize_4096k:
    case VisibleRegionSize_8192k:
    case VisibleRegionSize_16kk:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(visible_region_size Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case VisibleRegionSize_1: { Result = CSz("1"); } break;
      case VisibleRegionSize_2: { Result = CSz("2"); } break;
      case VisibleRegionSize_4: { Result = CSz("4"); } break;
      case VisibleRegionSize_8: { Result = CSz("8"); } break;
      case VisibleRegionSize_16: { Result = CSz("16"); } break;
      case VisibleRegionSize_32: { Result = CSz("32"); } break;
      case VisibleRegionSize_64: { Result = CSz("64"); } break;
      case VisibleRegionSize_128: { Result = CSz("128"); } break;
      case VisibleRegionSize_256: { Result = CSz("256"); } break;
      case VisibleRegionSize_512: { Result = CSz("512"); } break;
      case VisibleRegionSize_1k: { Result = CSz("1k"); } break;
      case VisibleRegionSize_2k: { Result = CSz("2k"); } break;
      case VisibleRegionSize_4k: { Result = CSz("4k"); } break;
      case VisibleRegionSize_8k: { Result = CSz("8k"); } break;
      case VisibleRegionSize_16k: { Result = CSz("16k"); } break;
      case VisibleRegionSize_32k: { Result = CSz("32k"); } break;
      case VisibleRegionSize_64k: { Result = CSz("64k"); } break;
      case VisibleRegionSize_128k: { Result = CSz("128k"); } break;
      case VisibleRegionSize_256k: { Result = CSz("256k"); } break;
      case VisibleRegionSize_512k: { Result = CSz("512k"); } break;
      case VisibleRegionSize_1024k: { Result = CSz("1024k"); } break;
      case VisibleRegionSize_2048k: { Result = CSz("2048k"); } break;
      case VisibleRegionSize_4096k: { Result = CSz("4096k"); } break;
      case VisibleRegionSize_8192k: { Result = CSz("8192k"); } break;
      case VisibleRegionSize_16kk: { Result = CSz("16kk"); } break;


      
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
ToString(visible_region_size Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case VisibleRegionSize_1: { Result = CSz("VisibleRegionSize_1"); } break;
    case VisibleRegionSize_2: { Result = CSz("VisibleRegionSize_2"); } break;
    case VisibleRegionSize_4: { Result = CSz("VisibleRegionSize_4"); } break;
    case VisibleRegionSize_8: { Result = CSz("VisibleRegionSize_8"); } break;
    case VisibleRegionSize_16: { Result = CSz("VisibleRegionSize_16"); } break;
    case VisibleRegionSize_32: { Result = CSz("VisibleRegionSize_32"); } break;
    case VisibleRegionSize_64: { Result = CSz("VisibleRegionSize_64"); } break;
    case VisibleRegionSize_128: { Result = CSz("VisibleRegionSize_128"); } break;
    case VisibleRegionSize_256: { Result = CSz("VisibleRegionSize_256"); } break;
    case VisibleRegionSize_512: { Result = CSz("VisibleRegionSize_512"); } break;
    case VisibleRegionSize_1k: { Result = CSz("VisibleRegionSize_1k"); } break;
    case VisibleRegionSize_2k: { Result = CSz("VisibleRegionSize_2k"); } break;
    case VisibleRegionSize_4k: { Result = CSz("VisibleRegionSize_4k"); } break;
    case VisibleRegionSize_8k: { Result = CSz("VisibleRegionSize_8k"); } break;
    case VisibleRegionSize_16k: { Result = CSz("VisibleRegionSize_16k"); } break;
    case VisibleRegionSize_32k: { Result = CSz("VisibleRegionSize_32k"); } break;
    case VisibleRegionSize_64k: { Result = CSz("VisibleRegionSize_64k"); } break;
    case VisibleRegionSize_128k: { Result = CSz("VisibleRegionSize_128k"); } break;
    case VisibleRegionSize_256k: { Result = CSz("VisibleRegionSize_256k"); } break;
    case VisibleRegionSize_512k: { Result = CSz("VisibleRegionSize_512k"); } break;
    case VisibleRegionSize_1024k: { Result = CSz("VisibleRegionSize_1024k"); } break;
    case VisibleRegionSize_2048k: { Result = CSz("VisibleRegionSize_2048k"); } break;
    case VisibleRegionSize_4096k: { Result = CSz("VisibleRegionSize_4096k"); } break;
    case VisibleRegionSize_8192k: { Result = CSz("VisibleRegionSize_8192k"); } break;
    case VisibleRegionSize_16kk: { Result = CSz("VisibleRegionSize_16kk"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal visible_region_size
VisibleRegionSize(counted_string S)
{
  visible_region_size Result = {};

    if (StringsMatch(S, CSz("VisibleRegionSize_1"))) { return VisibleRegionSize_1; }
  if (StringsMatch(S, CSz("VisibleRegionSize_2"))) { return VisibleRegionSize_2; }
  if (StringsMatch(S, CSz("VisibleRegionSize_4"))) { return VisibleRegionSize_4; }
  if (StringsMatch(S, CSz("VisibleRegionSize_8"))) { return VisibleRegionSize_8; }
  if (StringsMatch(S, CSz("VisibleRegionSize_16"))) { return VisibleRegionSize_16; }
  if (StringsMatch(S, CSz("VisibleRegionSize_32"))) { return VisibleRegionSize_32; }
  if (StringsMatch(S, CSz("VisibleRegionSize_64"))) { return VisibleRegionSize_64; }
  if (StringsMatch(S, CSz("VisibleRegionSize_128"))) { return VisibleRegionSize_128; }
  if (StringsMatch(S, CSz("VisibleRegionSize_256"))) { return VisibleRegionSize_256; }
  if (StringsMatch(S, CSz("VisibleRegionSize_512"))) { return VisibleRegionSize_512; }
  if (StringsMatch(S, CSz("VisibleRegionSize_1k"))) { return VisibleRegionSize_1k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_2k"))) { return VisibleRegionSize_2k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_4k"))) { return VisibleRegionSize_4k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_8k"))) { return VisibleRegionSize_8k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_16k"))) { return VisibleRegionSize_16k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_32k"))) { return VisibleRegionSize_32k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_64k"))) { return VisibleRegionSize_64k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_128k"))) { return VisibleRegionSize_128k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_256k"))) { return VisibleRegionSize_256k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_512k"))) { return VisibleRegionSize_512k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_1024k"))) { return VisibleRegionSize_1024k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_2048k"))) { return VisibleRegionSize_2048k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_4096k"))) { return VisibleRegionSize_4096k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_8192k"))) { return VisibleRegionSize_8192k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_16kk"))) { return VisibleRegionSize_16kk; }


  return Result;
}


