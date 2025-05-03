// src/engine/editor.cpp:91:0

link_internal counted_string
ToStringPrefixless(visible_region_size Type)
{
  counted_string Result = {};
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
    case VisibleRegionSize_1024: { Result = CSz("1024"); } break;
    case VisibleRegionSize_2048: { Result = CSz("2048"); } break;
    case VisibleRegionSize_4096: { Result = CSz("4096"); } break;
    case VisibleRegionSize_8192: { Result = CSz("8192"); } break;
    case VisibleRegionSize_16k: { Result = CSz("16k"); } break;
    case VisibleRegionSize_32k: { Result = CSz("32k"); } break;
    case VisibleRegionSize_64k: { Result = CSz("64k"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(visible_region_size Type)
{
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
    case VisibleRegionSize_1024: { Result = CSz("VisibleRegionSize_1024"); } break;
    case VisibleRegionSize_2048: { Result = CSz("VisibleRegionSize_2048"); } break;
    case VisibleRegionSize_4096: { Result = CSz("VisibleRegionSize_4096"); } break;
    case VisibleRegionSize_8192: { Result = CSz("VisibleRegionSize_8192"); } break;
    case VisibleRegionSize_16k: { Result = CSz("VisibleRegionSize_16k"); } break;
    case VisibleRegionSize_32k: { Result = CSz("VisibleRegionSize_32k"); } break;
    case VisibleRegionSize_64k: { Result = CSz("VisibleRegionSize_64k"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
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
  if (StringsMatch(S, CSz("VisibleRegionSize_1024"))) { return VisibleRegionSize_1024; }
  if (StringsMatch(S, CSz("VisibleRegionSize_2048"))) { return VisibleRegionSize_2048; }
  if (StringsMatch(S, CSz("VisibleRegionSize_4096"))) { return VisibleRegionSize_4096; }
  if (StringsMatch(S, CSz("VisibleRegionSize_8192"))) { return VisibleRegionSize_8192; }
  if (StringsMatch(S, CSz("VisibleRegionSize_16k"))) { return VisibleRegionSize_16k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_32k"))) { return VisibleRegionSize_32k; }
  if (StringsMatch(S, CSz("VisibleRegionSize_64k"))) { return VisibleRegionSize_64k; }

  return Result;
}


