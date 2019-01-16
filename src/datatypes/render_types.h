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

debug_global float
g_quad_vertex_buffer_data[] =
{
  -1.0f, -1.0f, 1.0f,
   1.0f, -1.0f, 1.0f,
  -1.0f,  1.0f, 1.0f,
  -1.0f,  1.0f, 1.0f,
   1.0f, -1.0f, 1.0f,
   1.0f,  1.0f, 1.0f,
};

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

#define SSAO_KERNEL_SIZE 32
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
};

struct untextured_3d_geometry_buffer
{
  v3 *Verts;
  v4 *Colors;
  v3 *Normals;

  u32 End;
  u32 At;
};

struct gpu_mapped_element_buffer
{
  u32 VertexHandle;
  u32 NormalHandle;
  u32 ColorHandle;

  untextured_3d_geometry_buffer Buffer;
};

struct textured_2d_geometry_buffer
{
  v3 *Verts;
  v3 *Colors;
  v2 *UVs;

  u32 End;
  u32 At;
};

struct untextured_2d_geometry_buffer
{
  v3 *Verts;
  v3 *Colors;

  u32 End;
  u32 At;
};

struct debug_text_render_group
{
  framebuffer FBO;

  u32 SolidUIVertexBuffer;
  u32 SolidUIColorBuffer;
  u32 SolidUIUVBuffer;

  texture *FontTexture;
  shader DebugFontTextureShader;

  texture *CompositedTexture;
  shader DebugTextureShader;

  shader Text2DShader;
  s32 TextTextureUniform;
  textured_2d_geometry_buffer TextGeo;

  shader SolidUIShader;
  untextured_2d_geometry_buffer UIGeo;
};

global_variable m4 IdentityMatrix = {V4(1, 0, 0 ,0),
                                     V4(0, 1, 0 ,0),
                                     V4(0, 0, 1 ,0),
                                     V4(0, 0, 0 ,0)};

untextured_3d_geometry_buffer
Untextured3dGeometryBuffer(v3* Verts, v4* Colors, v3* Normals, u32 Count)
{
  untextured_3d_geometry_buffer Result = {};
  Result.Verts = Verts;
  Result.Colors = Colors;
  Result.Normals = Normals;
  Result.At = Count;

  return Result;
}

