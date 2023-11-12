// TODO(Jesse): Remove this?
#define PARTICLE_SYSTEM_EMIT_FOREVER (f32_MAX)
struct particle
{
  // TODO(Jesse, id: 85, tags: robustness, memory_consumption): Compress to 16 bit float?
  v3 Velocity;
  v3 Offset;

  u8 Color;
  b8 IsLight;
  r32 RemainingLifespan;
};

enum particle_spawn_type
{
  ParticleSpawnType_None,

  ParticleSpawnType_Random, // Particles spawn with random velocity
  ParticleSpawnType_Expanding, // Spawn velocity pointing away from center of spawn region
  ParticleSpawnType_Contracting, // Spawn velocity pointing towards center of spawn region
};

#define PARTICLE_SYSTEM_COLOR_COUNT 6
#define PARTICLES_PER_SYSTEM   (4096)
struct particle_system
{
  random_series Entropy;

  particle_spawn_type SpawnType;

  r32 Drag;

  r32 Lifetime;  // Time since spawned

  r32 EmissionDelay;    // How long the system waits to before starting to emit
  r32 EmissionLifespan; // How long the system emits for

  u32 ActiveParticles;

  r32 LifespanMod;
  r32 ParticleLifespan;      // How long an individual particle lasts
  r32 ParticlesPerSecond;

  // Are particles emissive?
  // Value is rounded up to 1.f.  Values greater than 1.f contribute to emission
  // which is accumulated into the bloom texuture
  r32 ParticleLightEmission;

  // Chance to emit a light-emitting particle that's treated as a point light
  // and accumulated into the lighting buffer
  r32 ParticleLightEmissionChance; // 0.f - 1.f (no chance - always emit)

  // 0.f is ignored, f32_MIN is fully transparent, 1.f is opaque
  r32 ParticleStartingTransparency;
  r32 ParticleEndingTransparency; // Can be 0.f

  v3  ParticleStartingDim;
  f32 ParticleEndingDim;

  v3 ParticleTurbMin;
  v3 ParticleTurbMax;

  aabb SpawnRegion;

  r32 SystemMovementCoefficient;

  r32 ElapsedSinceLastEmission;

  u8 Colors[PARTICLE_SYSTEM_COLOR_COUNT];

#if !POOF_PREPROCESSOR
  particle Particles[PARTICLES_PER_SYSTEM];
#endif

  /* untextured_3d_geometry_buffer *Dest; */
};


