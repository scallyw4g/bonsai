#ifndef PLAYGROUNDH
#define PLAYGROUNDH

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))

// Keep track of triangle count for debugging
static int triCount = 0;

struct v2 {
   int x;
   int y;
};

union v3 {
  struct { float x, y, z; };
  struct { float r, g, b; };

  struct {
    v2 xy;
    float Ignored0_;
  };

  struct {
    float Ignored1_;
    v2 yz;
  };

  float E[3];
};

union v4 {
  struct { float x, y, z, w; };
  struct { float r, g, b, a; };

  struct {
    v2 xy;
    float Ignored0_;
    float Ignored01_;
  };

  struct {
    v3 xyz;
    float Ignored02_;
  };

  float E[4];
};

struct chunk_position
{
  int x;
  int y;
  int z;
};

chunk_position operator+(chunk_position P1, chunk_position const P2)
{
  chunk_position Result;

  Result.x = P2.x + P1.x;
  Result.y = P2.y + P1.y;
  Result.z = P2.z + P1.z;

  return Result;
}

typedef chunk_position chunk_dim;


chunk_position Chunk_Position(int x, int y, int z)
{
  chunk_position Result;

  Result.x = x;
  Result.y = y;
  Result.z = z;

  return Result;
};

chunk_dim Chunk_Dim(int x, int y, int z)
{
  chunk_dim Result = Chunk_Position(x,y,z);

  return Result;
}

v2 V2(int x,int y)
{
  v2 Result = {};

  Result.x = x;
  Result.y = y;

  return Result;
}

v3 V3(chunk_position wp)
{
  v3 Result;

  Result.x = (float)wp.x;
  Result.y = (float)wp.y;
  Result.z = (float)wp.z;

  return Result;
}

v3 V3(glm::vec3 vec)
{
  v3 Result;

  Result.x = vec.x;
  Result.y = vec.y;
  Result.z = vec.z;

  return Result;
}

v3 V3(int x,int y,int z)
{
  v3 Result = {};

  Result.x = x;
  Result.y = y;
  Result.z = z;

  return Result;
}

v4 V4(int x,int y,int z,int w)
{
  v4 Result = {};

  Result.x = x;
  Result.y = y;
  Result.z = z;
  Result.w = w;

  return Result;
}

v2 operator+=(v2 P1, v2 P2)
{
  v2 Result;

  Result.x = P1.x + P2.x;
  Result.y = P1.y + P2.y;

  return Result;
}

v2 operator+(v2 P1, v2 P2)
{
  v2 Result;

  Result.x = P1.x + P2.x;
  Result.y = P1.y + P2.y;

  return Result;
}

v2 operator*(float f, v2 P)
{
  P.x *= f;
  P.y *= f;
}

inline v3
operator+(v3 A, v3 B)
{
  v3 Result;

  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;

  return Result;
}

inline v3&
operator+=(v3& A, float f)
{
  A.x += f;
  A.y += f;
  A.z += f;

  return(A);
}


inline v3&
operator+=(v3& A, v3 B)
{
  A = A + B;
  return(A);
}

inline v3
operator-(v3 A, v3 B)
{
  v3 Result;

  Result.x = A.x - B.x;
  Result.y = A.y - B.y;
  Result.z = A.z - B.z;

  return Result;
}

inline v3&
operator-=(v3& A, v3 B)
{
  A = A - B;
  return(A);
}

inline v3
operator*(v3 A, float f)
{
  v3 Result;

  Result.x = A.x * f;
  Result.y = A.y * f;
  Result.z = A.z * f;

  return Result;
}

inline v3&
operator*=(v3& A, float f)
{
  A.x = A.x * f;
  A.y = A.y * f;
  A.z = A.z * f;

  return A;
}

v4 operator*(v4 A, int B)
{
  v4 Result = V4(0,0,0,0);

  Result.x = A.x * B;
  Result.y = A.y * B;
  Result.z = A.z * B;
  Result.w = A.w * B;

  return Result;
}

v4 operator*=(v4 A, int B)
{
  A = A * B;
  return A;
}

v4 operator+(v4 A, v4 B)
{
  v4 Result = V4(0,0,0,0);

  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;
  Result.w = A.w + B.w;

  return Result;
}

glm::vec3 V3( v3 Vec )
{
  glm::vec3 Result;

  Result.x = Vec.x;
  Result.y = Vec.y;
  Result.z = Vec.z;

  return Result;
}

struct VertexBlock
{
  GLfloat *Data;
  int bytesAllocd;
  int filled;
};

v3 ToV3(chunk_position chunkP)
{
  v3 Result;

  Result.x = chunkP.x;
  Result.y = chunkP.y;
  Result.z = chunkP.z;

  return Result;
}

struct Chunk
{
  v4 *Voxels;
  chunk_dim Dim;

  // Position in absolute world coordinates.  A chunk is one world coordinate
  chunk_position WorldP;

  // Position within the chunk
  v3 Offset;

  VertexBlock VertexData;
  VertexBlock ColorData;
};

struct Entity
{
  Chunk Model;
};

#endif
