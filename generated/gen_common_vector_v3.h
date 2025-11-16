// callsite
// external/bonsai_stdlib/src/vector.h:805:0

// def (gen_common_vector)
// external/bonsai_stdlib/src/poof_functions.h:464:0
inline r32
LengthSq( v3 Vec )
{
  r32 Result =  Vec.E[0]*Vec.E[0] + Vec.E[1]*Vec.E[1] + Vec.E[2]*Vec.E[2] ;
  return Result;
}

inline r32
Length( v3 Vec )
{
  r32 Result = (r32)SquareRoot(LengthSq(Vec));
  return Result;
}

inline v3
Max( v3 A, v3 B )
{
  v3 Result;
    Result.E[0] = Max( A.E[0], B.E[0] );
  Result.E[1] = Max( A.E[1], B.E[1] );
  Result.E[2] = Max( A.E[2], B.E[2] );

  return Result;
}

inline v3
Min( v3 A, v3 B )
{
  v3 Result;
    Result.E[0] = Min( A.E[0], B.E[0] );
  Result.E[1] = Min( A.E[1], B.E[1] );
  Result.E[2] = Min( A.E[2], B.E[2] );

  return Result;
}

inline v3
Abs( v3 Vec )
{
  v3 Result;
    Result.E[0] = (r32)Abs( Vec.E[0] );
  Result.E[1] = (r32)Abs( Vec.E[1] );
  Result.E[2] = (r32)Abs( Vec.E[2] );

  return Result;
}


inline v3
GetSign( v3 Vec )
{
  v3 Result;
    Result.E[0] = GetSign( Vec.E[0] );
  Result.E[1] = GetSign( Vec.E[1] );
  Result.E[2] = GetSign( Vec.E[2] );

  return Result;
}


inline v3
Bilateral( v3 Vec )
{
  v3 Result;
    Result.E[0] = Bilateral( Vec.E[0] );
  Result.E[1] = Bilateral( Vec.E[1] );
  Result.E[2] = Bilateral( Vec.E[2] );

  return Result;
}

inline v3
ClampNegative( v3 V )
{
  v3 Result = V;
    if ( V.E[0] > r32(0) ) Result.E[0] = r32(0);
  if ( V.E[1] > r32(0) ) Result.E[1] = r32(0);
  if ( V.E[2] > r32(0) ) Result.E[2] = r32(0);

  return Result;
}

inline v3
ClampPositive( v3 V )
{
  v3 Result = V;
    if ( V.E[0] < r32(0) ) Result.E[0] = r32(0);
  if ( V.E[1] < r32(0) ) Result.E[1] = r32(0);
  if ( V.E[2] < r32(0) ) Result.E[2] = r32(0);

  return Result;
}

inline v3
Clamp01( v3 V )
{
  v3 Result = V;
    if ( V.E[0] < r32(0) ) Result.E[0] = r32(0);
  if ( V.E[0] > r32(1) ) Result.E[0] = r32(1);
  if ( V.E[1] < r32(0) ) Result.E[1] = r32(0);
  if ( V.E[1] > r32(1) ) Result.E[1] = r32(1);
  if ( V.E[2] < r32(0) ) Result.E[2] = r32(0);
  if ( V.E[2] > r32(1) ) Result.E[2] = r32(1);

  return Result;
}





