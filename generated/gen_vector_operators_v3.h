// callsite
// external/bonsai_stdlib/src/vector.h:226:0

// def (gen_vector_operators)
// external/bonsai_stdlib/src/poof_functions.h:270:0
link_internal void
DeepCopy(v3 *Src, v3 *Dest)
{
  *Dest = *Src;
}

inline b32
operator==( v3 P1, v3 P2 )
{
  b32 Result = (  P1.E[0] == P2.E[0] && P1.E[1] == P2.E[1] && P1.E[2] == P2.E[2] );
  return Result;
}


// NOTE(Jesse): This is for contaiers that call AreEqual()
link_internal b32
AreEqual(v3 V1, v3 V2)
{
  b32 Result = V1 == V2;
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

inline r32
Sum( v3 P1 )
{
  r32 Result = (  P1.E[0]  + P1.E[1]  + P1.E[2]  );
  return Result;
}



inline r32
MaxChannel( v3 P1 )
{
  r32 Result = P1.E[0];
    Result = Cast(r32, Max(Result, P1.E[0]));
  Result = Cast(r32, Max(Result, P1.E[1]));
  Result = Cast(r32, Max(Result, P1.E[2]));
;
  return Result;
}

inline r32
MinChannel( v3 P1 )
{
  r32 Result = P1.E[0];
    Result = Cast(r32, Min(Result, P1.E[0]));
  Result = Cast(r32, Min(Result, P1.E[1]));
  Result = Cast(r32, Min(Result, P1.E[2]));
;
  return Result;
}


inline b32
operator<( v3 P1, v3 P2 )
{
  b32 Result = (  P1.E[0] < P2.E[0] && P1.E[1] < P2.E[1] && P1.E[2] < P2.E[2] );
  return Result;
}


inline b32
operator<=( v3 P1, v3 P2 )
{
  b32 Result = (  P1.E[0] <= P2.E[0] && P1.E[1] <= P2.E[1] && P1.E[2] <= P2.E[2] );
  return Result;
}


inline b32
operator>( v3 P1, v3 P2 )
{
  b32 Result = (  P1.E[0] > P2.E[0] && P1.E[1] > P2.E[1] && P1.E[2] > P2.E[2] );
  return Result;
}


inline b32
operator>=( v3 P1, v3 P2 )
{
  b32 Result = (  P1.E[0] >= P2.E[0] && P1.E[1] >= P2.E[1] && P1.E[2] >= P2.E[2] );
  return Result;
}


inline v3
operator+( v3 P1, v3 P2 )
{
  v3 Result = {{
            Cast(r32, P1.E[0] + P2.E[0]),
      Cast(r32, P1.E[1] + P2.E[1]),
      Cast(r32, P1.E[2] + P2.E[2]),

    }};
  return Result;
}

inline v3
operator+( v3 P1, r32 Scalar )
{
  v3 Result = {{
            Cast(r32, P1.E[0] + Scalar),
      Cast(r32, P1.E[1] + Scalar),
      Cast(r32, P1.E[2] + Scalar),

    }};
  return Result;
}

inline v3
operator+( r32 Scalar, v3 P1 )
{
  v3 Result = {{
            Cast(r32, Scalar + P1.E[0]),
      Cast(r32, Scalar + P1.E[1]),
      Cast(r32, Scalar + P1.E[2]),

    }};
  return Result;
}



inline v3
operator-( v3 P1, v3 P2 )
{
  v3 Result = {{
            Cast(r32, P1.E[0] - P2.E[0]),
      Cast(r32, P1.E[1] - P2.E[1]),
      Cast(r32, P1.E[2] - P2.E[2]),

    }};
  return Result;
}

inline v3
operator-( v3 P1, r32 Scalar )
{
  v3 Result = {{
            Cast(r32, P1.E[0] - Scalar),
      Cast(r32, P1.E[1] - Scalar),
      Cast(r32, P1.E[2] - Scalar),

    }};
  return Result;
}

inline v3
operator-( r32 Scalar, v3 P1 )
{
  v3 Result = {{
            Cast(r32, Scalar - P1.E[0]),
      Cast(r32, Scalar - P1.E[1]),
      Cast(r32, Scalar - P1.E[2]),

    }};
  return Result;
}



inline v3
operator*( v3 P1, v3 P2 )
{
  v3 Result = {{
            Cast(r32, P1.E[0] * P2.E[0]),
      Cast(r32, P1.E[1] * P2.E[1]),
      Cast(r32, P1.E[2] * P2.E[2]),

    }};
  return Result;
}

inline v3
operator*( v3 P1, r32 Scalar )
{
  v3 Result = {{
            Cast(r32, P1.E[0] * Scalar),
      Cast(r32, P1.E[1] * Scalar),
      Cast(r32, P1.E[2] * Scalar),

    }};
  return Result;
}

inline v3
operator*( r32 Scalar, v3 P1 )
{
  v3 Result = {{
            Cast(r32, Scalar * P1.E[0]),
      Cast(r32, Scalar * P1.E[1]),
      Cast(r32, Scalar * P1.E[2]),

    }};
  return Result;
}



inline v3
operator/( v3 P1, v3 P2 )
{
  v3 Result = {{
            Cast(r32, P1.E[0] / P2.E[0]),
      Cast(r32, P1.E[1] / P2.E[1]),
      Cast(r32, P1.E[2] / P2.E[2]),

    }};
  return Result;
}

inline v3
operator/( v3 P1, r32 Scalar )
{
  v3 Result = {{
            Cast(r32, P1.E[0] / Scalar),
      Cast(r32, P1.E[1] / Scalar),
      Cast(r32, P1.E[2] / Scalar),

    }};
  return Result;
}

inline v3
operator/( r32 Scalar, v3 P1 )
{
  v3 Result = {{
            Cast(r32, Scalar / P1.E[0]),
      Cast(r32, Scalar / P1.E[1]),
      Cast(r32, Scalar / P1.E[2]),

    }};
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




