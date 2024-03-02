// src/engine/editor.h:548:0

link_internal counted_string
ToString(ui_noise_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case NoiseType_None: { Result = CSz("NoiseType_None"); } break;
    case NoiseType_Perlin: { Result = CSz("NoiseType_Perlin"); } break;
    case NoiseType_Voronoi: { Result = CSz("NoiseType_Voronoi"); } break;

    
  }
  return Result;
}

link_internal ui_noise_type
UiNoiseType(counted_string S)
{
  ui_noise_type Result = {};

  if (StringsMatch(S, CSz("NoiseType_None"))) { return NoiseType_None; }
  if (StringsMatch(S, CSz("NoiseType_Perlin"))) { return NoiseType_Perlin; }
  if (StringsMatch(S, CSz("NoiseType_Voronoi"))) { return NoiseType_Voronoi; }

  return Result;
}


