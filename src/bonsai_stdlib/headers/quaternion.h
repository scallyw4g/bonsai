union Quaternion
{
  struct { r32 x; r32 y; r32 z; r32 w; };
  struct { v3 xyz; r32 _ignored; };

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

inline r32
GetTheta(v3 P1, v3 P2)
{
  r32 DotP1P2 = Dot(P1,P2);

  r32 LP1 = Length(P1);
  r32 LP2 = Length(P2);

  Assert(LP1 != 0.0f);
  Assert(LP2 != 0.0f);

  r32 cosTheta = ClampBilateral(DotP1P2 / (LP1*LP2));
  r32 Theta = (r32)ArcCos( cosTheta );

  return Theta;
}

inline Quaternion
RotatePoint(v3 P1, v3 P2)
{
  P1 = Normalize(P1);
  P2 = Normalize(P2);
  v3 Axis = Normalize(Cross(P1, P2));

  r32 Theta = GetTheta(P1, P2);

  Quaternion Result = Quaternion((Axis*(r32)Sin(Theta/2.0f)), (r32)Cos(Theta/2.0f));

  if (Length(Result.xyz) == 0)  // The resulting rotation was inconsequential
    Result = Quaternion();

  return Result;
}

inline Quaternion
Conjugate( Quaternion q )
{
  Quaternion Result = {-q.x, -q.y, -q.z, q.w};
  return Result;
}


inline v3
Rotate(v3 P, Quaternion Rotation)
{
  v3 Result = ((Rotation * Quaternion(P,0)) * Conjugate(Rotation)).xyz;
  return Result;
}

inline line
Rotate(line Line, Quaternion Rotation)
{
  line Result;

  Result.MinP = Rotate(Line.MinP, Rotation);
  Result.MaxP = Rotate(Line.MaxP, Rotation);

  return Result;
}

