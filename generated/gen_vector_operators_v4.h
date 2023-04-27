inline b32
operator==( v4 P1, v4 P2 )
{
  b32 Result = ( P1.E[0] == P2.E[0] && P1.E[1] == P2.E[1] && P1.E[2] == P2.E[2] && P1.E[3] == P2.E[3] );
  return Result;
}


// NOTE(Jesse): Can't gen != because the condition welding it together
// is not &&, it's ||
//
/* gen_vector_comparator(Type, {!=}) */
inline b32
operator!=( v4 P1, v4 P2 )
{
  b32 Result = !(P1 == P2);
  return Result;
}

inline b32
operator<( v4 P1, v4 P2 )
{
  b32 Result = ( P1.E[0] < P2.E[0] && P1.E[1] < P2.E[1] && P1.E[2] < P2.E[2] && P1.E[3] < P2.E[3] );
  return Result;
}


inline b32
operator<=( v4 P1, v4 P2 )
{
  b32 Result = ( P1.E[0] <= P2.E[0] && P1.E[1] <= P2.E[1] && P1.E[2] <= P2.E[2] && P1.E[3] <= P2.E[3] );
  return Result;
}


inline b32
operator>( v4 P1, v4 P2 )
{
  b32 Result = ( P1.E[0] > P2.E[0] && P1.E[1] > P2.E[1] && P1.E[2] > P2.E[2] && P1.E[3] > P2.E[3] );
  return Result;
}


inline b32
operator>=( v4 P1, v4 P2 )
{
  b32 Result = ( P1.E[0] >= P2.E[0] && P1.E[1] >= P2.E[1] && P1.E[2] >= P2.E[2] && P1.E[3] >= P2.E[3] );
  return Result;
}


inline v4
operator+( v4 P1, v4 P2 )
{
  v4 Result = {
    .E[0] = P1.E[0] + P2.E[0],
    .E[1] = P1.E[1] + P2.E[1],
    .E[2] = P1.E[2] + P2.E[2],
    .E[3] = P1.E[3] + P2.E[3],
  };
  return Result;
}

inline v4
operator+( v4 P1, r32 Scalar )
{
  v4 Result = {
    .E[0] = P1.E[0] + Scalar,
    .E[1] = P1.E[1] + Scalar,
    .E[2] = P1.E[2] + Scalar,
    .E[3] = P1.E[3] + Scalar,
  };
  return Result;
}

inline v4
operator+( r32 Scalar, v4 P1 )
{
  v4 Result = {
    .E[0] = Scalar + P1.E[0],
    .E[1] = Scalar + P1.E[1],
    .E[2] = Scalar + P1.E[2],
    .E[3] = Scalar + P1.E[3],
  };
  return Result;
}



inline v4
operator-( v4 P1, v4 P2 )
{
  v4 Result = {
    .E[0] = P1.E[0] - P2.E[0],
    .E[1] = P1.E[1] - P2.E[1],
    .E[2] = P1.E[2] - P2.E[2],
    .E[3] = P1.E[3] - P2.E[3],
  };
  return Result;
}

inline v4
operator-( v4 P1, r32 Scalar )
{
  v4 Result = {
    .E[0] = P1.E[0] - Scalar,
    .E[1] = P1.E[1] - Scalar,
    .E[2] = P1.E[2] - Scalar,
    .E[3] = P1.E[3] - Scalar,
  };
  return Result;
}

inline v4
operator-( r32 Scalar, v4 P1 )
{
  v4 Result = {
    .E[0] = Scalar - P1.E[0],
    .E[1] = Scalar - P1.E[1],
    .E[2] = Scalar - P1.E[2],
    .E[3] = Scalar - P1.E[3],
  };
  return Result;
}



inline v4
operator*( v4 P1, v4 P2 )
{
  v4 Result = {
    .E[0] = P1.E[0] * P2.E[0],
    .E[1] = P1.E[1] * P2.E[1],
    .E[2] = P1.E[2] * P2.E[2],
    .E[3] = P1.E[3] * P2.E[3],
  };
  return Result;
}

inline v4
operator*( v4 P1, r32 Scalar )
{
  v4 Result = {
    .E[0] = P1.E[0] * Scalar,
    .E[1] = P1.E[1] * Scalar,
    .E[2] = P1.E[2] * Scalar,
    .E[3] = P1.E[3] * Scalar,
  };
  return Result;
}

inline v4
operator*( r32 Scalar, v4 P1 )
{
  v4 Result = {
    .E[0] = Scalar * P1.E[0],
    .E[1] = Scalar * P1.E[1],
    .E[2] = Scalar * P1.E[2],
    .E[3] = Scalar * P1.E[3],
  };
  return Result;
}



inline v4
operator/( v4 P1, v4 P2 )
{
  v4 Result = {
    .E[0] = P1.E[0] / P2.E[0],
    .E[1] = P1.E[1] / P2.E[1],
    .E[2] = P1.E[2] / P2.E[2],
    .E[3] = P1.E[3] / P2.E[3],
  };
  return Result;
}

inline v4
operator/( v4 P1, r32 Scalar )
{
  v4 Result = {
    .E[0] = P1.E[0] / Scalar,
    .E[1] = P1.E[1] / Scalar,
    .E[2] = P1.E[2] / Scalar,
    .E[3] = P1.E[3] / Scalar,
  };
  return Result;
}

inline v4
operator/( r32 Scalar, v4 P1 )
{
  v4 Result = {
    .E[0] = Scalar / P1.E[0],
    .E[1] = Scalar / P1.E[1],
    .E[2] = Scalar / P1.E[2],
    .E[3] = Scalar / P1.E[3],
  };
  return Result;
}



inline v4 &
operator+=( v4 &P1, v4 P2 )
{
  P1.E[0] += P2.E[0];
  P1.E[1] += P2.E[1];
  P1.E[2] += P2.E[2];
  P1.E[3] += P2.E[3];
  return P1;
}

inline v4 &
operator+=( v4 &P1, r32 Scalar )
{
  P1.E[0] += Scalar;
  P1.E[1] += Scalar;
  P1.E[2] += Scalar;
  P1.E[3] += Scalar;
  return P1;
}


inline v4 &
operator-=( v4 &P1, v4 P2 )
{
  P1.E[0] -= P2.E[0];
  P1.E[1] -= P2.E[1];
  P1.E[2] -= P2.E[2];
  P1.E[3] -= P2.E[3];
  return P1;
}

inline v4 &
operator-=( v4 &P1, r32 Scalar )
{
  P1.E[0] -= Scalar;
  P1.E[1] -= Scalar;
  P1.E[2] -= Scalar;
  P1.E[3] -= Scalar;
  return P1;
}


inline v4 &
operator*=( v4 &P1, v4 P2 )
{
  P1.E[0] *= P2.E[0];
  P1.E[1] *= P2.E[1];
  P1.E[2] *= P2.E[2];
  P1.E[3] *= P2.E[3];
  return P1;
}

inline v4 &
operator*=( v4 &P1, r32 Scalar )
{
  P1.E[0] *= Scalar;
  P1.E[1] *= Scalar;
  P1.E[2] *= Scalar;
  P1.E[3] *= Scalar;
  return P1;
}


inline v4 &
operator/=( v4 &P1, v4 P2 )
{
  P1.E[0] /= P2.E[0];
  P1.E[1] /= P2.E[1];
  P1.E[2] /= P2.E[2];
  P1.E[3] /= P2.E[3];
  return P1;
}

inline v4 &
operator/=( v4 &P1, r32 Scalar )
{
  P1.E[0] /= Scalar;
  P1.E[1] /= Scalar;
  P1.E[2] /= Scalar;
  P1.E[3] /= Scalar;
  return P1;
}




