inline b32
operator==( v3 P1, v3 P2 )
{
  b32 Result = ( P1.E[0] == P2.E[0] && P1.E[1] == P2.E[1] && P1.E[2] == P2.E[2] );
  return Result;
}


// NOTE(Jesse): Can't gen != because the condition welding it together
// is not &&, it's ||
//
/* gen_vector_comparator(Type, {!=}) */
inline b32
operator!=( v3 P1, v3 P2 )
{
  b32 Result = !(P1 == P2);
  return Result;
}

inline b32
operator<( v3 P1, v3 P2 )
{
  b32 Result = ( P1.E[0] < P2.E[0] && P1.E[1] < P2.E[1] && P1.E[2] < P2.E[2] );
  return Result;
}


inline b32
operator<=( v3 P1, v3 P2 )
{
  b32 Result = ( P1.E[0] <= P2.E[0] && P1.E[1] <= P2.E[1] && P1.E[2] <= P2.E[2] );
  return Result;
}


inline b32
operator>( v3 P1, v3 P2 )
{
  b32 Result = ( P1.E[0] > P2.E[0] && P1.E[1] > P2.E[1] && P1.E[2] > P2.E[2] );
  return Result;
}


inline b32
operator>=( v3 P1, v3 P2 )
{
  b32 Result = ( P1.E[0] >= P2.E[0] && P1.E[1] >= P2.E[1] && P1.E[2] >= P2.E[2] );
  return Result;
}


inline v3
operator+( v3 P1, v3 P2 )
{
  v3 Result = {
    .E[0] = P1.E[0] + P2.E[0],
    .E[1] = P1.E[1] + P2.E[1],
    .E[2] = P1.E[2] + P2.E[2],
  };
  return Result;
}

inline v3
operator+( v3 P1, r32 Scalar )
{
  v3 Result = {
    .E[0] = P1.E[0] + Scalar,
    .E[1] = P1.E[1] + Scalar,
    .E[2] = P1.E[2] + Scalar,
  };
  return Result;
}

inline v3
operator+( r32 Scalar, v3 P1 )
{
  v3 Result = {
    .E[0] = Scalar + P1.E[0],
    .E[1] = Scalar + P1.E[1],
    .E[2] = Scalar + P1.E[2],
  };
  return Result;
}



inline v3
operator-( v3 P1, v3 P2 )
{
  v3 Result = {
    .E[0] = P1.E[0] - P2.E[0],
    .E[1] = P1.E[1] - P2.E[1],
    .E[2] = P1.E[2] - P2.E[2],
  };
  return Result;
}

inline v3
operator-( v3 P1, r32 Scalar )
{
  v3 Result = {
    .E[0] = P1.E[0] - Scalar,
    .E[1] = P1.E[1] - Scalar,
    .E[2] = P1.E[2] - Scalar,
  };
  return Result;
}

inline v3
operator-( r32 Scalar, v3 P1 )
{
  v3 Result = {
    .E[0] = Scalar - P1.E[0],
    .E[1] = Scalar - P1.E[1],
    .E[2] = Scalar - P1.E[2],
  };
  return Result;
}



inline v3
operator*( v3 P1, v3 P2 )
{
  v3 Result = {
    .E[0] = P1.E[0] * P2.E[0],
    .E[1] = P1.E[1] * P2.E[1],
    .E[2] = P1.E[2] * P2.E[2],
  };
  return Result;
}

inline v3
operator*( v3 P1, r32 Scalar )
{
  v3 Result = {
    .E[0] = P1.E[0] * Scalar,
    .E[1] = P1.E[1] * Scalar,
    .E[2] = P1.E[2] * Scalar,
  };
  return Result;
}

inline v3
operator*( r32 Scalar, v3 P1 )
{
  v3 Result = {
    .E[0] = Scalar * P1.E[0],
    .E[1] = Scalar * P1.E[1],
    .E[2] = Scalar * P1.E[2],
  };
  return Result;
}



inline v3
operator/( v3 P1, v3 P2 )
{
  v3 Result = {
    .E[0] = P1.E[0] / P2.E[0],
    .E[1] = P1.E[1] / P2.E[1],
    .E[2] = P1.E[2] / P2.E[2],
  };
  return Result;
}

inline v3
operator/( v3 P1, r32 Scalar )
{
  v3 Result = {
    .E[0] = P1.E[0] / Scalar,
    .E[1] = P1.E[1] / Scalar,
    .E[2] = P1.E[2] / Scalar,
  };
  return Result;
}

inline v3
operator/( r32 Scalar, v3 P1 )
{
  v3 Result = {
    .E[0] = Scalar / P1.E[0],
    .E[1] = Scalar / P1.E[1],
    .E[2] = Scalar / P1.E[2],
  };
  return Result;
}



inline v3 &
operator+=( v3 &P1, v3 P2 )
{
  P1.E[0] += P2.E[0];
  P1.E[1] += P2.E[1];
  P1.E[2] += P2.E[2];
  return P1;
}

inline v3 &
operator+=( v3 &P1, r32 Scalar )
{
  P1.E[0] += Scalar;
  P1.E[1] += Scalar;
  P1.E[2] += Scalar;
  return P1;
}


inline v3 &
operator-=( v3 &P1, v3 P2 )
{
  P1.E[0] -= P2.E[0];
  P1.E[1] -= P2.E[1];
  P1.E[2] -= P2.E[2];
  return P1;
}

inline v3 &
operator-=( v3 &P1, r32 Scalar )
{
  P1.E[0] -= Scalar;
  P1.E[1] -= Scalar;
  P1.E[2] -= Scalar;
  return P1;
}


inline v3 &
operator*=( v3 &P1, v3 P2 )
{
  P1.E[0] *= P2.E[0];
  P1.E[1] *= P2.E[1];
  P1.E[2] *= P2.E[2];
  return P1;
}

inline v3 &
operator*=( v3 &P1, r32 Scalar )
{
  P1.E[0] *= Scalar;
  P1.E[1] *= Scalar;
  P1.E[2] *= Scalar;
  return P1;
}


inline v3 &
operator/=( v3 &P1, v3 P2 )
{
  P1.E[0] /= P2.E[0];
  P1.E[1] /= P2.E[1];
  P1.E[2] /= P2.E[2];
  return P1;
}

inline v3 &
operator/=( v3 &P1, r32 Scalar )
{
  P1.E[0] /= Scalar;
  P1.E[1] /= Scalar;
  P1.E[2] /= Scalar;
  return P1;
}




