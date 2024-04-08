struct light
{
  v3 Position;
  v3 Color;
};

struct game_lights
{
  texture ColorTex;
  texture PositionTex;

  r32 IndexToUV;

  // Must be signed because of the shader
  //
  s32 Count; poof(@no_ui) // Don't display this in the UI; it's wrong.  The lights get reset before the UI draws.
  light *Lights;
};

struct shadow_render_group
{
  u32 FramebufferName;
  s32 MVP_ID;

  shader DepthShader;

  m4 MVP;

  texture ShadowMap;
};



struct lighting_render_group
{
  game_lights Lights;

  framebuffer FBO;
  texture     LuminanceTex;
  shader      Shader;

  bloom_render_group Bloom;
};

link_internal void
DoLight(game_lights *Lights, v3 RenderPosition, v3 Color)
{
  u32 ThisLightIndex = AtomicIncrement((volatile u32 *)&Lights->Count) - 1;
  if (ThisLightIndex < MAX_LIGHTS)
  {
    light *Light = Lights->Lights + ThisLightIndex;
    Light->Position = RenderPosition;
    Light->Color = Color;
  }
  else
  {
    Warn("Too many lights in scene!");
  }
}

/* #define LUMINANCE_MAP_RESOLUTION_X (SCR_WIDTH) */
/* #define LUMINANCE_MAP_RESOLUTION_Y (SCR_HEIGHT) */

// TODO(Jesse): Make a shadow.h ?
// NOTE(Jesse): 32x1024 x2 == 64k at 32bit float
/* #define SHADOW_MAP_RESOLUTION_X ((16*1024)/SCREEN_RATIO) */
/* #define SHADOW_MAP_RESOLUTION_Y ((16*1024)/SCREEN_RATIO) */
/* #define SHADOW_MAP_RESOLUTION_X (2*1024) */
/* #define SHADOW_MAP_RESOLUTION_Y (2*1024) */

#define SHADOW_MAP_X 512
#define SHADOW_MAP_Y 512
#define SHADOW_MAP_Z_MIN -512
#define SHADOW_MAP_Z_MAX  512

/* #define SHADOW_MAP_X 512 */
/* #define SHADOW_MAP_Y 512 */
/* #define SHADOW_MAP_Z_MIN -512 */
/* #define SHADOW_MAP_Z_MAX  512 */

