
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
union Quaternion
{
  struct { float x, y, z, w; };
  struct { v3 xyz, _ignored; };

  float E[4];

  Quaternion(void)
  {
    Clear(this);
  }

  Quaternion( v3 Vec, r32 W )
  {
    this->x = Vec.x;
    this->y = Vec.y;
    this->z = Vec.z;
    this->w = W;
  }

  Quaternion( r32 X, r32 Y, r32 Z, r32 W )
  {
    this->x = X;
    this->y = Y;
    this->z = Z;
    this->w = W;
  }
};
#pragma GCC diagnostic pop

Quaternion
operator*(Quaternion A, Quaternion B)
{
  Quaternion Result = {};

  Result.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
  Result.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
  Result.y = A.w*B.y + A.y*B.w + A.z*B.x - A.x*B.z;
  Result.z = A.w*B.z + A.z*B.w + A.x*B.y - A.y*B.x;

  return Result;
}


inline Quaternion
Conjugate( Quaternion q )
{
  Quaternion Result = {-q.x, -q.y, -q.z, q.w};
  return Result;
}

