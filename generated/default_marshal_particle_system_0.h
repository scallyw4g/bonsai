// callsite
// src/engine/serdes.cpp:288:0

// def (default_marshal)
// src/engine/serdes.h:15:0
Live->Entropy = Stored->Entropy;



Live->SpawnType = Stored->SpawnType;



Live->Drag = Stored->Drag;



Live->Lifetime = Stored->Lifetime;



Live->EmissionDelay = Stored->EmissionDelay;



Live->EmissionLifespan = Stored->EmissionLifespan;



Live->ActiveParticles = Stored->ActiveParticles;



Live->LifespanMod = Stored->LifespanMod;



Live->ParticleLifespan = Stored->ParticleLifespan;



Live->ParticlesPerSecond = Stored->ParticlesPerSecond;



Live->ParticleLightEmission = Stored->ParticleLightEmission;



Live->ParticleLightEmissionChance = Stored->ParticleLightEmissionChance;



Live->ParticleStartingTransparency = Stored->ParticleStartingTransparency;



Live->ParticleEndingTransparency = Stored->ParticleEndingTransparency;



Live->ParticleStartingDim = Stored->ParticleStartingDim;



Live->ParticleEndingDim = Stored->ParticleEndingDim;



Live->ParticleTurbMin = Stored->ParticleTurbMin;



Live->ParticleTurbMax = Stored->ParticleTurbMax;



Live->SpawnRegion = Stored->SpawnRegion;



Live->SystemMovementCoefficient = Stored->SystemMovementCoefficient;



Live->ElapsedSinceLastEmission = Stored->ElapsedSinceLastEmission;



RangeIterator(Index, 6)
{
     Live->PackedHSVColors[Index] = MagicaVoxelDefaultPaletteToPackedHSV(Stored->MCVColors[Index]);

}





