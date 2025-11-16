// callsite
// external/bonsai_stdlib/src/vector.h:808:0

// def (gen_common_vector)
// external/bonsai_stdlib/src/poof_functions.h:464:0
inline s32
LengthSq( v3i Vec )
{
  s32 Result =  Vec.E[0]*Vec.E[0] + Vec.E[1]*Vec.E[1] + Vec.E[2]*Vec.E[2] ;
  return Result;
}

inline r32
Length( v3i Vec )
{
  r32 Result = (r32)SquareRoot(LengthSq(Vec));
  return Result;
}

inline v3i
Max( v3i A, v3i B )
{
  v3i Result;
    Result.E[0] = Max( A.E[0], B.E[0] );
  Result.E[1] = Max( A.E[1], B.E[1] );
  Result.E[2] = Max( A.E[2], B.E[2] );

  return Result;
}

inline v3i
Min( v3i A, v3i B )
{
  v3i Result;
    Result.E[0] = Min( A.E[0], B.E[0] );
  Result.E[1] = Min( A.E[1], B.E[1] );
  Result.E[2] = Min( A.E[2], B.E[2] );

  return Result;
}

inline v3i
Abs( v3i Vec )
{
  v3i Result;
    Result.E[0] = (s32)Abs( Vec.E[0] );
  Result.E[1] = (s32)Abs( Vec.E[1] );
  Result.E[2] = (s32)Abs( Vec.E[2] );

  return Result;
}


inline v3i
GetSign( v3i Vec )
{
  v3i Result;
    Result.E[0] = GetSign( Vec.E[0] );
  Result.E[1] = GetSign( Vec.E[1] );
  Result.E[2] = GetSign( Vec.E[2] );

  return Result;
}


inline v3i
Bilateral( v3i Vec )
{
  v3i Result;
    Result.E[0] = Bilateral( Vec.E[0] );
  Result.E[1] = Bilateral( Vec.E[1] );
  Result.E[2] = Bilateral( Vec.E[2] );

  return Result;
}

inline v3i
ClampNegative( v3i V )
{
  v3i Result = V;
    if ( V.E[0] > s32(0) ) Result.E[0] = s32(0);
  if ( V.E[1] > s32(0) ) Result.E[1] = s32(0);
  if ( V.E[2] > s32(0) ) Result.E[2] = s32(0);

  return Result;
}

inline v3i
ClampPositive( v3i V )
{
  v3i Result = V;
    if ( V.E[0] < s32(0) ) Result.E[0] = s32(0);
  if ( V.E[1] < s32(0) ) Result.E[1] = s32(0);
  if ( V.E[2] < s32(0) ) Result.E[2] = s32(0);

  return Result;
}

inline v3i
Clamp01( v3i V )
{
  v3i Result = V;
    if ( V.E[0] < s32(0) ) Result.E[0] = s32(0);
  if ( V.E[0] > s32(1) ) Result.E[0] = s32(1);
  if ( V.E[1] < s32(0) ) Result.E[1] = s32(0);
  if ( V.E[1] > s32(1) ) Result.E[1] = s32(1);
  if ( V.E[2] < s32(0) ) Result.E[2] = s32(0);
  if ( V.E[2] > s32(1) ) Result.E[2] = s32(1);

  return Result;
}





