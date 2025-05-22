// src/engine/editor.h:733:0

link_internal counted_string
ToStringPrefixless(ui_noise_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case NoiseType_Perlin: { Result = CSz("Perlin"); } break;
    case NoiseType_Voronoi: { Result = CSz("Voronoi"); } break;
    case NoiseType_White: { Result = CSz("White"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(ui_noise_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case NoiseType_Perlin: { Result = CSz("NoiseType_Perlin"); } break;
    case NoiseType_Voronoi: { Result = CSz("NoiseType_Voronoi"); } break;
    case NoiseType_White: { Result = CSz("NoiseType_White"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal ui_noise_type
UiNoiseType(counted_string S)
{
  ui_noise_type Result = {};

    if (StringsMatch(S, CSz("NoiseType_Perlin"))) { return NoiseType_Perlin; }
  if (StringsMatch(S, CSz("NoiseType_Voronoi"))) { return NoiseType_Voronoi; }
  if (StringsMatch(S, CSz("NoiseType_White"))) { return NoiseType_White; }

  return Result;
}


