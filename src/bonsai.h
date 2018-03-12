#include <bonsai_math.h>

#include <perlin.h>
#include <stdio.h>
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

  Chunk_BufferMesh      = 1 << 2,
  Chunk_Queued          = 1 << 3,
  Chunk_Garbage         = 1 << 4,
  Chunk_Collected       = 1 << 5,
  /* Chunk_LodGenerated = 1 << 6, */
};

enum voxel_flag
{
  Voxel_Uninitialzied =   0,
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

  boundary_voxel(s32 x, s32 y, s32 z, s32 w)
  {
    this->Offset.x = x;
    this->Offset.y = y;
    this->Offset.z = z;

    this->V.Color = w;

    this->V.Flags = Voxel_Uninitialzied;
  }

  boundary_voxel(voxel *V, voxel_position Offset)
  {
    this->V = *V;
    this->Offset = Offset;
  }
};

struct noise_3d
{
  voxel *Voxels;
  chunk_dimension Dim;
};

struct chunk_data
{
  chunk_flag Flags;
  untextured_3d_geometry_buffer Mesh;
  voxel *Voxels;
};

struct collision_event
{
  canonical_position CP;
  bool didCollide;
};

struct plane
{
  v3 P;
  v3 Normal;
  float d;

  plane( v3 P, v3 Normal )
  {
    this->P = P;
    this->Normal = Normal;

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

  canonical_position P;
  canonical_position Target; // TODO : Can this just be a v3?

  v3 Front;
  v3 Right;
  v3 Up;
};

struct model
{
  chunk_data *Chunk;
  chunk_dimension Dim;
};

struct random_series
{
  u64 Seed;

  random_series(u64 Seed = DEFAULT_ENTROPY_SEED)
  {
    this->Seed = Seed;
  }
};

struct physics
{
  v3 Velocity;
  v3 Force;

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

  r32 EmissionLifespan;
  r32 EmissionChance;

  r32 ParticleLifespan;

  random_series Entropy;

  u8 Colors[PARTICLE_SYSTEM_COLOR_COUNT];
};

struct particle_system
{
  random_series Entropy;

  s32 ActiveParticles;
  r32 ParticleDuration;

  aabb SpawnRegion;

  r32 EmissionLifespan;

  r32 ParticleLifespan;
  r32 EmissionChance;
  physics ParticlePhysics;

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

  frame_event(frame_event_type Type)
  {
    this->Type = Type;
    this->Entity = 0;
    this->Next = 0;
  }

  frame_event(entity *Entity, frame_event_type Type)
  {
    this->Type = Type;
    this->Entity = Entity;
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

struct world_chunk
{
  chunk_data *Data;

  // TODO(Jesse): This is only for looking up chunks in the hashtable and
  // should be factored out of this struct somehow as it's cold data
  world_chunk *Next;
  world_chunk *Prev;

  world_position WorldP;

  v3 Normal;

  /* s32 Filled; */

  entity_list *Occupiers;
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

  v3 Gravity;

  untextured_3d_geometry_buffer Mesh;
};



#include <render.h>



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
SetFlag( model *Model, chunk_flag Flag )
{
  SetFlag(Model->Chunk, Flag);
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
  u64 Mod = (1L << 63);

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
  Mesh->CurrentIndex = 0;
  return;
}

void
ZeroChunk( chunk_data *Chunk, s32 Volume )
{
  ZeroMesh(&Chunk->Mesh);

  Chunk->Flags = Chunk_Uninitialized;
  SetFlag( Chunk, Chunk_BufferMesh );

  // TODO(Jesse): Pretty sure this is not necessary
  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume;
        ++VoxelIndex)
  {
    voxel *Voxel = &Chunk->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Uninitialzied;
    Voxel->Color = 0;
  }

  return;
}

u32
GetWorldChunkHash(world_position P)
{
  TIMED_FUNCTION();
  // TODO(Jesse): Better hash function!
  u32 i =
    (P.x) +
    (P.y*Global_WorldChunkDim.x) +
    (P.z*Global_WorldChunkDim.x*Global_WorldChunkDim.y);

  u32 HashIndex = i % WORLD_HASH_SIZE;
  Assert(HashIndex < WORLD_HASH_SIZE);

  return HashIndex;
}

void
FreeWorldChunk(world *World, world_chunk *chunk)
{
  TIMED_FUNCTION();

  // Only free chunks that have been initialized, or chunks that had previously
  // been marked as garbage and have been flushed all the way through the world
  // initialization queue.
  if ( IsSet(chunk->Data->Flags, Chunk_Initialized) || IsSet(chunk->Data->Flags, Chunk_Collected) )
  {
    // Unlink from middle of linked list
    if (chunk->Prev)
    {
      chunk->Prev->Next = chunk->Next;
    }

    if (chunk->Next)
    {
      chunk->Next->Prev = chunk->Prev;
    }

    // Unlink from head end of linked list
    if (!chunk->Prev)
    {
      World->ChunkHash[GetWorldChunkHash(chunk->WorldP)] = chunk->Next;
    }

    chunk->Prev = 0;
    chunk->Next = 0;

    Assert(World->FreeChunkCount < FREELIST_SIZE);
    World->FreeChunks[World->FreeChunkCount++] = chunk;

    ZeroChunk(chunk->Data, Volume(World->ChunkDim));

    Assert( NotSet(chunk->Data->Flags, Chunk_Initialized) );
    Assert( NotSet(chunk->Data->Flags, Chunk_Queued) );
  }
  else
  {
    SetFlag(chunk, Chunk_Garbage);
  }

  return;
}

inline s32
GetIndex(voxel_position P, chunk_data *chunk, chunk_dimension Dim)
{
  s32 i =
    (P.x) +
    (P.y*Dim.x) +
    (P.z*Dim.x*Dim.y);

  Assert(i < Volume(Dim));

  return i;
}

inline s32
GetIndex(v3 Offset, chunk_data *Chunk, chunk_dimension Dim)
{
  s32 Index = GetIndex( Voxel_Position(Offset), Chunk, Dim);
  return Index;
}

void
AllocateMesh(untextured_3d_geometry_buffer *Mesh, u32 NumVerts, memory_arena *Memory)
{
  Mesh->Verts = PUSH_STRUCT_CHECKED(v3, Memory, NumVerts );
  Mesh->Colors = PUSH_STRUCT_CHECKED(v3,  Memory, NumVerts );
  Mesh->Normals = PUSH_STRUCT_CHECKED(v3, Memory, NumVerts );

  Mesh->Allocated = NumVerts;
  Mesh->CurrentIndex = 0;
}

chunk_data*
AllocateChunk(memory_arena *WorldStorage, chunk_dimension Dim)
{
  chunk_data *Result = PUSH_STRUCT_CHECKED(chunk_data, WorldStorage, 1);;
  Result->Voxels = PUSH_STRUCT_CHECKED(voxel, WorldStorage , Volume(Dim));

  // TODO(Jesse): Allocate this based on actual need?
  AllocateMesh(&Result->Mesh, 15000, WorldStorage);

  ZeroChunk(Result, Volume(Dim));

  return Result;
}

void
InsertChunkIntoWorld(world *World, world_chunk *chunk)
{
  u32 HashIndex = GetWorldChunkHash(chunk->WorldP);
  world_chunk *Last = World->ChunkHash[HashIndex];;

  if (Last)
  {
    // Assert(Last->WorldP != chunk->WorldP);

    while (Last->Next)
    {
      // Assert(Last->WorldP != chunk->WorldP);
      Last = Last->Next;
    }

    Last->Next = chunk;
    chunk->Prev = Last;
  }
  else
  {
    World->ChunkHash[HashIndex] = chunk;
  }

  return;
}

world_chunk*
AllocateWorldChunk(memory_arena *Storage, world *World, world_position WorldP)
{
  world_chunk *Result = PUSH_STRUCT_CHECKED(world_chunk, Storage, 1);

  Result->Data = AllocateChunk(Storage, World->ChunkDim);
  Assert(Result->Data);

  Result->WorldP = WorldP;

  InsertChunkIntoWorld(World, Result);

  return Result;
}

inline bool
IsFacingPoint( v3 FaceToPoint, v3 FaceNormal )
{
  bool Result = false;

#if DEBUG_OPTIMIZE_TRI_COUNT
  if ( Dot(FaceToPoint, FaceNormal) > 0 )
  {
    Result = true;
  }
#else
  Result=true;
#endif

  return Result;
}

world_chunk*
GetWorldChunk( world *World, world_position P )
{
  TIMED_FUNCTION();
  u32 HashIndex = GetWorldChunkHash(P);
  world_chunk *Result = World->ChunkHash[HashIndex];

  while (Result)
  {
    if ( Result->WorldP == P )
        break;

    Result = Result->Next;
  }

  if (Result)
  {
    Assert(Result->WorldP == P)
  }

  return Result;
}

inline b32
IsFilled( chunk_data *chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  int i = GetIndex(VoxelP, chunk, Dim);

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

inline b32
IsFilledInChunk( chunk_data *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 isFilled = True;

  if (Chunk && IsSet(Chunk, Chunk_Initialized) )
  {
    s32 i = GetIndex(VoxelP, Chunk, Dim);

    Assert(i > -1);
    Assert(i < Volume(Dim));

    isFilled = IsSet(&Chunk->Voxels[i], Voxel_Filled);
  }

  return isFilled;
}

inline b32
NotFilledInChunk( chunk_data *Chunk, voxel_position VoxelP, chunk_dimension Dim)
{
  b32 Result = !IsFilledInChunk(Chunk, VoxelP, Dim);
  return Result;
}

inline b32
IsFilledInWorld( world *World, world_chunk *chunk, canonical_position VoxelP )
{
  TIMED_FUNCTION();
  b32 isFilled = true;

  if ( chunk )
  {
    world_chunk *localChunk = chunk;

    if ( chunk->WorldP != VoxelP.WorldP )
    {
      localChunk = GetWorldChunk(World, VoxelP.WorldP);
    }

    isFilled = localChunk && IsFilledInChunk(localChunk->Data, Voxel_Position(VoxelP.Offset), World->ChunkDim );
  }

  return isFilled;
}

inline b32
NotFilledInWorld( world *World, world_chunk *chunk, canonical_position VoxelP )
{
  TIMED_FUNCTION();
  b32 Result = !(IsFilledInWorld(World, chunk, VoxelP));
  return Result;
}

inline world_position
GetAbsoluteP( world_position P, chunk_dimension WorldChunkDim)
{
  world_position Result = World_Position((WorldChunkDim.x*P.x), (WorldChunkDim.y*P.y), (WorldChunkDim.z*P.z));
  return Result;
}

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
