
struct m4
{
  v4 E[4];

  v4&
  operator[](int index)
  {
    v4& Result = this->E[index];
    return Result;
  }
};

inline glm::mat4
GLM4(m4 M)
{
  glm::mat4 Result(
    glm::vec4(M[0][0], M[0][1], M[0][2], M[0][3]),
    glm::vec4(M[1][0], M[1][1], M[1][2], M[1][3]),
    glm::vec4(M[2][0], M[2][1], M[2][2], M[2][3]),
    glm::vec4(M[3][0], M[3][1], M[3][2], M[3][3])
  );

  return Result;
}

inline m4
GLM4(glm::mat4 M)
{
  m4 Result;

  Result[0] = V4(M[0][0], M[0][1], M[0][2], M[0][3]);
  Result[1] = V4(M[1][0], M[1][1], M[1][2], M[1][3]);
  Result[2] = V4(M[2][0], M[2][1], M[2][2], M[2][3]);
  Result[3] = V4(M[3][0], M[3][1], M[3][2], M[3][3]);

  return Result;
}

m4
operator*(m4 A, m4 B)
{
  m4 Result = {};

  for(u32 Row = 0; Row < 4; ++Row)
  {
    for(u32 Column = 0; Column < 4; ++Column)
    {
      for(u32 ResultIndex = 0; ResultIndex < 4; ++ResultIndex)
      {
        Result.E[Row][Column] += A.E[ResultIndex][Column] * B.E[Row][ResultIndex];
      }
    }
  }

  return Result;
}

inline m4
Translate( v3 v )
{
  m4 Result;

  Result.E[0] = v4(1,0,0,v.x);
  Result.E[1] = v4(0,1,0,v.y);
  Result.E[2] = v4(0,0,1,v.z);
  Result.E[3] = v4(0,0,0,1);

  return Result;
}

