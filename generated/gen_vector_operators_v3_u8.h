// callsite
// external/bonsai_stdlib/src/vector.h:232:0

// def (gen_vector_operators)
// external/bonsai_stdlib/src/poof_functions.h:270:0
link_internal void
DeepCopy(v3_u8 *Src, v3_u8 *Dest)
{
  *Dest = *Src;
}

inline b32
operator==( v3_u8 P1, v3_u8 P2 )
{
  b32 Result = (  P1.E[0] == P2.E[0] && P1.E[1] == P2.E[1] && P1.E[2] == P2.E[2] );
  return Result;
}


// NOTE(Jesse): This is for contaiers that call AreEqual()
link_internal b32
AreEqual(v3_u8 V1, v3_u8 V2)
{
  b32 Result = V1 == V2;
  return Result;
}

// NOTE(Jesse): Can't gen != because the condition welding it together
// is not &&, it's ||
//
/* gen_vector_comparator(Type, {!=}) */
inline b32
operator!=( v3_u8 P1, v3_u8 P2 )
{
  b32 Result = !(P1 == P2);
  return Result;
}

inline u8
Sum( v3_u8 P1 )
{
  u8 Result = (  P1.E[0]  + P1.E[1]  + P1.E[2]  );
  return Result;
}



inline u8
MaxChannel( v3_u8 P1 )
{
  u8 Result = P1.E[0];
    Result = Cast(u8, Max(Result, P1.E[0]));
  Result = Cast(u8, Max(Result, P1.E[1]));
  Result = Cast(u8, Max(Result, P1.E[2]));
;
  return Result;
}

inline u8
MinChannel( v3_u8 P1 )
{
  u8 Result = P1.E[0];
    Result = Cast(u8, Min(Result, P1.E[0]));
  Result = Cast(u8, Min(Result, P1.E[1]));
  Result = Cast(u8, Min(Result, P1.E[2]));
;
  return Result;
}


inline b32
operator<( v3_u8 P1, v3_u8 P2 )
{
  b32 Result = (  P1.E[0] < P2.E[0] && P1.E[1] < P2.E[1] && P1.E[2] < P2.E[2] );
  return Result;
}


inline b32
operator<=( v3_u8 P1, v3_u8 P2 )
{
  b32 Result = (  P1.E[0] <= P2.E[0] && P1.E[1] <= P2.E[1] && P1.E[2] <= P2.E[2] );
  return Result;
}


inline b32
operator>( v3_u8 P1, v3_u8 P2 )
{
  b32 Result = (  P1.E[0] > P2.E[0] && P1.E[1] > P2.E[1] && P1.E[2] > P2.E[2] );
  return Result;
}


inline b32
operator>=( v3_u8 P1, v3_u8 P2 )
{
  b32 Result = (  P1.E[0] >= P2.E[0] && P1.E[1] >= P2.E[1] && P1.E[2] >= P2.E[2] );
  return Result;
}


inline v3_u8
operator+( v3_u8 P1, v3_u8 P2 )
{
  v3_u8 Result = {{
            Cast(u8, P1.E[0] + P2.E[0]),
      Cast(u8, P1.E[1] + P2.E[1]),
      Cast(u8, P1.E[2] + P2.E[2]),

    }};
  return Result;
}

inline v3_u8
operator+( v3_u8 P1, u8 Scalar )
{
  v3_u8 Result = {{
            Cast(u8, P1.E[0] + Scalar),
      Cast(u8, P1.E[1] + Scalar),
      Cast(u8, P1.E[2] + Scalar),

    }};
  return Result;
}

inline v3_u8
operator+( u8 Scalar, v3_u8 P1 )
{
  v3_u8 Result = {{
            Cast(u8, Scalar + P1.E[0]),
      Cast(u8, Scalar + P1.E[1]),
      Cast(u8, Scalar + P1.E[2]),

    }};
  return Result;
}



inline v3_u8
operator-( v3_u8 P1, v3_u8 P2 )
{
  v3_u8 Result = {{
            Cast(u8, P1.E[0] - P2.E[0]),
      Cast(u8, P1.E[1] - P2.E[1]),
      Cast(u8, P1.E[2] - P2.E[2]),

    }};
  return Result;
}

inline v3_u8
operator-( v3_u8 P1, u8 Scalar )
{
  v3_u8 Result = {{
            Cast(u8, P1.E[0] - Scalar),
      Cast(u8, P1.E[1] - Scalar),
      Cast(u8, P1.E[2] - Scalar),

    }};
  return Result;
}

inline v3_u8
operator-( u8 Scalar, v3_u8 P1 )
{
  v3_u8 Result = {{
            Cast(u8, Scalar - P1.E[0]),
      Cast(u8, Scalar - P1.E[1]),
      Cast(u8, Scalar - P1.E[2]),

    }};
  return Result;
}



inline v3_u8
operator*( v3_u8 P1, v3_u8 P2 )
{
  v3_u8 Result = {{
            Cast(u8, P1.E[0] * P2.E[0]),
      Cast(u8, P1.E[1] * P2.E[1]),
      Cast(u8, P1.E[2] * P2.E[2]),

    }};
  return Result;
}

inline v3_u8
operator*( v3_u8 P1, u8 Scalar )
{
  v3_u8 Result = {{
            Cast(u8, P1.E[0] * Scalar),
      Cast(u8, P1.E[1] * Scalar),
      Cast(u8, P1.E[2] * Scalar),

    }};
  return Result;
}

inline v3_u8
operator*( u8 Scalar, v3_u8 P1 )
{
  v3_u8 Result = {{
            Cast(u8, Scalar * P1.E[0]),
      Cast(u8, Scalar * P1.E[1]),
      Cast(u8, Scalar * P1.E[2]),

    }};
  return Result;
}



inline v3_u8
operator/( v3_u8 P1, v3_u8 P2 )
{
  v3_u8 Result = {{
            Cast(u8, P1.E[0] / P2.E[0]),
      Cast(u8, P1.E[1] / P2.E[1]),
      Cast(u8, P1.E[2] / P2.E[2]),

    }};
  return Result;
}

inline v3_u8
operator/( v3_u8 P1, u8 Scalar )
{
  v3_u8 Result = {{
            Cast(u8, P1.E[0] / Scalar),
      Cast(u8, P1.E[1] / Scalar),
      Cast(u8, P1.E[2] / Scalar),

    }};
  return Result;
}

inline v3_u8
operator/( u8 Scalar, v3_u8 P1 )
{
  v3_u8 Result = {{
            Cast(u8, Scalar / P1.E[0]),
      Cast(u8, Scalar / P1.E[1]),
      Cast(u8, Scalar / P1.E[2]),

    }};
  return Result;
}



inline v3_u8 &
operator+=( v3_u8 &P1, v3_u8 P2 )
{
    P1.E[0] += P2.E[0];
  P1.E[1] += P2.E[1];
  P1.E[2] += P2.E[2];

  return P1;
}

inline v3_u8 &
operator+=( v3_u8 &P1, u8 Scalar )
{
    P1.E[0] += Scalar;
  P1.E[1] += Scalar;
  P1.E[2] += Scalar;

  return P1;
}


inline v3_u8 &
operator-=( v3_u8 &P1, v3_u8 P2 )
{
    P1.E[0] -= P2.E[0];
  P1.E[1] -= P2.E[1];
  P1.E[2] -= P2.E[2];

  return P1;
}

inline v3_u8 &
operator-=( v3_u8 &P1, u8 Scalar )
{
    P1.E[0] -= Scalar;
  P1.E[1] -= Scalar;
  P1.E[2] -= Scalar;

  return P1;
}


inline v3_u8 &
operator*=( v3_u8 &P1, v3_u8 P2 )
{
    P1.E[0] *= P2.E[0];
  P1.E[1] *= P2.E[1];
  P1.E[2] *= P2.E[2];

  return P1;
}

inline v3_u8 &
operator*=( v3_u8 &P1, u8 Scalar )
{
    P1.E[0] *= Scalar;
  P1.E[1] *= Scalar;
  P1.E[2] *= Scalar;

  return P1;
}


inline v3_u8 &
operator/=( v3_u8 &P1, v3_u8 P2 )
{
    P1.E[0] /= P2.E[0];
  P1.E[1] /= P2.E[1];
  P1.E[2] /= P2.E[2];

  return P1;
}

inline v3_u8 &
operator/=( v3_u8 &P1, u8 Scalar )
{
    P1.E[0] /= Scalar;
  P1.E[1] /= Scalar;
  P1.E[2] /= Scalar;

  return P1;
}




