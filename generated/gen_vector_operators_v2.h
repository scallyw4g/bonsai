inline b32
operator==( v2 P1, v2 P2 )
{
  b32 Result = ( P1.E[0] == P2.E[0] && P1.E[1] == P2.E[1] );
  return Result;
}


// NOTE(Jesse): Can't gen != because the condition welding it together
// is not &&, it's ||
//
/* gen_vector_comparator(Type, {!=}) */
inline b32
operator!=( v2 P1, v2 P2 )
{
  b32 Result = !(P1 == P2);
  return Result;
}

inline b32
operator<( v2 P1, v2 P2 )
{
  b32 Result = ( P1.E[0] < P2.E[0] && P1.E[1] < P2.E[1] );
  return Result;
}


inline b32
operator<=( v2 P1, v2 P2 )
{
  b32 Result = ( P1.E[0] <= P2.E[0] && P1.E[1] <= P2.E[1] );
  return Result;
}


inline b32
operator>( v2 P1, v2 P2 )
{
  b32 Result = ( P1.E[0] > P2.E[0] && P1.E[1] > P2.E[1] );
  return Result;
}


inline b32
operator>=( v2 P1, v2 P2 )
{
  b32 Result = ( P1.E[0] >= P2.E[0] && P1.E[1] >= P2.E[1] );
  return Result;
}


inline v2
operator+( v2 P1, v2 P2 )
{
  v2 Result = {
    .E[0] = P1.E[0] + P2.E[0],
    .E[1] = P1.E[1] + P2.E[1],
  };
  return Result;
}

inline v2
operator+( v2 P1, r32 Scalar )
{
  v2 Result = {
    .E[0] = P1.E[0] + Scalar,
    .E[1] = P1.E[1] + Scalar,
  };
  return Result;
}

inline v2
operator+( r32 Scalar, v2 P1 )
{
  v2 Result = {
    .E[0] = Scalar + P1.E[0],
    .E[1] = Scalar + P1.E[1],
  };
  return Result;
}



inline v2
operator-( v2 P1, v2 P2 )
{
  v2 Result = {
    .E[0] = P1.E[0] - P2.E[0],
    .E[1] = P1.E[1] - P2.E[1],
  };
  return Result;
}

inline v2
operator-( v2 P1, r32 Scalar )
{
  v2 Result = {
    .E[0] = P1.E[0] - Scalar,
    .E[1] = P1.E[1] - Scalar,
  };
  return Result;
}

inline v2
operator-( r32 Scalar, v2 P1 )
{
  v2 Result = {
    .E[0] = Scalar - P1.E[0],
    .E[1] = Scalar - P1.E[1],
  };
  return Result;
}



inline v2
operator*( v2 P1, v2 P2 )
{
  v2 Result = {
    .E[0] = P1.E[0] * P2.E[0],
    .E[1] = P1.E[1] * P2.E[1],
  };
  return Result;
}

inline v2
operator*( v2 P1, r32 Scalar )
{
  v2 Result = {
    .E[0] = P1.E[0] * Scalar,
    .E[1] = P1.E[1] * Scalar,
  };
  return Result;
}

inline v2
operator*( r32 Scalar, v2 P1 )
{
  v2 Result = {
    .E[0] = Scalar * P1.E[0],
    .E[1] = Scalar * P1.E[1],
  };
  return Result;
}



inline v2
operator/( v2 P1, v2 P2 )
{
  v2 Result = {
    .E[0] = P1.E[0] / P2.E[0],
    .E[1] = P1.E[1] / P2.E[1],
  };
  return Result;
}

inline v2
operator/( v2 P1, r32 Scalar )
{
  v2 Result = {
    .E[0] = P1.E[0] / Scalar,
    .E[1] = P1.E[1] / Scalar,
  };
  return Result;
}

inline v2
operator/( r32 Scalar, v2 P1 )
{
  v2 Result = {
    .E[0] = Scalar / P1.E[0],
    .E[1] = Scalar / P1.E[1],
  };
  return Result;
}



inline v2 &
operator+=( v2 &P1, v2 P2 )
{
  P1.E[0] += P2.E[0];
  P1.E[1] += P2.E[1];
  return P1;
}

inline v2 &
operator+=( v2 &P1, r32 Scalar )
{
  P1.E[0] += Scalar;
  P1.E[1] += Scalar;
  return P1;
}


inline v2 &
operator-=( v2 &P1, v2 P2 )
{
  P1.E[0] -= P2.E[0];
  P1.E[1] -= P2.E[1];
  return P1;
}

inline v2 &
operator-=( v2 &P1, r32 Scalar )
{
  P1.E[0] -= Scalar;
  P1.E[1] -= Scalar;
  return P1;
}


inline v2 &
operator*=( v2 &P1, v2 P2 )
{
  P1.E[0] *= P2.E[0];
  P1.E[1] *= P2.E[1];
  return P1;
}

inline v2 &
operator*=( v2 &P1, r32 Scalar )
{
  P1.E[0] *= Scalar;
  P1.E[1] *= Scalar;
  return P1;
}


inline v2 &
operator/=( v2 &P1, v2 P2 )
{
  P1.E[0] /= P2.E[0];
  P1.E[1] /= P2.E[1];
  return P1;
}

inline v2 &
operator/=( v2 &P1, r32 Scalar )
{
  P1.E[0] /= Scalar;
  P1.E[1] /= Scalar;
  return P1;
}




