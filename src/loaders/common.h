struct r32_static_array
{
  r32* Start;
  r32* At;
  r32* End;
};

struct u32_static_array
{
  u32* Start;
  u32* At;
  u32* End;
};

struct v3_static_array
{
  v3* Start;
  v3* At;
  v3* End;
};

r32_static_array
R32_Static_Array(u32 Count, memory_arena *Memory)
{
  r32 *Elements = Allocate(r32, Memory, Count);
  r32_static_array Result = {};

  Result.Start = Elements;
  Result.At = Elements;
  Result.End = Elements + Count;

  return Result;
}

u32_static_array
U32_Static_Array(u32 Count, memory_arena *Memory)
{
  u32 *Elements = Allocate(u32, Memory, Count);
  u32_static_array Result = {};

  Result.Start = Elements;
  Result.At = Elements;
  Result.End = Elements + Count;

  return Result;
}

v3_static_array
V3_Static_Array(u32 Count, memory_arena *Memory)
{
  v3 *Elements = Allocate(v3, Memory, Count);
  v3_static_array Result = {};

  Result.Start = Elements;
  Result.At = Elements;
  Result.End = Elements + Count;

  return Result;
}

template <typename T, typename T_a>inline void
Push(T Vec, T_a *Array)
{
  Assert( Array->At < Array->End );
  *Array->At = Vec;
  ++Array->At;
  return;
}

struct mesh_metadata
{
  u32 VertCount;
  u32 NormalCount;
  u32 UVCount;
  u32 FaceCount;
};

