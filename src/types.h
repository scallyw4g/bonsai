
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

struct voxel_position
{
  int x;
  int y;
  int z;
};

inline bool
operator==(v3 P1, v3 P2)
{
  bool Result;

  Result = (
    P1.x == P2.x &&
    P1.y == P2.y &&
    P1.z == P2.z );

  return Result;
}

inline bool
operator!=(v3 P1, v3 P2)
{
  bool Result = !(P1 == P2);
  return Result;
}

inline bool
operator==(voxel_position P1, voxel_position P2)
{
  bool Result;

  Result = (
    P1.x == P2.x &&
    P1.y == P2.y &&
    P1.z == P2.z );

  return Result;
}

inline bool
operator!=(voxel_position P1, voxel_position P2)
{
  bool Result = !(P1 == P2);
  return Result;
}

inline voxel_position
operator-(voxel_position P1, int i)
{
  voxel_position Result;

  Result.x = P1.x - i;
  Result.y = P1.y - i;
  Result.z = P1.z - i;

  return Result;
}

inline voxel_position
operator*(voxel_position P1, int i)
{
  voxel_position Result;

  Result.x = P1.x * i;
  Result.y = P1.y * i;
  Result.z = P1.z * i;

  return Result;
}

inline voxel_position
operator*(int i, voxel_position P1)
{
  voxel_position Result = P1*i;
  return Result;
}

inline voxel_position
operator*(voxel_position P1, voxel_position const P2)
{
  voxel_position Result;

  Result.x = P2.x * P1.x;
  Result.y = P2.y * P1.y;
  Result.z = P2.z * P1.z;

  return Result;
}

inline v3
operator*(v3 P1, voxel_position const P2)
{
  v3 Result;

  Result.x = (float)P2.x * P1.x;
  Result.y = (float)P2.y * P1.y;
  Result.z = (float)P2.z * P1.z;

  return Result;
}

inline v3
operator+(v3 P2, glm::vec3 P1)
{
  v3 Result;

  Result.x = P1.x + P2.x;
  Result.y = P1.y + P2.y;
  Result.z = P1.z + P2.z;

  return Result;
}

inline v3
operator+(glm::vec3 P1, v3 P2)
{
  v3 Result;

  Result.x = P1.x + P2.x;
  Result.y = P1.y + P2.y;
  Result.z = P1.z + P2.z;

  return Result;
}

inline glm::vec3
operator+(glm::vec3 Vec, voxel_position Pos)
{
  glm::vec3 Result;

  Result.x = Vec.x + Pos.x;
  Result.y = Vec.y + Pos.y;
  Result.z = Vec.z + Pos.z;

  return Result;
}

inline v3
operator-(v3 Vec, voxel_position Pos)
{
  v3 Result;

  Result.x = Vec.x - Pos.x;
  Result.y = Vec.y - Pos.y;
  Result.z = Vec.z - Pos.z;

  return Result;
}

inline voxel_position
operator+(voxel_position P1, int i)
{
  voxel_position Result;

  Result.x = P1.x + i;
  Result.y = P1.y + i;
  Result.z = P1.z + i;

  return Result;
}
inline voxel_position
operator+(voxel_position P1, voxel_position const P2)
{
  voxel_position Result;

  Result.x = P2.x + P1.x;
  Result.y = P2.y + P1.y;
  Result.z = P2.z + P1.z;

  return Result;
}

inline voxel_position
operator-(voxel_position P1, voxel_position const P2)
{
  voxel_position Result;

  Result.x = P1.x - P2.x;
  Result.y = P1.y - P2.y;
  Result.z = P1.z - P2.z;

  return Result;
}

inline v3
operator*(voxel_position P1, float f)
{
  v3 Result;

  Result.x = P1.x * f;
  Result.y = P1.y * f;
  Result.z = P1.z * f;

  return Result;
}

typedef voxel_position chunk_dimension;
typedef voxel_position world_position;

struct canonical_position
{
  v3 Offset;
  world_position WorldP;
};

inline v3
V3(voxel_position wp)
{
  v3 Result;

  Result.x = (float)wp.x;
  Result.y = (float)wp.y;
  Result.z = (float)wp.z;

  return Result;
}

inline v3
GLV3(glm::vec3 vec)
{
  v3 Result;

  Result.x = vec.x;
  Result.y = vec.y;
  Result.z = vec.z;

  return Result;
}

inline v3
V3(float x, float y, float z)
{
  v3 Result = {};

  Result.x = x;
  Result.y = y;
  Result.z = z;

  return Result;
}


inline canonical_position
Canonical_Position( glm::vec3 Offset, world_position WorldP )
{
  canonical_position Result;

  Result.Offset = GLV3(Offset);
  Result.WorldP = WorldP;

  return Result;
}

inline canonical_position
Canonical_Position( v3 Offset, world_position WorldP )
{
  canonical_position Result;

  Result.Offset = Offset;
  Result.WorldP = WorldP;

  return Result;
}

inline voxel_position
Voxel_Position(v3 Offset)
{
  voxel_position Result;

  Result.x = (int)Offset.x;
  Result.y = (int)Offset.y;
  Result.z = (int)Offset.z;

  return Result;
}

inline voxel_position
Voxel_Position(int x, int y, int z)
{
  voxel_position Result;

  Result.x = x;
  Result.y = y;
  Result.z = z;

  return Result;
};

inline world_position
World_Position(int x, int y, int z)
{
  chunk_dimension Result = Voxel_Position(x,y,z);
  return Result;
}

inline chunk_dimension
Chunk_Dimension(int x, int y, int z)
{
  chunk_dimension Result = Voxel_Position(x,y,z);
  return Result;
}

inline int
Volume(chunk_dimension Dim)
{
  return (Dim.x*Dim.y*Dim.z);
}

v2 V2(int x,int y)
{
  v2 Result = {};

  Result.x = x;
  Result.y = y;

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

v2
operator*(float f, v2 P)
{
  v2 Result;

  Result.x = P.x *= f;
  Result.y = P.y *= f;

  return Result;
}

inline canonical_position
operator-(canonical_position A, v3 B)
{
  canonical_position Result = A;

  Result.Offset.x = A.Offset.x - B.x;
  Result.Offset.y = A.Offset.y - B.y;
  Result.Offset.z = A.Offset.z - B.z;

  return Result;
}

inline canonical_position
operator+(canonical_position A, v3 B)
{
  canonical_position Result = A;

  Result.Offset.x = A.Offset.x + B.x;
  Result.Offset.y = A.Offset.y + B.y;
  Result.Offset.z = A.Offset.z + B.z;

  return Result;
}

inline v3
operator%(v3 A, int i)
{
  v3 Result;

  Result.x = (int)A.x % i;
  Result.y = (int)A.y % i;
  Result.z = (int)A.z % i;

  return Result;
}

inline v3
operator+(v3 A, voxel_position B)
{
  v3 Result;

  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;

  return Result;
}

inline v3
operator/(v3 A, int B)
{
  v3 Result;

  Result.x = A.x / (float)B;
  Result.y = A.y / (float)B;
  Result.z = A.z / (float)B;

  return Result;
}

inline v3
operator/(voxel_position A, int i)
{
  v3 Result;

  Result.x = A.x / (float)i;
  Result.y = A.y / (float)i;
  Result.z = A.z / (float)i;

  return Result;
}

inline v3
operator/(v3 A, voxel_position B)
{
  v3 Result;

  Result.x = A.x / B.x;
  Result.y = A.y / B.y;
  Result.z = A.z / B.z;

  return Result;
}

inline voxel_position&
operator-=(voxel_position& A, voxel_position B)
{
  A.x -= B.x;
  A.y -= B.y;
  A.z -= B.z;

  return(A);
}

inline voxel_position&
operator+=(voxel_position& A, voxel_position B)
{
  A.x += B.x;
  A.y += B.y;
  A.z += B.z;

  return(A);
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
operator-(v3 A, float f)
{
  v3 Result;

  Result.x = A.x - f;
  Result.y = A.y - f;
  Result.z = A.z - f;

  return Result;
}

inline v3
operator+(v3 A, float f)
{
  v3 Result;

  Result.x = A.x + f;
  Result.y = A.y + f;
  Result.z = A.z + f;

  return Result;
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
operator*(v3 A, v3 B)
{
  v3 Result;

  Result.x = A.x * B.x;
  Result.y = A.y * B.y;
  Result.z = A.z * B.z;

  return Result;
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


inline glm::vec3
GLV3( v3 Vec )
{
  glm::vec3 Result = vec3( Vec.x, Vec.y, Vec.z);
  return Result;
}

