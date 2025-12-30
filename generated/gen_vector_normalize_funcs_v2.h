// callsite
// external/bonsai_stdlib/src/vector.h:811:0

// def (gen_vector_normalize)
// external/bonsai_stdlib/src/poof_functions.h:585:0
inline v2
Normalize( v2 Vec, r32 Length)
{
  if (Length < 0.00001f) return {};
  v2 Result = Vec/Length;
  return Result;
}

inline v2
Normalize( v2 Vec )
{
  v2 Result = Normalize(Vec, Length(Vec));
  return Result;
}



