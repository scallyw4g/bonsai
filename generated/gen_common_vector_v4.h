// callsite
// external/bonsai_stdlib/src/vector.h:808:0

// def (gen_common_vector)
// external/bonsai_stdlib/src/poof_functions.h:464:0
inline r32
LengthSq( v4 Vec )
{
  r32 Result =  Vec.E[0]*Vec.E[0] + Vec.E[1]*Vec.E[1] + Vec.E[2]*Vec.E[2] + Vec.E[3]*Vec.E[3] ;
  return Result;
}

inline r32
Length( v4 Vec )
{
  r32 Result = (r32)SquareRoot(LengthSq(Vec));
  return Result;
}

inline v4
Max( v4 A, v4 B )
{
  v4 Result;
    Result.E[0] = Max( A.E[0], B.E[0] );
  Result.E[1] = Max( A.E[1], B.E[1] );
  Result.E[2] = Max( A.E[2], B.E[2] );
  Result.E[3] = Max( A.E[3], B.E[3] );

  return Result;
}

inline v4
Min( v4 A, v4 B )
{
  v4 Result;
    Result.E[0] = Min( A.E[0], B.E[0] );
  Result.E[1] = Min( A.E[1], B.E[1] );
  Result.E[2] = Min( A.E[2], B.E[2] );
  Result.E[3] = Min( A.E[3], B.E[3] );

  return Result;
}

inline v4
Abs( v4 Vec )
{
  v4 Result;
    Result.E[0] = (r32)Abs( Vec.E[0] );
  Result.E[1] = (r32)Abs( Vec.E[1] );
  Result.E[2] = (r32)Abs( Vec.E[2] );
  Result.E[3] = (r32)Abs( Vec.E[3] );

  return Result;
}


inline v4
GetSign( v4 Vec )
{
  v4 Result;
    Result.E[0] = GetSign( Vec.E[0] );
  Result.E[1] = GetSign( Vec.E[1] );
  Result.E[2] = GetSign( Vec.E[2] );
  Result.E[3] = GetSign( Vec.E[3] );

  return Result;
}


inline v4
Bilateral( v4 Vec )
{
  v4 Result;
    Result.E[0] = Bilateral( Vec.E[0] );
  Result.E[1] = Bilateral( Vec.E[1] );
  Result.E[2] = Bilateral( Vec.E[2] );
  Result.E[3] = Bilateral( Vec.E[3] );

  return Result;
}

inline v4
ClampNegative( v4 V )
{
  v4 Result = V;
    if ( V.E[0] > r32(0) ) Result.E[0] = r32(0);
  if ( V.E[1] > r32(0) ) Result.E[1] = r32(0);
  if ( V.E[2] > r32(0) ) Result.E[2] = r32(0);
  if ( V.E[3] > r32(0) ) Result.E[3] = r32(0);

  return Result;
}

inline v4
ClampPositive( v4 V )
{
  v4 Result = V;
    if ( V.E[0] < r32(0) ) Result.E[0] = r32(0);
  if ( V.E[1] < r32(0) ) Result.E[1] = r32(0);
  if ( V.E[2] < r32(0) ) Result.E[2] = r32(0);
  if ( V.E[3] < r32(0) ) Result.E[3] = r32(0);

  return Result;
}

inline v4
Clamp01( v4 V )
{
  v4 Result = V;
    if ( V.E[0] < r32(0) ) Result.E[0] = r32(0);
  if ( V.E[0] > r32(1) ) Result.E[0] = r32(1);
  if ( V.E[1] < r32(0) ) Result.E[1] = r32(0);
  if ( V.E[1] > r32(1) ) Result.E[1] = r32(1);
  if ( V.E[2] < r32(0) ) Result.E[2] = r32(0);
  if ( V.E[2] > r32(1) ) Result.E[2] = r32(1);
  if ( V.E[3] < r32(0) ) Result.E[3] = r32(0);
  if ( V.E[3] > r32(1) ) Result.E[3] = r32(1);

  return Result;
}





