// callsite
// external/bonsai_stdlib/src/vector.h:796:0

// def (gen_vector_area)
// external/bonsai_stdlib/src/poof_functions.h:441:0
inline r32
Area( v2 Vec )
{
  Assert(Vec.x > 0);
  Assert(Vec.y > 0);
  r32 Result =  Vec.E[0] * Vec.E[1] ;
  return Result;
}



