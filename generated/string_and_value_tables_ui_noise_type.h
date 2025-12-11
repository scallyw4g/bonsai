// callsite
// src/engine/editor.h:1054:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(brush_noise_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case NoiseType_Perlin:
    case NoiseType_Voronoi:
    case NoiseType_White:
    case NoiseType_RectLattice:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(brush_noise_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case NoiseType_Perlin: { Result = CSz("Perlin"); } break;
      case NoiseType_Voronoi: { Result = CSz("Voronoi"); } break;
      case NoiseType_White: { Result = CSz("White"); } break;
      case NoiseType_RectLattice: { Result = CSz("RectLattice"); } break;


      
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
ToString(brush_noise_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case NoiseType_Perlin: { Result = CSz("NoiseType_Perlin"); } break;
    case NoiseType_Voronoi: { Result = CSz("NoiseType_Voronoi"); } break;
    case NoiseType_White: { Result = CSz("NoiseType_White"); } break;
    case NoiseType_RectLattice: { Result = CSz("NoiseType_RectLattice"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal brush_noise_type
BrushNoiseType(counted_string S)
{
  brush_noise_type Result = {};

    if (StringsMatch(S, CSz("NoiseType_Perlin"))) { return NoiseType_Perlin; }
  if (StringsMatch(S, CSz("NoiseType_Voronoi"))) { return NoiseType_Voronoi; }
  if (StringsMatch(S, CSz("NoiseType_White"))) { return NoiseType_White; }
  if (StringsMatch(S, CSz("NoiseType_RectLattice"))) { return NoiseType_RectLattice; }


  return Result;
}


