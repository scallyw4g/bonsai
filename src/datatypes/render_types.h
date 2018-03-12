enum shader_uniform_type
{
  ShaderUniform_Undefined,
  ShaderUniform_M4,
  ShaderUniform_V3,
  ShaderUniform_S32,
  ShaderUniform_U32,
  ShaderUniform_R32,
  ShaderUniform_Texture,
  ShaderUniform_Light,
  ShaderUniform_Camera,
};

struct texture;
struct light;
struct camera;
struct shader_uniform
{
  shader_uniform_type Type;

  s32 ID;

  union {
    texture *Texture;
    light *Light;
    m4 *M4;
    v3 *V3;
    s32 *S32;
    u32 *U32;
    r32 *R32;
    camera *Camera;
    void *Data;
  };

  const char *Name;
  shader_uniform *Next;
};

struct shader
{
  u32 ID;
  shader_uniform *FirstUniform;
};

struct texture
{
  u32 ID;
  v2i Dim;
};

debug_global float g_quad_vertex_buffer_data[] =
{
  -1.0f, -1.0f, 1.0f,
   1.0f, -1.0f, 1.0f,
  -1.0f,  1.0f, 1.0f,
  -1.0f,  1.0f, 1.0f,
   1.0f, -1.0f, 1.0f,
   1.0f,  1.0f, 1.0f,
};

enum light_type
{
  LightType_Undefined,

  LightType_Directional,
  LightType_Spot,
  LightType_Point,
};

struct light
{
  light_type Type;
  v3 Position;
};

struct game_lights
{
  s32 Count;
  light *Lights;

  light *
  operator[](s32 Index)
  {
    light *Result = &this->Lights[Index];
    return Result;
  }
};

struct RenderBasis
{
  m4 ModelMatrix;
  m4 ViewMatrix;
  m4 ProjectionMatrix;
};

struct framebuffer
{
  u32 ID;
  u32 Attachments;
};

#define SSAO_KERNEL_SIZE 8
struct ao_render_group
{
  framebuffer FBO;
  shader Shader;
  shader DebugSsaoShader;

  v3 SsaoKernel[SSAO_KERNEL_SIZE]; // Could just be pushed on the heap
  u32 SsaoKernelUniform; // FIXME(Jesse): Automate me!

  texture *Texture;

  v3 NoiseTile;
};

struct g_buffer_textures
{
  texture *Color;
  texture *Normal;
  texture *Position;
};

struct g_buffer_render_group
{
  framebuffer FBO;

  g_buffer_textures *Textures;

  u32 colorbuffer;
  u32 vertexbuffer;
  u32 normalbuffer;

  shader LightingShader;
  shader gBufferShader;

  // Cached here because this one gets pre-multiplied by the NdcToScreenSpace bias matrix
  m4 ShadowMVP;
  m4 ViewProjection;

#if BONSAI_INTERNAL
  shader DebugColorTextureShader;
  shader DebugNormalTextureShader;
  shader DebugPositionTextureShader;
#endif
};

struct shadow_render_group
{
  u32 FramebufferName;
  u32 MVP_ID;

  shader DebugTextureShader;
  shader DepthShader;

  texture *ShadowMap;

  game_lights GameLights;
};

struct untextured_3d_geometry_buffer
{
  v3 *Verts;
  v3 *Colors;
  v3 *Normals;

  s32 Allocated;
  s32 CurrentIndex;
};

struct textured_2d_geometry_buffer
{
  v3 *Verts;
  v3 *Colors;
  v2 *UVs;

  u32 Allocated;
  u32 CurrentIndex;
};

struct untextured_2d_geometry_buffer
{
  v3 *Verts;
  v3 *Colors;

  u32 Allocated;
  u32 CurrentIndex;
};

struct debug_text_render_group
{
  framebuffer FBO;

  texture FontTexture;
  texture *CompositedTexture;

  u32 SolidUIVertexBuffer;
  u32 SolidUIColorBuffer;

  u32 VertexBuffer;
  u32 UVBuffer;
  u32 ColorBuffer;

  shader Text2DShader;
  textured_2d_geometry_buffer TextGeo;

  shader SolidUIShader;
  untextured_2d_geometry_buffer UIGeo;

  shader DebugFontTextureShader;
  shader DebugTextureShader;

  u32 TextureUniformID;
};
