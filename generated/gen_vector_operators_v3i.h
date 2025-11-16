// callsite
// external/bonsai_stdlib/src/vector.h:229:0

// def (gen_vector_operators)
// external/bonsai_stdlib/src/poof_functions.h:270:0
link_internal void
DeepCopy(v3i *Src, v3i *Dest)
{
  *Dest = *Src;
}

inline b32
operator==( v3i P1, v3i P2 )
{
  b32 Result = (  P1.E[0] == P2.E[0] && P1.E[1] == P2.E[1] && P1.E[2] == P2.E[2] );
  return Result;
}


// NOTE(Jesse): This is for contaiers that call AreEqual()
link_internal b32
AreEqual(v3i V1, v3i V2)
{
  b32 Result = V1 == V2;
  return Result;
}

// NOTE(Jesse): Can't gen != because the condition welding it together
// is not &&, it's ||
//
/* gen_vector_comparator(Type, {!=}) */
inline b32
operator!=( v3i P1, v3i P2 )
{
  b32 Result = !(P1 == P2);
  return Result;
}

inline s32
Sum( v3i P1 )
{
  s32 Result = (  P1.E[0]  + P1.E[1]  + P1.E[2]  );
  return Result;
}



inline s32
MaxChannel( v3i P1 )
{
  s32 Result = P1.E[0];
    Result = Cast(s32, Max(Result, P1.E[0]));
  Result = Cast(s32, Max(Result, P1.E[1]));
  Result = Cast(s32, Max(Result, P1.E[2]));
;
  return Result;
}

inline s32
MinChannel( v3i P1 )
{
  s32 Result = P1.E[0];
    Result = Cast(s32, Min(Result, P1.E[0]));
  Result = Cast(s32, Min(Result, P1.E[1]));
  Result = Cast(s32, Min(Result, P1.E[2]));
;
  return Result;
}


inline b32
operator<( v3i P1, v3i P2 )
{
  b32 Result = (  P1.E[0] < P2.E[0] && P1.E[1] < P2.E[1] && P1.E[2] < P2.E[2] );
  return Result;
}


inline b32
operator<=( v3i P1, v3i P2 )
{
  b32 Result = (  P1.E[0] <= P2.E[0] && P1.E[1] <= P2.E[1] && P1.E[2] <= P2.E[2] );
  return Result;
}


inline b32
operator>( v3i P1, v3i P2 )
{
  b32 Result = (  P1.E[0] > P2.E[0] && P1.E[1] > P2.E[1] && P1.E[2] > P2.E[2] );
  return Result;
}


inline b32
operator>=( v3i P1, v3i P2 )
{
  b32 Result = (  P1.E[0] >= P2.E[0] && P1.E[1] >= P2.E[1] && P1.E[2] >= P2.E[2] );
  return Result;
}


inline v3i
operator+( v3i P1, v3i P2 )
{
  v3i Result = {{
            Cast(s32, P1.E[0] + P2.E[0]),
      Cast(s32, P1.E[1] + P2.E[1]),
      Cast(s32, P1.E[2] + P2.E[2]),

    }};
  return Result;
}

inline v3i
operator+( v3i P1, s32 Scalar )
{
  v3i Result = {{
            Cast(s32, P1.E[0] + Scalar),
      Cast(s32, P1.E[1] + Scalar),
      Cast(s32, P1.E[2] + Scalar),

    }};
  return Result;
}

inline v3i
operator+( s32 Scalar, v3i P1 )
{
  v3i Result = {{
            Cast(s32, Scalar + P1.E[0]),
      Cast(s32, Scalar + P1.E[1]),
      Cast(s32, Scalar + P1.E[2]),

    }};
  return Result;
}



inline v3i
operator-( v3i P1, v3i P2 )
{
  v3i Result = {{
            Cast(s32, P1.E[0] - P2.E[0]),
      Cast(s32, P1.E[1] - P2.E[1]),
      Cast(s32, P1.E[2] - P2.E[2]),

    }};
  return Result;
}

inline v3i
operator-( v3i P1, s32 Scalar )
{
  v3i Result = {{
            Cast(s32, P1.E[0] - Scalar),
      Cast(s32, P1.E[1] - Scalar),
      Cast(s32, P1.E[2] - Scalar),

    }};
  return Result;
}

inline v3i
operator-( s32 Scalar, v3i P1 )
{
  v3i Result = {{
            Cast(s32, Scalar - P1.E[0]),
      Cast(s32, Scalar - P1.E[1]),
      Cast(s32, Scalar - P1.E[2]),

    }};
  return Result;
}



inline v3i
operator*( v3i P1, v3i P2 )
{
  v3i Result = {{
            Cast(s32, P1.E[0] * P2.E[0]),
      Cast(s32, P1.E[1] * P2.E[1]),
      Cast(s32, P1.E[2] * P2.E[2]),

    }};
  return Result;
}

inline v3i
operator*( v3i P1, s32 Scalar )
{
  v3i Result = {{
            Cast(s32, P1.E[0] * Scalar),
      Cast(s32, P1.E[1] * Scalar),
      Cast(s32, P1.E[2] * Scalar),

    }};
  return Result;
}

inline v3i
operator*( s32 Scalar, v3i P1 )
{
  v3i Result = {{
            Cast(s32, Scalar * P1.E[0]),
      Cast(s32, Scalar * P1.E[1]),
      Cast(s32, Scalar * P1.E[2]),

    }};
  return Result;
}



inline v3i
operator/( v3i P1, v3i P2 )
{
  v3i Result = {{
            Cast(s32, P1.E[0] / P2.E[0]),
      Cast(s32, P1.E[1] / P2.E[1]),
      Cast(s32, P1.E[2] / P2.E[2]),

    }};
  return Result;
}

inline v3i
operator/( v3i P1, s32 Scalar )
{
  v3i Result = {{
            Cast(s32, P1.E[0] / Scalar),
      Cast(s32, P1.E[1] / Scalar),
      Cast(s32, P1.E[2] / Scalar),

    }};
  return Result;
}

inline v3i
operator/( s32 Scalar, v3i P1 )
{
  v3i Result = {{
            Cast(s32, Scalar / P1.E[0]),
      Cast(s32, Scalar / P1.E[1]),
      Cast(s32, Scalar / P1.E[2]),

    }};
  return Result;
}



inline v3i &
operator+=( v3i &P1, v3i P2 )
{
    P1.E[0] += P2.E[0];
  P1.E[1] += P2.E[1];
  P1.E[2] += P2.E[2];

  return P1;
}

inline v3i &
operator+=( v3i &P1, s32 Scalar )
{
    P1.E[0] += Scalar;
  P1.E[1] += Scalar;
  P1.E[2] += Scalar;

  return P1;
}


inline v3i &
operator-=( v3i &P1, v3i P2 )
{
    P1.E[0] -= P2.E[0];
  P1.E[1] -= P2.E[1];
  P1.E[2] -= P2.E[2];

  return P1;
}

inline v3i &
operator-=( v3i &P1, s32 Scalar )
{
    P1.E[0] -= Scalar;
  P1.E[1] -= Scalar;
  P1.E[2] -= Scalar;

  return P1;
}


inline v3i &
operator*=( v3i &P1, v3i P2 )
{
    P1.E[0] *= P2.E[0];
  P1.E[1] *= P2.E[1];
  P1.E[2] *= P2.E[2];

  return P1;
}

inline v3i &
operator*=( v3i &P1, s32 Scalar )
{
    P1.E[0] *= Scalar;
  P1.E[1] *= Scalar;
  P1.E[2] *= Scalar;

  return P1;
}


inline v3i &
operator/=( v3i &P1, v3i P2 )
{
    P1.E[0] /= P2.E[0];
  P1.E[1] /= P2.E[1];
  P1.E[2] /= P2.E[2];

  return P1;
}

inline v3i &
operator/=( v3i &P1, s32 Scalar )
{
    P1.E[0] /= Scalar;
  P1.E[1] /= Scalar;
  P1.E[2] /= Scalar;

  return P1;
}




