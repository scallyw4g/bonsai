inline r32
LengthSq( v2 Vec )
{
  r32 Result = Vec.E[0]*Vec.E[0] + Vec.E[1]*Vec.E[1] ;
  return Result;
}

inline r32
Length( v2 Vec )
{
  r32 Result = (r32)sqrt(LengthSq(Vec));
  return Result;
}

inline v2
Max( v2 A, v2 B )
{
  v2 Result;
  Result.E[0] = Max( A.E[0], B.E[0] );
  Result.E[1] = Max( A.E[1], B.E[1] );
  return Result;
}

inline v2
Min( v2 A, v2 B )
{
  v2 Result;
  Result.E[0] = Min( A.E[0], B.E[0] );
  Result.E[1] = Min( A.E[1], B.E[1] );
  return Result;
}

inline v2
Abs( v2 Vec )
{
  v2 Result;
  Result.E[0] = (r32)Abs( Vec.E[0] );
  Result.E[1] = (r32)Abs( Vec.E[1] );
  return Result;
}


inline v2
GetSign( v2 Vec )
{
  v2 Result;
  Result.E[0] = GetSign( Vec.E[0] );
  Result.E[1] = GetSign( Vec.E[1] );
  return Result;
}


inline v2
Bilateral( v2 Vec )
{
  v2 Result;
  Result.E[0] = Bilateral( Vec.E[0] );
  Result.E[1] = Bilateral( Vec.E[1] );
  return Result;
}

inline v2
ClampNegative( v2 V )
{
  v2 Result = V;
  if ( V.E[0] > r32(0) ) Result.E[0] = r32(0);
  if ( V.E[1] > r32(0) ) Result.E[1] = r32(0);
  return Result;
}

inline v2
ClampPositive( v2 V )
{
  v2 Result = V;
  if ( V.E[0] < r32(0) ) Result.E[0] = r32(0);
  if ( V.E[1] < r32(0) ) Result.E[1] = r32(0);
  return Result;
}





