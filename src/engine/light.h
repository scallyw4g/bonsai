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

  shader DebugTextureShader;
  shader DepthShader;

  m4 MVP;

  texture *ShadowMap;
  light Sun;
};

struct lighting_render_group
{
  game_lights Lights;
};

void
DoLight(game_lights *Lights, v3 Position, v3 Color)
{
  Assert(Lights->Count < MAX_LIGHTS);

  if (Lights->Count < MAX_LIGHTS)
  {
    light *Light = Lights->Lights + Lights->Count++;
    Light->Position = Position;
    Light->Color = Color;
  }

 return;
}

