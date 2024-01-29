// src/engine/editor.cpp:0:134

link_internal counted_string
ToString(particle_spawn_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case ParticleSpawnType_None: { Result = CSz("None"); } break;
    case ParticleSpawnType_Random: { Result = CSz("Random"); } break;
    case ParticleSpawnType_Expanding: { Result = CSz("Expanding"); } break;
    case ParticleSpawnType_Contracting: { Result = CSz("Contracting"); } break;

    
  }
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


