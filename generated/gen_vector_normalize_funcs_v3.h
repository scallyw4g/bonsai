inline v3
Normalize( v3 Vec, r32 Length)
{
  if (Length == 0.f) return {};
  v3 Result = Vec/Length;
  return Result;
}

inline v3
Normalize( v3 Vec )
{
  v3 Result = Normalize(Vec, Length(Vec));
  return Result;
}



