// callsite
// src/engine/editor.cpp:330:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(particle_spawn_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case ParticleSpawnType_None:
    case ParticleSpawnType_Random:
    case ParticleSpawnType_Expanding:
    case ParticleSpawnType_Contracting:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(particle_spawn_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case ParticleSpawnType_None: { Result = CSz("None"); } break;
      case ParticleSpawnType_Random: { Result = CSz("Random"); } break;
      case ParticleSpawnType_Expanding: { Result = CSz("Expanding"); } break;
      case ParticleSpawnType_Contracting: { Result = CSz("Contracting"); } break;


      
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
ToString(particle_spawn_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case ParticleSpawnType_None: { Result = CSz("ParticleSpawnType_None"); } break;
    case ParticleSpawnType_Random: { Result = CSz("ParticleSpawnType_Random"); } break;
    case ParticleSpawnType_Expanding: { Result = CSz("ParticleSpawnType_Expanding"); } break;
    case ParticleSpawnType_Contracting: { Result = CSz("ParticleSpawnType_Contracting"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal particle_spawn_type
ParticleSpawnType(counted_string S)
{
  particle_spawn_type Result = {};

    if (StringsMatch(S, CSz("ParticleSpawnType_None"))) { return ParticleSpawnType_None; }
  if (StringsMatch(S, CSz("ParticleSpawnType_Random"))) { return ParticleSpawnType_Random; }
  if (StringsMatch(S, CSz("ParticleSpawnType_Expanding"))) { return ParticleSpawnType_Expanding; }
  if (StringsMatch(S, CSz("ParticleSpawnType_Contracting"))) { return ParticleSpawnType_Contracting; }


  return Result;
}


