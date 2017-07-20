#ifndef PLAYGROUNDH
#define PLAYGROUNDH


#include <bonsai_types.h>
#include <platform.h>

#include <bonsai_math.h>

#include <perlin.h>
#include <stdio.h>
#include <bonsai_types.h>
#include <debug.h>
#include <constants.hpp>

enum chunk_flag
{
  Chunk_Uninitialized           = 0 << 0,
  Chunk_Initialized             = 1 << 1,

  Chunk_RebuildBoundary         = 1 << 2,
  Chunk_Queued                  = 1 << 3,
  Chunk_Garbage                 = 1 << 4,
  Chunk_Collected               = 1 << 5,
  /* Chunk_LodGenerated            = 1 << 6, */
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
  EntityState_Destoryed        = 1 << 2,
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

GLOBAL_VARIABLE const entity_type ENTITY_TYPES = (entity_type)
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
  Collision_Player_Enemy           = EntityType_Player|EntityType_Enemy,
  Collision_Player_EnemyProjectile = EntityType_Player|EntityType_EnemyProjectile,
  Collision_Player_Loot            = EntityType_Player|EntityType_Loot,
  Collision_Enemy_PlayerProjectile = EntityType_Enemy |EntityType_PlayerProjectile,
  Collision_Enemy_PlayerProton     = EntityType_Enemy |EntityType_PlayerProton,
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

struct mesh_buffer_target
{
  s32 bytesAllocd;
  s32 filled;

  s32 VertexCount;

  r32* VertexData;
  r32* ColorData;
  r32* NormalData;
};

struct chunk_data
{
  chunk_flag Flags;

  s32 BoundaryVoxelCount;

  mesh_buffer_target Mesh;

  voxel *Voxels;
  boundary_voxel *BoundaryVoxels;
};

#if 0
struct mesh_block
{
  v3 Meshes[64];
  mesh_block *Next;
};
#endif

struct world_chunk
{
  chunk_data *Data;
  v3 *Mesh;

  // TODO(Jesse): This is only for looking up chunks in the hashtable and
  // should be factored out of this struct somehow as it's cold data
  world_chunk *Next;
  world_chunk *Prev;

  world_position WorldP;

  point_buffer PB;
  v3 Normal;

  s32 Filled;
};

struct free_world_chunk
{
  world_chunk *chunk_data;
  world_chunk *Next;
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

struct Frustum
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

struct Camera_Object
{
  Frustum Frust;

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
  u32 Seed;
};

struct particle
{
  // TODO(Jesse): Compress to 16 bit float?
  v3 Offset;
  v3 Velocity;

  u8 Color;
  r32 RemainingLifespan;
};

struct particle_system
{
  random_series Entropy;

  s32 ActiveParticles;
  r32 ParticleDuration;
  r32 ParticleSpeed;

  v3 InitialVelocity;

  aabb SpawnRegion;

  b32 Active;

  particle Particles[PARTICLES_PER_SYSTEM];
};

struct entity
{
  model Model;
  v3 CollisionVolumeRadius;

  particle_system *Emitter;

  v3 Velocity;
  v3 Acceleration;
  v3 Drag;

  canonical_position P;

  Quaternion Rotation;

  entity_state State;
  entity_type Type;

  r32 Scale;

   // TODO(Jesse): Unneeded for projectiles. factor out of here?
  r32 RateOfFire;
  r32 FireCooldown;

  u32 Health; // Only needed for Player
};

struct World
{
  world_chunk **ChunkHash;

  world_chunk **FreeChunks;
  int FreeChunkCount;

  // This is the number of chunks in xyz we're going to update and render
  chunk_dimension VisibleRegion;
  chunk_dimension ChunkDim;

  world_position Center;

  v3 Gravity;

  mesh_buffer_target Mesh;
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
  b32 Result = System->Active;
  return Result;
}

inline b32
Destroyed(entity *Entity)
{
  b32 Result = Entity->State == EntityState_Destoryed;
  return Result;
}

inline b32
Inactive(particle_system *System)
{
  b32 Result = !Active(System);
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
  u32 A = 1664525;
  u32 B = 1013904223;

  // TODO(Jesse): This is LCG RNG - do we want a better one?
  //
  // Found the A and B values at:
  // http://www.lomont.org/Math/Papers/2008/Lomont_PRNG_2008.pdf
  //
  Entropy->Seed = ((A * Entropy->Seed) + B) % UINT_MAX;

  return Entropy->Seed;
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
ZeroChunk( chunk_data *chunk, s32 Volume )
{
  chunk->BoundaryVoxelCount = 0;
  chunk->Flags = Chunk_Uninitialized;
  SetFlag( chunk, Chunk_RebuildBoundary );

  for ( s32 VoxelIndex = 0;
        VoxelIndex < Volume;
        ++VoxelIndex)
  {
    voxel *Voxel = &chunk->Voxels[VoxelIndex];
    Voxel->Flags = Voxel_Uninitialzied;
    Voxel->Color = 0;

    boundary_voxel *BoundaryVoxel = &chunk->BoundaryVoxels[VoxelIndex];
    BoundaryVoxel->V = *Voxel;
    BoundaryVoxel->Offset = Voxel_Position(0,0,0);
  }

  return;
}

unsigned int
GetWorldChunkHash(world_position P)
{
  // TODO(Jesse): Better hash function!

  unsigned int i =
    (P.x) +
    (P.y*CD_X) +
    (P.z*CD_X*CD_Y);

  unsigned int HashIndex = (i * 42 * 13 * 233) % WORLD_HASH_SIZE;
  Assert(HashIndex < WORLD_HASH_SIZE);

  return HashIndex;
}

void
FreeWorldChunk(World *world, world_chunk *chunk)
{
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
      world->ChunkHash[GetWorldChunkHash(chunk->WorldP)] = chunk->Next;
    }

    chunk->Prev = 0;
    chunk->Next = 0;
    chunk->PB.Count = 0;

    Assert(world->FreeChunkCount < FREELIST_SIZE);
    world->FreeChunks[world->FreeChunkCount++] = chunk;

    ZeroChunk(chunk->Data, Volume(WORLD_CHUNK_DIM));

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

chunk_data*
AllocateChunk(memory_arena *WorldStorage, chunk_dimension Dim)
{
  chunk_data *Result = PUSH_STRUCT_CHECKED(chunk_data, WorldStorage, 1);;

  Result->Voxels          = PUSH_STRUCT_CHECKED(voxel, WorldStorage , Volume(Dim));
  Result->BoundaryVoxels  = PUSH_STRUCT_CHECKED(boundary_voxel, WorldStorage , Volume(Dim));

  ZeroChunk(Result, Volume(Dim));

  return Result;
}

void
InsertChunkIntoWorld(World *world, world_chunk *chunk)
{
  unsigned int HashIndex = GetWorldChunkHash(chunk->WorldP);
  world_chunk *Last = world->ChunkHash[HashIndex];;

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
    world->ChunkHash[HashIndex] = chunk;
  }

  return;
}

world_chunk*
AllocateWorldChunk(memory_arena *Storage, World *world, world_position WorldP)
{
  world_chunk *Result = PUSH_STRUCT_CHECKED(world_chunk, Storage, 1);

  Result->Data = AllocateChunk(Storage, Chunk_Dimension(CD_X, CD_Y, CD_Z));
  Assert(Result->Data);

  Result->WorldP = WorldP;

  InsertChunkIntoWorld(world, Result);

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
GetWorldChunk( World *world, world_position P )
{
  unsigned int HashIndex = GetWorldChunkHash(P);
  world_chunk *Result = world->ChunkHash[HashIndex];

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
IsFilledInWorld( World *world, world_chunk *chunk, canonical_position VoxelP )
{
  b32 isFilled = true;

  if ( chunk )
  {
    world_chunk *localChunk = chunk;

    if ( chunk->WorldP != VoxelP.WorldP )
    {
      localChunk = GetWorldChunk(world, VoxelP.WorldP);
    }

    isFilled = localChunk && IsFilledInChunk(localChunk->Data, Voxel_Position(VoxelP.Offset), world->ChunkDim );
  }

  return isFilled;
}

inline b32
NotFilledInWorld( World *world, world_chunk *chunk, canonical_position VoxelP )
{
  b32 Result = !(IsFilledInWorld(world, chunk, VoxelP));
  return Result;
}

inline world_position
GetAbsoluteP( world_position P )
{
  world_position Result = World_Position((CD_X*P.x), (CD_Y*P.y), (CD_Z*P.z));
  return Result;
}

inline v3
GetAbsoluteP( canonical_position CP )
{
  v3 Result = V3(CP.Offset.x+(CD_X*CP.WorldP.x),
                 CP.Offset.y+(CD_Y*CP.WorldP.y),
                 CP.Offset.z+(CD_Z*CP.WorldP.z));
  return Result;
}

inline aabb
GetAABB(entity *Entity)
{
  v3 Radius = Entity->CollisionVolumeRadius;
  v3 Center = GetAbsoluteP(Entity->P) + Radius;

  aabb Result(Center, Radius);
  return Result;
}

#endif
