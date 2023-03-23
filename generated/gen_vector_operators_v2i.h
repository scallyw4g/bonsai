inline b32
operator==( v2i P1, v2i P2 )
{
  b32 Result = ( P1.E[0] == P2.E[0] && P1.E[1] == P2.E[1] );
  return Result;
}


// NOTE(Jesse): Can't gen != because the condition welding it together
// is not &&, it's ||
//
/* gen_vector_comparator(Type, {!=}) */
inline b32
operator!=( v2i P1, v2i P2 )
{
  b32 Result = !(P1 == P2);
  return Result;
}

inline b32
operator<( v2i P1, v2i P2 )
{
  b32 Result = ( P1.E[0] < P2.E[0] && P1.E[1] < P2.E[1] );
  return Result;
}


inline b32
operator<=( v2i P1, v2i P2 )
{
  b32 Result = ( P1.E[0] <= P2.E[0] && P1.E[1] <= P2.E[1] );
  return Result;
}


inline b32
operator>( v2i P1, v2i P2 )
{
  b32 Result = ( P1.E[0] > P2.E[0] && P1.E[1] > P2.E[1] );
  return Result;
}


inline b32
operator>=( v2i P1, v2i P2 )
{
  b32 Result = ( P1.E[0] >= P2.E[0] && P1.E[1] >= P2.E[1] );
  return Result;
}


inline v2i
operator+( v2i P1, v2i P2 )
{
  v2i Result = {
    .E[0] = P1.E[0] + P2.E[0],
    .E[1] = P1.E[1] + P2.E[1],
  };
  return Result;
}

inline v2i
operator+( v2i P1, s32 Scalar )
{
  v2i Result = {
    .E[0] = P1.E[0] + Scalar,
    .E[1] = P1.E[1] + Scalar,
  };
  return Result;
}

inline v2i
operator+( s32 Scalar, v2i P1 )
{
  v2i Result = {
    .E[0] = Scalar + P1.E[0],
    .E[1] = Scalar + P1.E[1],
  };
  return Result;
}



inline v2i
operator-( v2i P1, v2i P2 )
{
  v2i Result = {
    .E[0] = P1.E[0] - P2.E[0],
    .E[1] = P1.E[1] - P2.E[1],
  };
  return Result;
}

inline v2i
operator-( v2i P1, s32 Scalar )
{
  v2i Result = {
    .E[0] = P1.E[0] - Scalar,
    .E[1] = P1.E[1] - Scalar,
  };
  return Result;
}

inline v2i
operator-( s32 Scalar, v2i P1 )
{
  v2i Result = {
    .E[0] = Scalar - P1.E[0],
    .E[1] = Scalar - P1.E[1],
  };
  return Result;
}



inline v2i
operator*( v2i P1, v2i P2 )
{
  v2i Result = {
    .E[0] = P1.E[0] * P2.E[0],
    .E[1] = P1.E[1] * P2.E[1],
  };
  return Result;
}

inline v2i
operator*( v2i P1, s32 Scalar )
{
  v2i Result = {
    .E[0] = P1.E[0] * Scalar,
    .E[1] = P1.E[1] * Scalar,
  };
  return Result;
}

inline v2i
operator*( s32 Scalar, v2i P1 )
{
  v2i Result = {
    .E[0] = Scalar * P1.E[0],
    .E[1] = Scalar * P1.E[1],
  };
  return Result;
}



inline v2i
operator/( v2i P1, v2i P2 )
{
  v2i Result = {
    .E[0] = P1.E[0] / P2.E[0],
    .E[1] = P1.E[1] / P2.E[1],
  };
  return Result;
}

inline v2i
operator/( v2i P1, s32 Scalar )
{
  v2i Result = {
    .E[0] = P1.E[0] / Scalar,
    .E[1] = P1.E[1] / Scalar,
  };
  return Result;
}

inline v2i
operator/( s32 Scalar, v2i P1 )
{
  v2i Result = {
    .E[0] = Scalar / P1.E[0],
    .E[1] = Scalar / P1.E[1],
  };
  return Result;
}



inline v2i &
operator+=( v2i &P1, v2i P2 )
{
  P1.E[0] += P2.E[0];
  P1.E[1] += P2.E[1];
  return P1;
}

inline v2i &
operator+=( v2i &P1, s32 Scalar )
{
  P1.E[0] += Scalar;
  P1.E[1] += Scalar;
  return P1;
}


inline v2i &
operator-=( v2i &P1, v2i P2 )
{
  P1.E[0] -= P2.E[0];
  P1.E[1] -= P2.E[1];
  return P1;
}

inline v2i &
operator-=( v2i &P1, s32 Scalar )
{
  P1.E[0] -= Scalar;
  P1.E[1] -= Scalar;
  return P1;
}


inline v2i &
operator*=( v2i &P1, v2i P2 )
{
  P1.E[0] *= P2.E[0];
  P1.E[1] *= P2.E[1];
  return P1;
}

inline v2i &
operator*=( v2i &P1, s32 Scalar )
{
  P1.E[0] *= Scalar;
  P1.E[1] *= Scalar;
  return P1;
}


inline v2i &
operator/=( v2i &P1, v2i P2 )
{
  P1.E[0] /= P2.E[0];
  P1.E[1] /= P2.E[1];
  return P1;
}

inline v2i &
operator/=( v2i &P1, s32 Scalar )
{
  P1.E[0] /= Scalar;
  P1.E[1] /= Scalar;
  return P1;
}




