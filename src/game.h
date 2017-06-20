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

  entity *Entities[TOTAL_ENEMY_COUNT];
  trigger *Triggers[TOTAL_TRIGGER_COUNT];
};


#endif
