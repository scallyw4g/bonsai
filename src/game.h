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
  void *Input;

  frame_event *Next;
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


  s32 CurrentFrame;
  frame_event **EventQueue;

  frame_event *FirstFreeEvent;


  random_series Entropy;

  entity *Enemies[TOTAL_ENTITY_COUNT];
  projectile *Projectiles[TOTAL_PROJECTILE_COUNT];
};


#endif
