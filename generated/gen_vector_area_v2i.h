// callsite
// external/bonsai_stdlib/src/vector.h:799:0

// def (gen_vector_area)
// external/bonsai_stdlib/src/poof_functions.h:441:0
inline s32
Area( v2i Vec )
{
  Assert(Vec.x > 0);
  Assert(Vec.y > 0);
  s32 Result =  Vec.E[0] * Vec.E[1] ;
  return Result;
}



