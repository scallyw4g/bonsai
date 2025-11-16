// callsite
// external/bonsai_stdlib/src/vector.h:235:0

// def (gen_vector_operators)
// external/bonsai_stdlib/src/poof_functions.h:270:0
link_internal void
DeepCopy(v4 *Src, v4 *Dest)
{
  *Dest = *Src;
}

inline b32
operator==( v4 P1, v4 P2 )
{
  b32 Result = (  P1.E[0] == P2.E[0] && P1.E[1] == P2.E[1] && P1.E[2] == P2.E[2] && P1.E[3] == P2.E[3] );
  return Result;
}


// NOTE(Jesse): This is for contaiers that call AreEqual()
link_internal b32
AreEqual(v4 V1, v4 V2)
{
  b32 Result = V1 == V2;
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

inline r32
Sum( v4 P1 )
{
  r32 Result = (  P1.E[0]  + P1.E[1]  + P1.E[2]  + P1.E[3]  );
  return Result;
}



inline r32
MaxChannel( v4 P1 )
{
  r32 Result = P1.E[0];
    Result = Cast(r32, Max(Result, P1.E[0]));
  Result = Cast(r32, Max(Result, P1.E[1]));
  Result = Cast(r32, Max(Result, P1.E[2]));
  Result = Cast(r32, Max(Result, P1.E[3]));
;
  return Result;
}

inline r32
MinChannel( v4 P1 )
{
  r32 Result = P1.E[0];
    Result = Cast(r32, Min(Result, P1.E[0]));
  Result = Cast(r32, Min(Result, P1.E[1]));
  Result = Cast(r32, Min(Result, P1.E[2]));
  Result = Cast(r32, Min(Result, P1.E[3]));
;
  return Result;
}


inline b32
operator<( v4 P1, v4 P2 )
{
  b32 Result = (  P1.E[0] < P2.E[0] && P1.E[1] < P2.E[1] && P1.E[2] < P2.E[2] && P1.E[3] < P2.E[3] );
  return Result;
}


inline b32
operator<=( v4 P1, v4 P2 )
{
  b32 Result = (  P1.E[0] <= P2.E[0] && P1.E[1] <= P2.E[1] && P1.E[2] <= P2.E[2] && P1.E[3] <= P2.E[3] );
  return Result;
}


inline b32
operator>( v4 P1, v4 P2 )
{
  b32 Result = (  P1.E[0] > P2.E[0] && P1.E[1] > P2.E[1] && P1.E[2] > P2.E[2] && P1.E[3] > P2.E[3] );
  return Result;
}


inline b32
operator>=( v4 P1, v4 P2 )
{
  b32 Result = (  P1.E[0] >= P2.E[0] && P1.E[1] >= P2.E[1] && P1.E[2] >= P2.E[2] && P1.E[3] >= P2.E[3] );
  return Result;
}


inline v4
operator+( v4 P1, v4 P2 )
{
  v4 Result = {{
            Cast(r32, P1.E[0] + P2.E[0]),
      Cast(r32, P1.E[1] + P2.E[1]),
      Cast(r32, P1.E[2] + P2.E[2]),
      Cast(r32, P1.E[3] + P2.E[3]),

    }};
  return Result;
}

inline v4
operator+( v4 P1, r32 Scalar )
{
  v4 Result = {{
            Cast(r32, P1.E[0] + Scalar),
      Cast(r32, P1.E[1] + Scalar),
      Cast(r32, P1.E[2] + Scalar),
      Cast(r32, P1.E[3] + Scalar),

    }};
  return Result;
}

inline v4
operator+( r32 Scalar, v4 P1 )
{
  v4 Result = {{
            Cast(r32, Scalar + P1.E[0]),
      Cast(r32, Scalar + P1.E[1]),
      Cast(r32, Scalar + P1.E[2]),
      Cast(r32, Scalar + P1.E[3]),

    }};
  return Result;
}



inline v4
operator-( v4 P1, v4 P2 )
{
  v4 Result = {{
            Cast(r32, P1.E[0] - P2.E[0]),
      Cast(r32, P1.E[1] - P2.E[1]),
      Cast(r32, P1.E[2] - P2.E[2]),
      Cast(r32, P1.E[3] - P2.E[3]),

    }};
  return Result;
}

inline v4
operator-( v4 P1, r32 Scalar )
{
  v4 Result = {{
            Cast(r32, P1.E[0] - Scalar),
      Cast(r32, P1.E[1] - Scalar),
      Cast(r32, P1.E[2] - Scalar),
      Cast(r32, P1.E[3] - Scalar),

    }};
  return Result;
}

inline v4
operator-( r32 Scalar, v4 P1 )
{
  v4 Result = {{
            Cast(r32, Scalar - P1.E[0]),
      Cast(r32, Scalar - P1.E[1]),
      Cast(r32, Scalar - P1.E[2]),
      Cast(r32, Scalar - P1.E[3]),

    }};
  return Result;
}



inline v4
operator*( v4 P1, v4 P2 )
{
  v4 Result = {{
            Cast(r32, P1.E[0] * P2.E[0]),
      Cast(r32, P1.E[1] * P2.E[1]),
      Cast(r32, P1.E[2] * P2.E[2]),
      Cast(r32, P1.E[3] * P2.E[3]),

    }};
  return Result;
}

inline v4
operator*( v4 P1, r32 Scalar )
{
  v4 Result = {{
            Cast(r32, P1.E[0] * Scalar),
      Cast(r32, P1.E[1] * Scalar),
      Cast(r32, P1.E[2] * Scalar),
      Cast(r32, P1.E[3] * Scalar),

    }};
  return Result;
}

inline v4
operator*( r32 Scalar, v4 P1 )
{
  v4 Result = {{
            Cast(r32, Scalar * P1.E[0]),
      Cast(r32, Scalar * P1.E[1]),
      Cast(r32, Scalar * P1.E[2]),
      Cast(r32, Scalar * P1.E[3]),

    }};
  return Result;
}



inline v4
operator/( v4 P1, v4 P2 )
{
  v4 Result = {{
            Cast(r32, P1.E[0] / P2.E[0]),
      Cast(r32, P1.E[1] / P2.E[1]),
      Cast(r32, P1.E[2] / P2.E[2]),
      Cast(r32, P1.E[3] / P2.E[3]),

    }};
  return Result;
}

inline v4
operator/( v4 P1, r32 Scalar )
{
  v4 Result = {{
            Cast(r32, P1.E[0] / Scalar),
      Cast(r32, P1.E[1] / Scalar),
      Cast(r32, P1.E[2] / Scalar),
      Cast(r32, P1.E[3] / Scalar),

    }};
  return Result;
}

inline v4
operator/( r32 Scalar, v4 P1 )
{
  v4 Result = {{
            Cast(r32, Scalar / P1.E[0]),
      Cast(r32, Scalar / P1.E[1]),
      Cast(r32, Scalar / P1.E[2]),
      Cast(r32, Scalar / P1.E[3]),

    }};
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




