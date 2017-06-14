#ifndef GAME_H
#define GAME_H

struct game_state
{
  World                   *world;
  platform                *Plat;
  entity                  *Player;
  Camera_Object           *Camera;

  RenderGroup             *RG;
  ShadowRenderGroup       *SG;
  debug_text_render_group *DebugRG;

  entity Enemies[TOTAL_ENEMY_COUNT];
};


#endif
