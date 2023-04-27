inline v2
Normalize( v2 Vec, r32 Length)
{
  if (Length == 0.f) return {};
  v2 Result = Vec/Length;
  return Result;
}

inline v2
Normalize( v2 Vec )
{
  v2 Result = Normalize(Vec, Length(Vec));
  return Result;
}



