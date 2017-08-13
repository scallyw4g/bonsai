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
  FrameEvent_GameModeLoss,
  FrameEvent_GameModePlaying,
  FrameEvent_GameModeWon,
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

enum game_mode_type
{
  GameMode_Playing,
  GameMode_Won,
  GameMode_Loss
};

struct game_mode
{
  game_mode_type ActiveMode;
  r64 TimeRunning;
};

struct game_state
{
  world         *World;
  platform      *Plat;
  entity        *Player;
  camera        *Camera;

  RenderGroup             *RG;
  ShadowRenderGroup       *SG;

  memory_arena *Memory;

  model   *Models;

  noise_3d *Turb;

  random_series Entropy;
  game_mode Mode;

  event_queue EventQueue;

  logical_frame_state Frame;

  aabb *FolieTable;

  entity *EntityTable[TOTAL_ENTITY_COUNT];
};

#endif
