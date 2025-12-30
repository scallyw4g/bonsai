// callsite
// external/bonsai_stdlib/src/vector.h:814:0

// def (gen_vector_normalize)
// external/bonsai_stdlib/src/poof_functions.h:585:0
inline v3
Normalize( v3 Vec, r32 Length)
{
  if (Length < 0.00001f) return {};
  v3 Result = Vec/Length;
  return Result;
}

inline v3
Normalize( v3 Vec )
{
  v3 Result = Normalize(Vec, Length(Vec));
  return Result;
}



