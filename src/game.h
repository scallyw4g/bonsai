#ifndef GAME_H
#define GAME_H

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

enum frame_event_type
{
  FrameEvent_Undefined,

  FrameEvent_Spawn,
  FrameEvent_Unspawn,
  FrameEvent_Explosion,
  FrameEvent_Deactivate,
};

struct frame_event
{
  frame_event_type Type;
  entity *Entity;

  frame_event *Next;

  frame_event(entity *Entity, frame_event_type Type)
  {
    this->Entity = Entity;
    this->Type = Type;
    this->Next = 0;
  }
};

struct event_queue
{
  u64 CurrentFrameIndex;
  frame_event **Queue;

  frame_event *FirstFreeEvent;
};

struct game_state
{
  World         *world;
  platform      *Plat;
  entity        *Player;
  Camera_Object *Camera;

  RenderGroup             *RG;
  ShadowRenderGroup       *SG;

  memory_arena *Memory;

  model   *Models;

  random_series Entropy;

  event_queue EventQueue;

  entity *EntityTable[TOTAL_ENTITY_COUNT];
};

#endif
