// external/bonsai_stdlib/src/vector.h:739:0

inline s32
Area( v2i Vec )
{
  Assert(Vec.x > 0);
  Assert(Vec.y > 0);
  s32 Result = Vec.E[0] * Vec.E[1] ;
  return Result;
}



