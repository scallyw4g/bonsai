// callsite
// external/bonsai_stdlib/src/vector.h:220:0

// def (gen_vector_operators)
// external/bonsai_stdlib/src/poof_functions.h:270:0
link_internal void
DeepCopy(v2 *Src, v2 *Dest)
{
  *Dest = *Src;
}

inline b32
operator==( v2 P1, v2 P2 )
{
  b32 Result = (  P1.E[0] == P2.E[0] && P1.E[1] == P2.E[1] );
  return Result;
}


// NOTE(Jesse): This is for contaiers that call AreEqual()
link_internal b32
AreEqual(v2 V1, v2 V2)
{
  b32 Result = V1 == V2;
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

inline r32
Sum( v2 P1 )
{
  r32 Result = (  P1.E[0]  + P1.E[1]  );
  return Result;
}



inline r32
MaxChannel( v2 P1 )
{
  r32 Result = P1.E[0];
    Result = Cast(r32, Max(Result, P1.E[0]));
  Result = Cast(r32, Max(Result, P1.E[1]));
;
  return Result;
}

inline r32
MinChannel( v2 P1 )
{
  r32 Result = P1.E[0];
    Result = Cast(r32, Min(Result, P1.E[0]));
  Result = Cast(r32, Min(Result, P1.E[1]));
;
  return Result;
}


inline b32
operator<( v2 P1, v2 P2 )
{
  b32 Result = (  P1.E[0] < P2.E[0] && P1.E[1] < P2.E[1] );
  return Result;
}


inline b32
operator<=( v2 P1, v2 P2 )
{
  b32 Result = (  P1.E[0] <= P2.E[0] && P1.E[1] <= P2.E[1] );
  return Result;
}


inline b32
operator>( v2 P1, v2 P2 )
{
  b32 Result = (  P1.E[0] > P2.E[0] && P1.E[1] > P2.E[1] );
  return Result;
}


inline b32
operator>=( v2 P1, v2 P2 )
{
  b32 Result = (  P1.E[0] >= P2.E[0] && P1.E[1] >= P2.E[1] );
  return Result;
}


inline v2
operator+( v2 P1, v2 P2 )
{
  v2 Result = {{
            Cast(r32, P1.E[0] + P2.E[0]),
      Cast(r32, P1.E[1] + P2.E[1]),

    }};
  return Result;
}

inline v2
operator+( v2 P1, r32 Scalar )
{
  v2 Result = {{
            Cast(r32, P1.E[0] + Scalar),
      Cast(r32, P1.E[1] + Scalar),

    }};
  return Result;
}

inline v2
operator+( r32 Scalar, v2 P1 )
{
  v2 Result = {{
            Cast(r32, Scalar + P1.E[0]),
      Cast(r32, Scalar + P1.E[1]),

    }};
  return Result;
}



inline v2
operator-( v2 P1, v2 P2 )
{
  v2 Result = {{
            Cast(r32, P1.E[0] - P2.E[0]),
      Cast(r32, P1.E[1] - P2.E[1]),

    }};
  return Result;
}

inline v2
operator-( v2 P1, r32 Scalar )
{
  v2 Result = {{
            Cast(r32, P1.E[0] - Scalar),
      Cast(r32, P1.E[1] - Scalar),

    }};
  return Result;
}

inline v2
operator-( r32 Scalar, v2 P1 )
{
  v2 Result = {{
            Cast(r32, Scalar - P1.E[0]),
      Cast(r32, Scalar - P1.E[1]),

    }};
  return Result;
}



inline v2
operator*( v2 P1, v2 P2 )
{
  v2 Result = {{
            Cast(r32, P1.E[0] * P2.E[0]),
      Cast(r32, P1.E[1] * P2.E[1]),

    }};
  return Result;
}

inline v2
operator*( v2 P1, r32 Scalar )
{
  v2 Result = {{
            Cast(r32, P1.E[0] * Scalar),
      Cast(r32, P1.E[1] * Scalar),

    }};
  return Result;
}

inline v2
operator*( r32 Scalar, v2 P1 )
{
  v2 Result = {{
            Cast(r32, Scalar * P1.E[0]),
      Cast(r32, Scalar * P1.E[1]),

    }};
  return Result;
}



inline v2
operator/( v2 P1, v2 P2 )
{
  v2 Result = {{
            Cast(r32, P1.E[0] / P2.E[0]),
      Cast(r32, P1.E[1] / P2.E[1]),

    }};
  return Result;
}

inline v2
operator/( v2 P1, r32 Scalar )
{
  v2 Result = {{
            Cast(r32, P1.E[0] / Scalar),
      Cast(r32, P1.E[1] / Scalar),

    }};
  return Result;
}

inline v2
operator/( r32 Scalar, v2 P1 )
{
  v2 Result = {{
            Cast(r32, Scalar / P1.E[0]),
      Cast(r32, Scalar / P1.E[1]),

    }};
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




