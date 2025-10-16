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


struct lighting_render_group
poof( @render_pass
      @vert_source_file(BONSAI_SHADER_PATH "Lighting.vertexshader")
      @frag_source_file(BONSAI_SHADER_PATH "Lighting.fragmentshader")
    )
{
  shader         Program;
  shader_uniform Uniforms[26];

  framebuffer FBO;

  texture *gColor;               poof (@uniform)
  texture *gNormal;              poof (@uniform)
  texture *gDepth;               poof (@uniform)
  texture *shadowMap;            poof (@uniform)
  texture *Ssao;                 poof (@uniform)

  texture *TransparencyAccumTex; poof (@uniform)
  texture *TransparencyCountTex; poof (@uniform)

  b32 *BravoilMyersOIT;          poof (@uniform)
  b32 *BravoilMcGuireOIT;        poof (@uniform)

  m4 *InverseViewMatrix;         poof (@uniform)
  m4 *InverseProjectionMatrix;   poof (@uniform)
  m4 *ShadowMVP;                 poof (@uniform)

  // from struct game_lights {
    texture *LightColors;        poof (@uniform)
    texture *LightPositions;     poof (@uniform)
    f32 *LightIndexToUV;         poof (@uniform)
    s32 *LightCount;             poof (@uniform)
  // }

  camera *Camera;                poof (@uniform)

   v3 *SunPosition;              poof (@uniform)
   v3 *SunColor;                 poof (@uniform)
   v3 *FogColor;                 poof (@uniform)
  f32 *FogPower;                 poof (@uniform)

  b32 *UseSsao;                  poof (@uniform)

  b32 *UseShadowMapping;         poof (@uniform)
  b32 *UseLightingBloom;         poof (@uniform)

  v2 *ApplicationResolution;      poof (@uniform)
  v2 *ShadowMapResolution;        poof (@uniform)


  texture LuminanceTex;
  game_lights Lights;
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

#define SHADOW_MAP_X 1024*4
#define SHADOW_MAP_Y 1024*4
#define SHADOW_MAP_Z_MIN -1024*4
#define SHADOW_MAP_Z_MAX  1024*4

/* #define SHADOW_MAP_X 512 */
/* #define SHADOW_MAP_Y 512 */
/* #define SHADOW_MAP_Z_MIN -512 */
/* #define SHADOW_MAP_Z_MAX  512 */

