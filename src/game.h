#ifndef GAME_H
#define GAME_H

enum model_index
{
  ModelIndex_Enemy,
  ModelIndex_Player,
  ModelIndex_Loot,
  ModelIndex_Projectile,
  ModelIndex_Proton,

  ModelIndex_Count,
};

enum frame_event_type
{
  FrameEvent_Spawn,
  FrameEvent_Unspawn,
};

struct frame_event
{
  frame_event_type Type;
  canonical_position P;

  void *Input;

  frame_event *Next;
};

struct event_queue
{
  s32 CurrentFrameIndex;
  frame_event **Queue;

  frame_event *FirstFreeEvent;
};

struct game_state
{
  World                   *world;
  platform                *Plat;
  entity                  *Player;
  Camera_Object           *Camera;

  RenderGroup             *RG;
  ShadowRenderGroup       *SG;
  debug_text_render_group *DebugRG;

  memory_arena            *Memory;

  model                   *Models;

  event_queue EventQueue;

  random_series Entropy;

  entity *Enemies[TOTAL_ENTITY_COUNT];
  projectile *Projectiles[TOTAL_PROJECTILE_COUNT];
};


#endif
