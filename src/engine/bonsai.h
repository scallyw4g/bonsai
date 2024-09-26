
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

struct entity_entity_collision_event
{
  // TODO(Jesse)(poof): generate this with 'mixin'
  union
  {
    collision_event Collision;
    struct
    {
      u32 FrameIndex;
      u32 Count;
      canonical_position MinP;
      canonical_position MaxP;
    };
  };

  entity *Entity;
};

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
SetFlag( chunk_flag *Flags, chunk_flag Flag )
{
  /* Assert( (*Flags & Flag) == 0); */ // NOTE(Jesse): This should probably be in, but it's annoying ..
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



link_internal void
ClearWorldChunk( world_chunk *Chunk )
{
  Chunk->WorldP = INVALID_WORLD_CHUNK_POSITION;
  Chunk->FilledCount = {};
  /* Chunk->DrawBoundingVoxels = {}; */
  /* Chunk->PointsToLeaveRemaining = {}; */
  /* Chunk->TriCount = {}; */
  /* Chunk->EdgeBoundaryVoxelCount = {}; */
  Chunk->StandingSpots.At = Chunk->StandingSpots.Start;
  Chunk->Entities = {};
  Chunk->Next = {};

  Chunk->DEBUG_OwnedByThread = {};

  Clear(&Chunk->Meshes);

  Chunk->Flags = {};
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
