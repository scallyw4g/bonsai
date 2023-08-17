#define DEFAULT_LINE_THICKNESS (0.1f)

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

struct RenderBasis
{
  m4 ModelMatrix;
  m4 ViewMatrix;
  m4 ProjectionMatrix;
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

  shader DebugColorShader;
  shader DebugNormalShader;
  shader DebugPositionShader;

  /* shader LightingShader; */
  shader gBufferShader;

  m4 ViewProjection;
};

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

/* TODO(Jesse, id: 84, tags: opengl, completeness, engine): This only gets used
 * when computing the shadow map, so I'm not even sure if it works ATM
 */
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
// https://gist.github.com/gyng/8921328
//
// Projection
//
//   a 0 0 0
//   0 b 0 0
//   0 0 c d
//   0 0 e 0
//
// Inv. Projection
//
//   1/a   0   0        0
//     0 1/b   0        0
//     0   0   0      1/e
//     0   0 1/d -c/(d*e)
#endif

  r32 FocalLength = (r32)tan(r64(FOV) / 2.0);
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
ProjectionMatrix(camera *Camera, r32 WindowWidth, r32 WindowHeight)
{
  m4 Result = Perspective( Rads(Camera->Frust.FOV),
                                V2(WindowWidth, WindowHeight),
                                Camera->Frust.nearClip,
                                Camera->Frust.farClip);

  return Result;
}

untextured_3d_geometry_buffer
ReserveBufferSpace(untextured_3d_geometry_buffer* Reservation, u32 ElementsToReserve);

link_internal gpu_mapped_element_buffer *
GetCurrentGpuMap(graphics *Graphics);
