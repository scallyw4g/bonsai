void
SpawnFire(entity *Entity, random_series *Entropy, v3 Offset, r32 Dim, b32 Colorful = False)
{
  particle_system *System = Entity->Emitter;

  System->SpawnType = ParticleSpawnType_Random;

  System->Drag = 5.0f;

  System->Entropy.Seed = RandomU32(Entropy);

  /* System->ParticleLightEmissionChance = 0.0; */
  System->ParticleLightEmissionChance = 0.40f;

  /* System->ParticleLightEmission = 1.f + Dim; */
  System->ParticleLightEmission = 0.05f;

  System->ParticleStartingTransparency = 0.4f;
  System->ParticleEndingTransparency = 0.3f;


  if (Colorful)
  {
    System->Colors[0] = (u8)Clamp(0u, RandomU32(Entropy), 254u);
    System->Colors[1] = (u8)Clamp(0u, RandomU32(Entropy), 254u);
    System->Colors[2] = (u8)Clamp(0u, RandomU32(Entropy), 254u);
    System->Colors[3] = (u8)Clamp(0u, RandomU32(Entropy), 254u);
    System->Colors[4] = (u8)Clamp(0u, RandomU32(Entropy), 254u);
    System->Colors[5] = (u8)Clamp(0u, RandomU32(Entropy), 254u);
  }
  else
  {
    /* System->Colors[0] = BLACK; */
    System->Colors[0] = GREY_7;
    /* System->Colors[0] = DARK_DARK_RED; */
    System->Colors[1] = DARK_DARK_RED;
    System->Colors[2] = DARK_RED;
    System->Colors[3] = DARK_ORANGE;
    System->Colors[4] = YELLOW;
    System->Colors[5] = LIGHT_LIGHT_YELLOW;
    /* System->Colors[5] = WHITE; */
  }


  System->SpawnRegion = RectCenterRad(Offset, V3(0.16f, 0.16f, 0.02f)*Dim*0.5f);


  System->EmissionLifespan = PARTICLE_SYSTEM_EMIT_FOREVER;

  System->LifespanMod = 0.17f;
  System->ParticleLifespan = 0.25f;
  System->ParticlesPerSecond = 25 + 25*Dim;

  /* System->Physics.Speed = 1; */
  /* System->Physics.Drag = V3(2.2f); */
  /* System->Physics.Mass = 6.0f; */

  r32 xyTurb = Dim;
  /* r32 xyTurb = 1.f + 2.f*Dim; */
  /* r32 xyTurb = 2.5f; */
  /* r32 xyTurb = 0.0f; */
  System->ParticleTurbMin = V3(-xyTurb, -xyTurb, 20.0f + (0.3f*Dim) );
  System->ParticleTurbMax = V3(xyTurb, xyTurb, 35.0f + (0.85f*Dim) );

  /* System->Physics.Velocity = V3(0.0f, 0.0f, 9.0f); */

  System->ParticleStartingDim = V3(0.9f) + (0.20f*Dim);
  System->ParticleEndingDim = 0.1f + (0.1f*(Dim/2.f));

  System->SystemMovementCoefficient = 0.25f;

  if (Inactive(System)) { SpawnParticleSystem(System); }
}

void
SpawnExplosion(entity *Entity, random_series *Entropy, v3 Offset, r32 Radius, untextured_3d_geometry_buffer *Dest)
{
  particle_system *System = Entity->Emitter;

  System->SpawnType = ParticleSpawnType_Expanding;

  System->Entropy.Seed = RandomU32(Entropy);

  System->Colors[0] = BLACK;
  System->Colors[1] = DARK_DARK_RED;
  System->Colors[2] = DARK_RED;
  System->Colors[3] = DARK_ORANGE;
  System->Colors[4] = LIGHT_ORANGE;
  System->Colors[5] = WHITE;

  /* System->Colors[1] = (u8)RandomU32(Entropy); */
  /* System->Colors[2] = (u8)RandomU32(Entropy); */
  /* System->Colors[3] = (u8)RandomU32(Entropy); */
  /* System->Colors[4] = (u8)RandomU32(Entropy); */
  /* System->Colors[5] = (u8)RandomU32(Entropy); */

  System->ParticleStartingTransparency = 0.4f;
  System->ParticleEndingTransparency = 0.4f;

  /* System->ParticleLightEmission = 1.2f; */
  System->ParticleLightEmissionChance = 0.05f;
  System->ParticleLightEmission = 0.1f;

  System->SpawnRegion = RectCenterRad(Offset, V3(Radius*0.2f) );

  System->EmissionLifespan = 0.12f;
  System->LifespanMod = 0.5f;
  System->ParticleLifespan = 0.15f;
  System->ParticlesPerSecond = 500.0f*Radius;

  // Fire particles are emissive

  /* System->ParticleLightEmission = 0.0f; */

  /* System->Physics.Speed = 2; */
  /* System->Physics.Drag = V3(2.2f); */
  /* System->Physics.Mass = 3.0f; */

  r32 xyTurb = 7.f*Radius;
  /* r32 xyTurb = 2.5f; */
  /* r32 xyTurb = 0.0f; */
  System->ParticleTurbMin = V3(-xyTurb, -xyTurb, -xyTurb);
  System->ParticleTurbMax = V3(xyTurb, xyTurb, xyTurb);

  /* System->Physics.Velocity = V3(0.f, 0.f, 10.f); */

  System->ParticleStartingDim = V3(0.3f) * Radius;
  System->ParticleEndingDim = 0.1f;

  System->SystemMovementCoefficient = 0.1f;
  System->Drag = 11.0f;

  /* System->Dest = Dest; */

  SpawnParticleSystem(Entity->Emitter);
}

void
SpawnSmoke(entity *Entity, random_series *Entropy, v3 Offset, r32 Radius, untextured_3d_geometry_buffer *Dest)
{
  particle_system *System = Entity->Emitter;

  System->SpawnType = ParticleSpawnType_Expanding;

  System->Entropy.Seed = RandomU32(Entropy);

  System->Colors[0] = GREY_0;
  System->Colors[1] = GREY_0;
  System->Colors[2] = GREY_0;
  System->Colors[3] = GREY_0;
  System->Colors[4] = GREY_0;
  System->Colors[5] = GREY_0;

  System->SpawnRegion = RectCenterRad(Offset, V3(Radius, Radius, Radius*0.5f) * 0.75f);

  System->EmissionDelay = 0.25f;

  System->EmissionLifespan = 0.2f;
  System->LifespanMod = 3.5f;
  System->ParticleLifespan = 0.1f;
  System->ParticlesPerSecond = 100.0f*Radius;

  /* System->Physics.Speed = 2; */
  /* System->Physics.Drag = V3(2.2f); */
  /* System->Physics.Mass = 3.0f; */

  System->ParticleStartingTransparency = 0.35f;
  System->ParticleEndingTransparency = 0.f;

  r32 TurbMin = 1.5f*Radius;
  r32 TurbMax = 2.0f*Radius;

  System->ParticleTurbMin = V3(TurbMin);
  System->ParticleTurbMax = V3(TurbMax);

  System->ParticleStartingDim = V3(0.5f, 0.5f, 0.5f) * Radius;
  System->ParticleEndingDim = 2.5f;

  System->SystemMovementCoefficient = 0.1f;
  System->Drag = 2.f;

  SpawnParticleSystem(Entity->Emitter);
}

link_internal void
SpawnPersistantSmoke(entity *Entity, random_series *Entropy, v3 Offset, r32 Radius)
{
  particle_system *System = Entity->Emitter;

  System->SpawnType = ParticleSpawnType_Expanding;

  System->Entropy.Seed = RandomU32(Entropy);

  System->Colors[0] = GREY_0;
  System->Colors[1] = GREY_0;
  System->Colors[2] = GREY_0;
  System->Colors[3] = GREY_0;
  System->Colors[4] = GREY_0;
  System->Colors[5] = GREY_0;

  System->SpawnRegion = RectCenterRad(Offset, V3(Radius, Radius, Radius*0.5f)*0.75f);

  System->EmissionLifespan = f32_MAX;

  System->LifespanMod = 5.f;
  System->ParticleLifespan = 2.5f;
  System->ParticlesPerSecond = Radius;

  System->ParticleStartingTransparency = 0.25f;
  System->ParticleEndingTransparency = 0.f;

  /* r32 TurbMin = 1.5f*Radius; */
  /* r32 TurbMax = 2.0f*Radius; */

  System->ParticleTurbMin = V3(-Radius, -Radius, Radius*0.2f);
  System->ParticleTurbMax = V3(Radius);

  System->ParticleStartingDim = V3(Radius, Radius, Radius*0.5f)*0.5f;
  System->ParticleEndingDim = Radius;

  System->SystemMovementCoefficient = 0.1f;
  System->Drag = 2.f;

  SpawnParticleSystem(Entity->Emitter);
}

link_internal void
SplosionBittyParticleSystem(entity *Entity, random_series *Entropy, v3 Offset, r32 Radius, untextured_3d_geometry_buffer *Dest)
{
  particle_system *System = Entity->Emitter;

  System->SpawnType = ParticleSpawnType_Expanding;

  System->Entropy.Seed = RandomU32(Entropy);

  System->Colors[0] = GREY_0;
  System->Colors[1] = GREY_1;
  System->Colors[2] = GREY_2;
  System->Colors[3] = GREY_3;
  System->Colors[4] = GREY_4;
  System->Colors[5] = GREY_5;

  System->SpawnRegion = RectCenterRad(Offset, V3(Radius) );

  System->ParticleStartingTransparency = 0.5f;

  /* System->EmissionLifespan = 1.0f; */
  /* System->EmissionLifespan = 5.0f; */
  /* System->EmissionLifespan = 15.0f; */
  /* System->EmissionLifespan = 0.23f; */
  System->EmissionLifespan = 1.f;
  System->LifespanMod = 0.5f;
  System->ParticleLifespan = 0.25f;
  System->ParticlesPerSecond = 20.0f;
  /* System->ParticlesPerSecond = 0.0f; */

  System->ParticleTurbMin = V3(0.f, 0.f, 0.f);
  System->ParticleTurbMax = V3(0.f, 0.f, .1f);

  System->ParticleStartingDim = V3(0.10f);
  System->ParticleEndingDim = 6.f;

  System->SystemMovementCoefficient = 1.f;
  /* System->Drag = 11.0f; */

  /* System->Dest = Dest; */

  SpawnParticleSystem(Entity->Emitter);
}

link_internal void
DoDig( engine_resources *Resources, canonical_position PickCP, f32 Radius, f32 Depth, memory_arena *TempMemory)
{
  v3 SimSpaceMinCenterP = GetSimSpaceP(Resources->World, PickCP);

  v3 MinP = SimSpaceMinCenterP - V3(Radius, Radius, Depth);
  v3 MaxP = SimSpaceMinCenterP + V3(Radius, Radius, Depth);

  world_update_op_shape Shape = {
    .Type = type_world_update_op_shape_params_rect,
    .world_update_op_shape_params_rect.P0 = MinP,
    .world_update_op_shape_params_rect.P1 = MaxP,
  };
  QueueWorldUpdateForRegion(Resources, WorldUpdateOperationMode_Subtractive, &Shape, ICE_BLUE, Resources->WorldUpdateMemory);
}

link_internal void
DoIceBlock( engine_resources *Resources, canonical_position PickCP, f32 Radius, memory_arena *TempMemory)
{
  v3 SimSpaceMinCenterP = GetSimSpaceP(Resources->World, PickCP);

  v3 MinP = SimSpaceMinCenterP - V3(Radius, Radius, 0.f);
  v3 MaxP = SimSpaceMinCenterP + V3(Radius, Radius, 3.f*Radius);

  world_update_op_shape Shape = {
    .Type = type_world_update_op_shape_params_rect,
    .world_update_op_shape_params_rect.P0 = MinP,
    .world_update_op_shape_params_rect.P1 = MaxP,
  };
  QueueWorldUpdateForRegion(Resources, WorldUpdateOperationMode_Additive, &Shape, ICE_BLUE, Resources->WorldUpdateMemory);
}

link_internal void
DoSplotion( engine_resources *Resources, canonical_position PickCP, f32 Radius, random_series *Entropy, memory_arena *TempMemory)
{
  UNPACK_ENGINE_RESOURCES(Resources);

  world_update_op_shape Shape = {
    .Type = type_world_update_op_shape_params_sphere,
    .world_update_op_shape_params_sphere.Radius = Radius,
    .world_update_op_shape_params_sphere.Location = PickCP,
  };
  QueueWorldUpdateForRegion(Resources, WorldUpdateOperationMode_Subtractive, WorldUpdateOperationModeModifier_Flood, &Shape, DARK_GREY, Resources->WorldUpdateMemory);
  /* QueueWorldUpdateForRegion(Resources, WorldUpdateOperationMode_Subtractive, WorldUpdateOperationModeModifier_None, &Shape, DARK_GREY, Resources->WorldUpdateMemory); */
  /* QueueWorldUpdateForRegion(Resources, WorldUpdateOperationMode_Additive, WorldUpdateOperationModeModifier_None, &Shape, DARK_GREY, Resources->WorldUpdateMemory); */
  /* QueueWorldUpdateForRegion(Resources, WorldUpdateOperationMode_Additive, WorldUpdateOperationModeModifier_Flood, &Shape, DARK_GREY, Resources->WorldUpdateMemory); */

  v3 SplosionSimP = GetSimSpaceP(World, PickCP);

  sphere Explosion = Sphere(SplosionSimP, Radius);
  u32_buffer Hits = GatherEntitiesIntersecting(World, EntityTable, &Explosion, TempMemory);

  for (u32 HitIndex = 0; HitIndex < Hits.Count; ++HitIndex)
  {
    u32 EntityIndex = Hits.Start[HitIndex];
    entity *HitEntity = EntityTable[EntityIndex];

    v3 ESimP = GetSimSpaceP(World, HitEntity->P);
    v3 EntityCenterP = GetSimSpaceCenterP(HitEntity, ESimP);

    aabb EntityAABB = GetSimSpaceAABB(World, HitEntity);
    v3 ClosestEntityPToSplosion = ClipPToAABB(&EntityAABB, Explosion.P);

    v3 SplosionToClosestEntityP = ClosestEntityPToSplosion - SplosionSimP;
    v3 SplosionToEntityCenter = EntityCenterP - SplosionSimP;

    r32 t = SafeDivide0(Length(SplosionToClosestEntityP), Radius);
    t = Clamp01(t);


    v3 MaxPower = V3(25.f, 25.f, 4.f) * Radius;
    v3 Power = Lerp(t, MaxPower, V3(0) );

    /* DebugLine("t(%f) Power(%f,%f,%f)", t, Power.x, Power.y, Power.z); */

    HitEntity->Physics.Force += Normalize(SplosionToEntityCenter) * Power;
  }

  entity_behavior_flags SpawnFlags = EntityBehaviorFlags_UnspawnOnParticleSystemTerminate;

  {
    entity *E = TryGetFreeEntityPtr(EntityTable);
    Assert(E);

    E->P = PickCP + V3(0.5f);
    SpawnEntity( E, SpawnFlags );
    /* E->UserData = (void*)GameEntityBehaviorFlags_Splosion; */
    SpawnExplosion(E, Entropy, {}, Radius, &Graphics->Transparency.GpuBuffer.Buffer);
  }
  {
    entity *E = TryGetFreeEntityPtr(EntityTable);
    Assert(E);

    E->P = PickCP + V3(0.5f);
    SpawnEntity( E, SpawnFlags );
    SpawnSmoke(E, Entropy, {}, Radius*0.5f, &Graphics->Transparency.GpuBuffer.Buffer);
  }

  u32 MaxBitties = 2*u32(Radius);
  for (u32 BittyIndex = 0; BittyIndex < MaxBitties; ++BittyIndex)
  {
    entity_behavior_flags BittySpawnFlags = entity_behavior_flags(EntityBehaviorFlags_Default|EntityBehaviorFlags_UnspawnOnParticleSystemTerminate);
    entity *E = TryGetFreeEntityPtr(EntityTable);
    Assert(E);

    E->Physics.Speed = 1.f;

    E->EulerAngles = RandomRotation(Entropy);
    E->Scale = 0.3f;
    E->_CollisionVolumeRadius = V3(.1f);

    v3 Rnd = RandomV3Bilateral(Entropy);
    E->Physics.Mass = 25.f;
    E->Physics.Force += Rnd*150.f*Radius;
    E->Physics.Force.z = Abs(E->Physics.Force.z) * 0.25f;
    E->P = PickCP + (Rnd*Radius) + V3(0.f, 0.f, 2.0f);
    E->P.Offset.z = PickCP.Offset.z + 2.f;

    SpawnEntity(E, BittySpawnFlags );

    if (GetCollision(World, E).Count) { Unspawn(E); continue; }

    // TODO(Jesse): Pretty sure we're spawning these things inside other entities
    // which is causing the bitty particles to wigg out, but this didn't fix it .. ?
    //
    /* if (GetCollision_Entities(World, E).Count) { Unspawn(E); continue; } */

    SplosionBittyParticleSystem(E, Entropy, {}, .1f, &Graphics->Transparency.GpuBuffer.Buffer);
  }
}

