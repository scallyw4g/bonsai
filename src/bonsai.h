#include <constants.hpp>

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

enum chunk_flag
{
  Chunk_Uninitialized   = 0 << 0,
  Chunk_Initialized     = 1 << 1,

  Chunk_Queued          = 1 << 2,
  Chunk_MeshComplete    = 1 << 3,
  Chunk_GpuMeshComplete = 1 << 4,
  Chunk_Garbage         = 1 << 5,
  Chunk_Collected       = 1 << 6,

};

enum voxel_flag
{
  Voxel_Empty      =      0,
  Voxel_Filled     = 1 << 0,

  Voxel_LeftFace   = 1 << 1,
  Voxel_RightFace  = 1 << 2,
  Voxel_TopFace    = 1 << 3,
  Voxel_BottomFace = 1 << 4,
  Voxel_FrontFace  = 1 << 5,
  Voxel_BackFace   = 1 << 6,
};

enum entity_state
{
  EntityState_Uninitialized    = 0,
  EntityState_Initialized      = 1 << 0,
  EntityState_Spawned          = 1 << 1,
  EntityState_Destroyed        = 1 << 2,
  EntityState_Reserved         = 1 << 3,
};

enum entity_type
{
  EntityType_None             = 0,

  EntityType_Player           = 1 << 0,
  EntityType_Enemy            = 1 << 1,
  EntityType_EnemyProjectile  = 1 << 2,
  EntityType_PlayerProjectile = 1 << 3,
  EntityType_Loot             = 1 << 4,
  EntityType_PlayerProton     = 1 << 5,
  EntityType_ParticleSystem   = 1 << 6,
};

global_variable const entity_type ENTITY_TYPES = (entity_type)
  ( EntityType_Player           |
    EntityType_Enemy            |
    EntityType_EnemyProjectile  |
    EntityType_PlayerProjectile |
    EntityType_Loot             |
    EntityType_PlayerProton     |
    EntityType_ParticleSystem
   );

enum collision_type
{
  Collision_Player_Enemy            = EntityType_Player|EntityType_Enemy,
  Collision_Player_EnemyProjectile  = EntityType_Player|EntityType_EnemyProjectile,
  Collision_Player_PlayerProjectile = EntityType_Player|EntityType_PlayerProjectile,
  Collision_Player_Loot             = EntityType_Player|EntityType_Loot,
  Collision_Enemy_PlayerProjectile  = EntityType_Enemy |EntityType_PlayerProjectile,
  Collision_Enemy_PlayerProton      = EntityType_Enemy |EntityType_PlayerProton,
  Collision_Enemy_EnemyProjectile   = EntityType_Enemy |EntityType_EnemyProjectile,
  Collision_Enemy_Enemy             = EntityType_Enemy,
};

enum model_index
{
  ModelIndex_None,

  ModelIndex_Enemy      = EntityType_Enemy,
  ModelIndex_Player     = EntityType_Player,
  ModelIndex_Loot       = EntityType_Loot,
  ModelIndex_Projectile,
  ModelIndex_Proton,

  ModelIndex_Count,
};

struct voxel
{
  voxel_flag Flags;
  u8 Color;
};

struct boundary_voxel
{
  voxel V;
  voxel_position Offset;

  boundary_voxel(s32 x, s32 y, s32 z, u8 w)
  {
    this->Offset.x = x;
    this->Offset.y = y;
    this->Offset.z = z;

    this->V.Color = w;

    this->V.Flags = Voxel_Empty;
  }

  boundary_voxel(voxel *V_in, voxel_position Offset_in)
  {
    this->V = *V_in;
    this->Offset = Offset_in;
  }
};

struct noise_3d
{
  voxel *Voxels;
  chunk_dimension Dim;
};

#pragma pack(push, 1)
struct chunk_data
{
  chunk_flag Flags;
  voxel *Voxels;

  u8 Pad[52];
};
CAssert(sizeof(chunk_data) == 64);
#pragma pack(pop)

struct plane
{
  v3 P;
  v3 Normal;
  float d;

  plane( v3 P_in, v3 Normal_in )
  {
    this->P = P_in;
    this->Normal = Normal_in;

    this->d = -1.0f * (Normal.x*P.x + Normal.y*P.y + Normal.z*P.z);

    Assert(Normal.x*P.x + Normal.y*P.y + Normal.z*P.z + this->d == 0);
  }

  plane() {}
};

struct frustum
{
  float farClip;
  float nearClip;
  float width;
  float FOV;

  plane Top;
  plane Bot;
  plane Left;
  plane Right;
};

struct camera
{
  frustum Frust;

  canonical_position TargetP;
  canonical_position CurrentP;

  canonical_position ViewingTarget; // TODO : Can this just be a v3?

  r32 Pitch;
  r32 Roll;
  r32 Yaw;

  r32 DistanceFromTarget;

  v3 Front;
  v3 Right;
  v3 Up;
};

struct model
{
  untextured_3d_geometry_buffer Mesh; // 32 bytes
  chunk_dimension Dim;
  animation Animation;
};

struct random_series
{
  u64 Seed;

  random_series(u64 Seed_in = DEFAULT_ENTROPY_SEED)
  {
    this->Seed = Seed_in;
  }
};

struct physics
{
  v3 Velocity;
  v3 Force;
  v3 Delta;

  r32 Drag;
  r32 Mass;

  r32 Speed;
};

struct particle
{
  // TODO(Jesse): Compress to 16 bit float?
  v3 Offset;

  physics Physics;

  u8 Color;
  r32 RemainingLifespan;
};

#define PARTICLE_SYSTEM_COLOR_COUNT 6
struct particle_system_init_params
{
  aabb SpawnRegion;
  physics Physics;
  r32 SystemMovementCoefficient;

  r32 EmissionLifespan;
  r32 EmissionChance;

  r32 ParticleLifespan;

  v3 StartingDiameter;

  random_series Entropy;

  u8 Colors[PARTICLE_SYSTEM_COLOR_COUNT];
};

struct particle_system
{
  random_series Entropy;

  s32 ActiveParticles;
  r32 ParticleDuration;

  aabb SpawnRegion;

  v3 StartingDiameter;

  r32 EmissionLifespan;

  r32 ParticleLifespan;
  r32 EmissionChance;

  physics ParticlePhysics;
  r32 SystemMovementCoefficient;

  u8 Colors[PARTICLE_SYSTEM_COLOR_COUNT];

  particle Particles[PARTICLES_PER_SYSTEM];
};

struct entity
{
  model Model;
  v3 CollisionVolumeRadius;

  particle_system *Emitter;

  physics Physics;

  canonical_position P;

  Quaternion Rotation;

  entity_state State;
  entity_type Type;

  r32 Scale;

   // TODO(Jesse): Unneeded for projectiles. factor out of here?
  r32 RateOfFire;
  r32 FireCooldown;

  s32 Health; // Only needed for Player
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

#pragma pack(push, 1)
struct current_triangles;
struct world_chunk
{
  chunk_data* Data;
  untextured_3d_geometry_buffer* Mesh;
  untextured_3d_geometry_buffer* LodMesh;

  current_triangles* CurrentTriangles;

  world_position WorldP;

  u32 FilledCount;

  b32 Picked;
  b32 LodMesh_Complete;

  u8 Reserved[8];
};
CAssert(sizeof(world_chunk) == 64);
#pragma pack(pop)

struct collision_event
{
  world_chunk *Chunk;
  canonical_position CP;
  bool didCollide;
};


struct free_world_chunk
{
  world_chunk *chunk_data;
  world_chunk *Next;
};

struct world
{
  world_chunk **ChunkHash;
  world_chunk **FreeChunks;
  int FreeChunkCount;

  // This is the number of chunks in xyz we're going to update and render
  chunk_dimension VisibleRegion;
  chunk_dimension ChunkDim;

  world_position Center;
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
UnSetFlag( chunk_data *Chunk, chunk_flag Flag )
{
  UnSetFlag(&Chunk->Flags, Flag);
  return;
}

inline void
UnSetFlag( world_chunk *Chunk, chunk_flag Flag )
{
  UnSetFlag(Chunk->Data, Flag);
  return;
}

inline void
SetFlag( voxel_flag *Flags, voxel_flag Flag )
{
  *Flags = (voxel_flag)(*Flags | Flag);
  return;
}

inline void
SetFlag( chunk_flag *Flags, chunk_flag Flag )
{
  *Flags = (chunk_flag)(*Flags | Flag);
  return;
}

inline void
SetFlag( chunk_data *Chunk, chunk_flag Flag )
{
  SetFlag(&Chunk->Flags, Flag);
  return;
}

inline void
SetFlag( world_chunk *Chunk, chunk_flag Flag )
{
  SetFlag(Chunk->Data, Flag);
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
IsSet( voxel_flag Flags, voxel_flag Flag )
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

inline b32
IsSet( chunk_data *C, chunk_flag Flag )
{
  b32 Result = IsSet(C->Flags, Flag);
  return Result;
}

inline b32
IsSet( world_chunk *Chunk, chunk_flag Flag )
{
  b32 Result = IsSet(Chunk->Data, Flag);
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

inline b32
NotSet( chunk_data *Chunk, chunk_flag Flag )
{
  b32 Result = !(IsSet(Chunk, Flag));
  return Result;
}

inline b32
NotSet( world_chunk *Chunk, chunk_flag Flag )
{
  b32 Result = !(IsSet(Chunk->Data, Flag));
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

inline b32
Active(particle_system *System)
{
  b32 Result = (System->EmissionLifespan > 0) || (System->ActiveParticles > 0);
  return Result;
}

inline b32
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

inline u32
RandomU32(random_series *Entropy)
{
  // TODO(Jesse): This is LCG RNG - do we want a better one?

  // Values from Knuth
  u64 A = 6364136223846793005;
  u64 B = 1442695040888963407;
  u64 One = 1;
  u64 Mod = (One << 63);

  Entropy->Seed = ((A * Entropy->Seed) + B) % Mod;

  // The bottom-most bits do not contain a high amount of entropy, so shift the
  // most significant bits down and return them
  u32 Result = (u32)(Entropy->Seed >> 31);
  return Result;
}

inline s32
RandomPositiveS32(random_series *Entropy)
{
  s32 Result = Abs((s32)RandomU32(Entropy));
  Assert(Result >= 0);
  return Result;
}

inline s32
RandomS32(random_series *Entropy)
{
  s32 Result = (s32)RandomU32(Entropy);
  return Result;
}

inline r32
RandomUnilateral(random_series *Entropy)
{
  u32 Random = RandomU32(Entropy);
  r32 Result = (r32)Random/(r32)UINT_MAX;
  return Result;
}

inline r32
RandomBilateral(random_series *Entropy)
{
  r32 Result = (RandomUnilateral(Entropy) * 2.0f) - 1.0f;
  return Result;
}

inline b32
RandomChoice(random_series* Entropy)
{
  b32 Result = RandomBilateral(Entropy) > 0.0f;
  return Result;
}

inline v3
RandomV3(random_series *Entropy)
{
  v3 Result =  {{ RandomUnilateral(Entropy),
                  RandomUnilateral(Entropy),
                  RandomUnilateral(Entropy) }};
  return Result;
}

#if 0
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

inline voxel_position
GetPosition(s32 Index, chunk_dimension Dim)
{
 int x = Index % Dim.x;
 int y = (Index/Dim.x) % Dim.y ;
 int z = Index / (Dim.x*Dim.y);

 Assert(x <= Dim.x);
 Assert(y <= Dim.y);
 Assert(z <= Dim.z);

 voxel_position Result = Voxel_Position(x,y,z);
 return Result;
}

void
ZeroMesh( untextured_3d_geometry_buffer *Mesh )
{
  Mesh->At = 0;
  return;
}

void
ZeroChunk( chunk_data *Chunk )
{
  Chunk->Flags = Chunk_Uninitialized;
  return;
}

inline s32
GetIndexUnsafe(voxel_position P, chunk_dimension Dim)
{
  s32 i =
    (P.x) +
    (P.y*Dim.x) +
    (P.z*Dim.x*Dim.y);

  return i;
}

inline s32
GetIndex(voxel_position P, chunk_dimension Dim)
{
  s32 i =
    (P.x) +
    (P.y*Dim.x) +
    (P.z*Dim.x*Dim.y);

  Assert(i < Volume(Dim));

  return i;
}

inline s32
GetIndex(v3 Offset, chunk_dimension Dim)
{
  s32 Index = GetIndex( Voxel_Position(Offset), Dim);
  return Index;
}

#if DEBUG_OPTIMIZE_TRI_COUNT
inline bool
IsFacingPoint( v3 FaceToPoint, v3 FaceNormal )
{
  bool Result = false;

  if ( Dot(FaceToPoint, FaceNormal) > 0 )
  {
    Result = true;
  }
  return Result;
}
#endif

inline b32
IsFilled( chunk_data *chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  int i = GetIndex(VoxelP, Dim);

  Assert(i > -1);
  Assert(i < Volume(Dim));

  b32 isFilled = IsSet(&chunk->Voxels[i], Voxel_Filled);
  return isFilled;
}

inline b32
NotFilled(chunk_data *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 Result = !IsFilled(Chunk, VoxelP, Dim);
  return Result;
}

inline voxel_position
ClampNegative( voxel_position V )
{
  voxel_position Result = V;

  if ( V.x > 0 )
    Result.x = 0;

  if ( V.y > 0 )
    Result.y = 0;

  if ( V.z > 0 )
    Result.z = 0;

  return Result;
}

inline voxel_position
ClampPositive( voxel_position V )
{
  voxel_position Result = V;

  if ( V.x < 0 )
    Result.x = 0;

  if ( V.y < 0 )
    Result.y = 0;

  if ( V.z < 0 )
    Result.z = 0;

  return Result;
}

inline world_position
GetAbsoluteP( world_position P, chunk_dimension WorldChunkDim)
{
  world_position Result = World_Position((WorldChunkDim.x*P.x), (WorldChunkDim.y*P.y), (WorldChunkDim.z*P.z));
  return Result;
}

// FIXME(Jesse): this is misguided for-sure.  Will certainly fail when entities
// are far from the origin due to floating point percision issues.  Change
// to a view-space computation.
inline v3
GetAbsoluteP( canonical_position CP, chunk_dimension WorldChunkDim)
{
  v3 Result = V3(CP.Offset.x+(WorldChunkDim.x*CP.WorldP.x),
                 CP.Offset.y+(WorldChunkDim.y*CP.WorldP.y),
                 CP.Offset.z+(WorldChunkDim.z*CP.WorldP.z));
  return Result;
}

inline aabb
GetAABB(entity *Entity, chunk_dimension WorldChunkDim)
{
  v3 Radius = Entity->CollisionVolumeRadius;
  v3 Center = GetAbsoluteP(Entity->P, WorldChunkDim) + Radius;

  aabb Result(Center, Radius);
  return Result;
}

inline b32
IsInsideDim( voxel_position Dim, voxel_position P )
{
  b32 Result = False;

  Result = (
              P.x >= 0 &&
              P.y >= 0 &&
              P.z >= 0 &&

              P.x < Dim.x &&
              P.y < Dim.y &&
              P.z < Dim.z
           );

  return Result;
}

inline b32
IsInsideDim( voxel_position Dim, v3 P )
{
  b32 Result = IsInsideDim(Dim, Voxel_Position(P) );
  return Result;
}
