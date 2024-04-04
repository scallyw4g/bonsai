// external/bonsai_stdlib/src/vector.h:717:0

inline v3i
operator&( v3i P1, v3i P2 )
{
  v3i Result = {
    .E[0] = P1.E[0] & P2.E[0],
    .E[1] = P1.E[1] & P2.E[1],
    .E[2] = P1.E[2] & P2.E[2],
  };
  return Result;
}



