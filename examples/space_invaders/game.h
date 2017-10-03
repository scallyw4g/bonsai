#ifndef GAME_H
#define GAME_H

struct game_state
{
  world         *World;
  platform      *Plat;
  entity        *Player;
  camera        *Camera;

  g_buffer_render_group *gBuffer;
  ao_render_group *AoGroup;
  shadow_render_group     *SG;

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
