struct light
{
  v3 Position;
  v3 Color;
};

struct game_lights
{
  texture *ColorTex;
  texture *PositionTex;

  r32 IndexToUV;

  s32 Count; // Must be signed because of the shader
  light *Lights;
};

struct shadow_render_group
{
  u32 FramebufferName;
  s32 MVP_ID;

  shader DepthShader;

  m4 MVP;

  texture *ShadowMap;
  light Sun;
};

struct lighting_render_group
{
  game_lights Lights;

  // This has both the Lighting and Bloom textures attached
  framebuffer FBO;

  // NOTE(Jesse): This is an FBO with only the Bloom texture attached; for
  // writing the final result of the gaussian blur to
  framebuffer BloomTextureFBO;

  texture *LightingTex;
  texture *BloomTex;

  shader Shader;

  shader DebugBloomShader;
  shader DebugLightingShader;
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

#define LUMINANCE_MAP_RESOLUTION_X (SCR_WIDTH)
#define LUMINANCE_MAP_RESOLUTION_Y (SCR_HEIGHT)

// TODO(Jesse): Make a shadow.h ?
// NOTE(Jesse): 32x1024 x2 == 64k at 32bit float
#define SHADOW_MAP_RESOLUTION_X (16*1024)
#define SHADOW_MAP_RESOLUTION_Y (16*1024)
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

