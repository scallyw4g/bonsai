
typedef v4 Quaternion;

Quaternion
operator*(Quaternion A, Quaternion B)
{
  Quaternion Result(0,0,0,0);

  Result.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
  Result.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
  Result.y = A.w*B.y + A.y*B.w + A.z*B.x - A.x*B.z;
  Result.z = A.w*B.z + A.z*B.w + A.x*B.y - A.y*B.x;

  return Result;
}

inline Quaternion
Conjugate( Quaternion q )
{
  Quaternion Result(-q.x, -q.y, -q.z, q.w);
  return Result;
}

