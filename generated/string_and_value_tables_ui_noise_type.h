// src/engine/editor.h:561:0

link_internal counted_string
ToStringPrefixless(ui_noise_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case NoiseType_Perlin: { Result = CSz("Perlin"); } break;
    case NoiseType_Voronoi: { Result = CSz("Voronoi"); } break;

    
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

  return Result;
}


