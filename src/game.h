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

  entity *Enemies[TOTAL_ENTITY_COUNT];
  projectile *Projectiles[TOTAL_PROJECTILE_COUNT];

  particle_system *ParticleSystems[TOTAL_PARTICLE_SYSTEMS];
};


#endif
