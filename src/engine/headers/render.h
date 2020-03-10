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

// TODO(Jesse, tags: metaprogramming, immediate): d_union-ify this
struct shader_uniform
{
  shader_uniform_type Type;
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

  s32 ID;
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
  s32 SsaoKernelUniform; // FIXME(Jesse): Automate me!

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
  v3 *UVs;

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

inline void
SetViewport(v2 Dim)
{
  glViewport(0, 0, (s32)Dim.x, (s32)Dim.y);
  return;
}

// TODO(Jesse, tags: opengl, completeness, engine): This only gets used when computing the shadow map, so I'm not even sure if it works ATM
inline m4
Orthographic( r32 X, r32 Y, r32 Zmin, r32 Zmax)
{
  r32 r = X;
  r32 l = -X;

  r32 t = Y;
  r32 b = -Y;

  r32 f = Zmax;
  r32 n = Zmin;

  m4 Result = {
    V4(2/(r-l) , 0       , 0        , -1*((r+l)/(r-l)) ) ,
    V4(0       , 2/(t-b) , 0        , -1*((t+b)/(t-b)) ) ,
    V4(0       , 0       , -2/(f-n) , -1*((f+n)/(f-n)) ) ,
    V4(0       , 0       , 0        , 1)
  };

  return Result;
}

inline m4
Perspective(radians FOV, v2 WindowDim, r32 NearClip, r32 FarClip)
{

#if 0
  https://gist.github.com/gyng/8921328

  Projection

    a 0 0 0
    0 b 0 0
    0 0 c d
    0 0 e 0

  Inv. Projection

    1/a   0   0        0
      0 1/b   0        0
      0   0   0      1/e
      0   0 1/d -c/(d*e)
#endif

  r32 FocalLength = (r32)tan(FOV / 2.0f);
  r32 Aspect = WindowDim.x/WindowDim.y;

  r32 a = 1.0f/(Aspect*FocalLength);
  r32 b = 1.0f/FocalLength ;
  r32 c = (-(FarClip+NearClip)) / (FarClip-NearClip);
  r32 d = -1.0f;
  r32 e = (-(2.0f*FarClip*NearClip)) / (FarClip - NearClip);

  m4 Result = {
    V4(a, 0, 0, 0),
    V4(0, b, 0, 0),
    V4(0, 0, c, d),
    V4(0, 0, e, 0),
  };

#if 0
  m4 Inverse = {
    V4(1.0f/a , 0      , 0      , 0)        ,
    V4(0      , 1.0f/b , 0      , 0)        ,
    V4(0      , 0      , 0      , 1.0f/e)   ,
    V4(0      , 0      , 1.0f/d , -c/(d*e)) ,
  };
#endif

  return Result;
}

inline radians
Rads(degrees Degrees)
{
  radians Result = (Degrees/180);
  return Result;
}

inline m4
ProjectionMatrix(camera *Camera, int WindowWidth, int WindowHeight)
{
  m4 Result = Perspective( Rads(Camera->Frust.FOV),
                                          V2(WindowWidth, WindowHeight),
                                          Camera->Frust.nearClip,
                                          Camera->Frust.farClip);

  return Result;
}
