#ifndef GAME_H
#define GAME_H

struct game_state
{
  World *world;
  platform *Plat;
  Entity *Player;
  Camera_Object *Camera;

  RenderGroup *RG;
  ShadowRenderGroup *SG;
};

#endif
