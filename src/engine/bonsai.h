
struct debug_state;

struct world;
struct heap_allocator;
struct entity;





enum frame_event_type
{
  FrameEvent_Undefined,

  FrameEvent_Explosion,
  FrameEvent_GameModeLoss,
  FrameEvent_GameModePlaying,
  FrameEvent_GameModeWon,
};

enum game_mode_type
{
  GameMode_Title,
  GameMode_Playing,
  GameMode_Won,
  GameMode_Loss,
};

struct game_mode
{
  game_mode_type ActiveMode;
  r64 TimeRunning;
};

/* enum collision_type */
/* { */
/*   Collision_Player_Enemy            = EntityBehaviorFlags_Player | EntityBehaviorFlags_Enemy, */
/*   Collision_Player_EnemyProjectile  = EntityBehaviorFlags_Player | EntityBehaviorFlags_EnemyProjectile, */
/*   Collision_Player_PlayerProjectile = EntityBehaviorFlags_Player | EntityBehaviorFlags_PlayerProjectile, */
/*   Collision_Player_Loot             = EntityBehaviorFlags_Player | EntityBehaviorFlags_Loot, */
/*   Collision_Enemy_PlayerProjectile  = EntityBehaviorFlags_Enemy  | EntityBehaviorFlags_PlayerProjectile, */
/*   Collision_Enemy_PlayerProton      = EntityBehaviorFlags_Enemy  | EntityBehaviorFlags_PlayerProton, */
/*   Collision_Enemy_EnemyProjectile   = EntityBehaviorFlags_Enemy  | EntityBehaviorFlags_EnemyProjectile, */
/*   Collision_Enemy_Enemy             = EntityBehaviorFlags_Enemy, */
/* }; */

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

  u8 Colors[PARTICLE_SYSTEM_COLOR_COUNT];

  r32 ElapsedSinceLastEmission;
  particle Particles[PARTICLES_PER_SYSTEM];

  /* untextured_3d_geometry_buffer *Dest; */
};

struct frame_event
{
  frame_event_type Type;
  entity *Entity;

  frame_event *Next;

  frame_event(frame_event_type Type_in)
  {
    this->Type = Type_in;
    this->Entity = 0;
    this->Next = 0;
  }

  frame_event(entity *Entity_in, frame_event_type Type_in)
  {
    this->Type = Type_in;
    this->Entity = Entity_in;
    this->Next = 0;
  }
};

struct event_queue
{
  u64 CurrentFrameIndex;
  frame_event **Queue;

  frame_event *FirstFreeEvent;
};

inline frame_event*
GetFreeFrameEvent(event_queue *Queue)
{
  frame_event *FreeEvent = Queue->FirstFreeEvent;

  if (FreeEvent)
  {
    Queue->FirstFreeEvent = FreeEvent->Next;
    FreeEvent->Next = 0;
  }

  return FreeEvent;
}

struct entity_list
{
  entity *This;
  entity *Next;
};

#define POINT_BUFFER_SIZE (12)
struct point_buffer
{
  s32 Count;
  voxel_position Points[POINT_BUFFER_SIZE];

  voxel_position Min;
  voxel_position Max;
};

struct collision_event
{
  u32 Count;
  canonical_position MinP;
  canonical_position MaxP;
};

inline void
UnSetFlag( voxel_flag *Flags, voxel_flag Flag )
{
  *Flags = (voxel_flag)(*Flags & ~Flag);
  return;
}

inline void
UnSetFlag( chunk_flag *Flags, chunk_flag Flag )
{
  *Flags = (chunk_flag)(*Flags & ~Flag);
  return;
}

inline void
UnSetFlag( world_chunk *Chunk, chunk_flag Flag )
{
  UnSetFlag(&Chunk->Flags, Flag);
  return;
}

/* inline void */
/* UnSetFlag( world_chunk *Chunk, chunk_flag Flag ) */
/* { */
/*   UnSetFlag(Chunk->Data, Flag); */
/*   return; */
/* } */

inline void
SetFlag( u8 *Flags, voxel_flag Flag )
{
  /* Assert(Flag < u8_MAX); */
  *Flags = (u8)(*Flags | Flag);
  return;
}

inline void
SetFlag( chunk_flag *Flags, chunk_flag Flag )
{
  *Flags = (chunk_flag)(*Flags | Flag);
  return;
}

/* inline void */
/* SetFlag( chunk_data *Chunk, chunk_flag Flag ) */
/* { */
/*   SetFlag(&Chunk->Flags, Flag); */
/*   return; */
/* } */

inline void
SetFlag( world_chunk *Chunk, chunk_flag Flag )
{
  Chunk->Flags = (chunk_flag)(Chunk->Flags | Flag);
  return;
}

inline void
SetFlag(voxel *Voxel, voxel_flag Flag )
{
  SetFlag(&Voxel->Flags, Flag);
  return;
}

inline void
SetFlag(boundary_voxel *Voxel, voxel_flag Flag )
{
  SetFlag(&Voxel->V.Flags, Flag);
  return;
}

inline b32
IsSet( u8 Flags, voxel_flag Flag )
{
  b32 Result = ( (Flags & Flag) != 0 );
  return Result;
}

inline b32
IsSet( chunk_flag Flags, chunk_flag Flag )
{
  b32 Result = ( (Flags & Flag) != 0 );
  return Result;
}

/* inline b32 */
/* IsSet( chunk_data *C, chunk_flag Flag ) */
/* { */
/*   b32 Result = IsSet(C->Flags, Flag); */
/*   return Result; */
/* } */

inline b32
IsSet( world_chunk *Chunk, chunk_flag Flag )
{
  b32 Result = IsSet(Chunk->Flags, Flag);
  return Result;
}

inline b32
IsSet( voxel *V, voxel_flag Flag )
{
  b32 Result = IsSet(V->Flags, Flag);
  return Result;
}

inline b32
IsSet( boundary_voxel *V, voxel_flag Flag )
{
  b32 Result = IsSet(&V->V, Flag);
  return Result;
}

inline b32
NotSet( voxel_flag Flags, voxel_flag Flag )
{
  b32 Result = !(IsSet(Flags, Flag));
  return Result;
}

inline b32
NotSet( chunk_flag Flags, chunk_flag Flag )
{
  b32 Result = !(IsSet(Flags, Flag));
  return Result;
}

/* inline b32 */
/* NotSet( chunk_data *Chunk, chunk_flag Flag ) */
/* { */
/*   b32 Result = !(IsSet(Chunk, Flag)); */
/*   return Result; */
/* } */

inline b32
NotSet( world_chunk *Chunk, chunk_flag Flag )
{
  b32 Result = !(IsSet(Chunk, Flag));
  return Result;
}

inline b32
NotSet( voxel *Voxel, voxel_flag Flag )
{
  b32 Result = !(IsSet(Voxel, Flag));
  return Result;
}

inline b32
Spawned(entity *Entity)
{
  b32 Result = Entity->State == EntityState_Spawned;
  return Result;
}

inline r32
RemainingLifespan(particle_system *System)
{
  r32 Result = (System->EmissionDelay + System->EmissionLifespan) - System->Lifetime;
  return Result;
}

inline b32
Active(particle_system *System)
{
  b32 Result = (System->Lifetime < (System->EmissionDelay + System->EmissionLifespan)) || (System->ActiveParticles > 0);
  return Result;
}

link_internal b32
Inactive(particle_system *System)
{
  b32 Result = !Active(System);
  return Result;
}

inline b32
Reserved(entity *Entity)
{
  b32 Result = Entity->State == EntityState_Reserved;
  return Result;
}

inline b32
Destroyed(entity *Entity)
{
  b32 Result = Entity->State == EntityState_Destroyed;
  return Result;
}

inline b32
Unspawned(entity *Entity)
{
  b32 Result = !Spawned(Entity);
  return Result;
}

#if 0
typedef umm packed_voxel;
typedef umm unpacked_voxel;

inline u8
GetVoxelColor(packed_voxel *V)
{
  u8 Color = (V->Data >> (FINAL_POSITION_BIT) ) & ~( 0xFFFFFFFF << (COLOR_BIT_WIDTH));

  Assert(Color < PALETTE_SIZE);
  return Color;
}

inline void
SetVoxelColor(packed_voxel *Voxel, int w)
{
  u32 flagMask = (0xFFFFFFFF << FINAL_COLOR_BIT);
  u32 colorMask = ( flagMask | ~(0xFFFFFFFF << (FINAL_POSITION_BIT)) );

  u32 currentFlags = Voxel->Data & colorMask;

  Voxel->Data = currentFlags;
  Voxel->Data |= (w << (FINAL_POSITION_BIT));

  u8 color = GetVoxelColor(Voxel);
  Assert(color == w);
}

inline voxel_position
GetVoxelP(packed_voxel *V)
{
  voxel_position P = Voxel_Position(
    V->Data >> (POSITION_BIT_WIDTH * 0) & 0x000000FF >> (8 - POSITION_BIT_WIDTH),
    V->Data >> (POSITION_BIT_WIDTH * 1) & 0x000000FF >> (8 - POSITION_BIT_WIDTH),
    V->Data >> (POSITION_BIT_WIDTH * 2) & 0x000000FF >> (8 - POSITION_BIT_WIDTH)
  );

  return P;
}

inline void
SetVoxelP(packed_voxel *Voxel, voxel_position P)
{
  Assert( P.x < Pow2(POSITION_BIT_WIDTH) );
  Assert( P.y < Pow2(POSITION_BIT_WIDTH) );
  Assert( P.z < Pow2(POSITION_BIT_WIDTH) );

  int currentFlags = ( Voxel->Data & (0xFFFFFFFF << FINAL_POSITION_BIT));
  Voxel->Data = currentFlags;

  Voxel->Data |= P.x << (POSITION_BIT_WIDTH * 0);
  Voxel->Data |= P.y << (POSITION_BIT_WIDTH * 1);
  Voxel->Data |= P.z << (POSITION_BIT_WIDTH * 2);

  voxel_position SetP = GetVoxelP(Voxel);
  Assert(SetP == P);

  return;
}

inline packed_voxel
PackVoxel(unpacked_voxel *V)
{
  packed_voxel Result = {};

  Result.Data = V->Flags; // Must come first

  SetVoxelP(&Result, V->Offset);
  SetVoxelColor(&Result, V->ColorIndex);

  Result.Data = SetFlag(Result.Data, Voxel_Filled);

  return Result;
}

inline unpacked_voxel
GetUnpackedVoxel(int x, int y, int z, int w)
{
  unpacked_voxel V;

  V.Offset = Voxel_Position(x,y,z);
  V.ColorIndex = w;
  V.Flags = (voxel_flag)0;

  return V;
}

inline packed_voxel
GetPackedVoxel(int x, int y, int z, int w)
{
  packed_voxel Result = {};
  voxel_position P = Voxel_Position(x,y,z);

  SetVoxelP(&Result, P );
  SetVoxelColor(&Result, w);

  Assert(GetVoxelP(&Result) == P);
  Assert(GetVoxelColor(&Result) == w);

  return Result;
}

#endif

void
ZeroMesh( untextured_3d_geometry_buffer *Mesh )
{
  Mesh->At = 0;
  return;
}

void
ClearWorldChunk( world_chunk *Chunk )
{
  Chunk->Flags = {};
  Chunk->WorldP = {};
  Chunk->FilledCount = {};
  Chunk->Picked = {};
  Chunk->DrawBoundingVoxels = {};
  Chunk->PointsToLeaveRemaining = {};
  Chunk->TriCount = {};
  Chunk->EdgeBoundaryVoxelCount = {};
  Chunk->StandingSpots.At = Chunk->StandingSpots.Start;
}

inline b32
IsFilled(voxel *Voxel)
{
  b32 Result = (Voxel->Flags & Voxel_Filled) == Voxel_Filled;

#if BONSAI_INTERNAL
  if (!Result) Assert( (Voxel->Flags&VoxelFaceMask) == 0);
#endif
  return Result;
}

inline b32
NotFilled(voxel *Voxel)
{
  b32 Result = !IsFilled(Voxel);
  return Result;
}

inline b32
IsFilled( voxel *Voxels, voxel_position VoxelP, chunk_dimension Dim)
{
  s32 VoxelIndex = GetIndex(VoxelP, Dim);
  b32 isFilled = IsSet(Voxels + VoxelIndex, Voxel_Filled);
  return isFilled;
}

inline b32
NotFilled(voxel *Voxels, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 Result = !IsFilled(Voxels, VoxelP, Dim);
  return Result;
}

inline voxel*
GetVoxel( world_chunk* Chunk, voxel_position VoxelP)
{
  s32 VoxelIndex = GetIndex(VoxelP, Chunk->Dim);
  voxel *Result = Chunk->Voxels + VoxelIndex;
  return Result;
}

inline world_position
GetAbsoluteP( world_position P, chunk_dimension WorldChunkDim)
{
  world_position Result = World_Position((WorldChunkDim.x*P.x), (WorldChunkDim.y*P.y), (WorldChunkDim.z*P.z));
  return Result;
}

// TODO(Jesse): this is misguided for-sure.  Will certainly fail when entities
// are far from the origin due to floating point percision issues.  Change
// to a view-space computation.
inline v3
GetAbsoluteP( canonical_position CP, chunk_dimension WorldChunkDim)
{
  v3 Result = V3(CP.Offset.x+(r32)(WorldChunkDim.x*CP.WorldP.x),
                 CP.Offset.y+(r32)(WorldChunkDim.y*CP.WorldP.y),
                 CP.Offset.z+(r32)(WorldChunkDim.z*CP.WorldP.z));
  return Result;
}

#if 0
inline aabb
GetAABB(entity *Entity, chunk_dimension WorldChunkDim)
{
  v3 Radius = Entity->CollisionVolumeRadius;
  v3 Center = GetAbsoluteP(Entity->P, WorldChunkDim) + Radius;

  aabb Result(Center, Radius);
  return Result;
}
#endif

inline b32
Contains( voxel_position Dim, voxel_position P )
{
  b32 Result = ( P.x >= 0    && P.y >= 0    && P.z >= 0 &&
                 P.x < Dim.x && P.y < Dim.y && P.z < Dim.z );
  return Result;
}

inline b32
IsInsideRange( v3i Min, v3i P, v3i Max)
{
  b32 Result = ( P.x >= Min.x && P.y >= Min.y && P.z >= Min.z &&
                 P.x <  Max.x && P.y <  Max.y && P.z <  Max.z );
  return Result;
}

inline b32
IsInsideDim( voxel_position Dim, voxel_position P )
{
  b32 Result = Contains(Dim, P);
  return Result;
}

inline b32
IsInsideDim( voxel_position Dim, v3 P )
{
  b32 Result = IsInsideDim(Dim, Voxel_Position(P) );
  return Result;
}
