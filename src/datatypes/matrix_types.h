
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
  m4 Result = {
    V4(M[0][0], M[0][1], M[0][2], M[0][3]),
    V4(M[1][0], M[1][1], M[1][2], M[1][3]),
    V4(M[2][0], M[2][1], M[2][2], M[2][3]),
    V4(M[3][0], M[3][1], M[3][2], M[3][3]),
  };

  return Result;
}

m4
operator*(m4 A, m4 B)
{

  glm::mat4 MA = GLM4(A);
  glm::mat4 MB = GLM4(B);
  m4 GlmResult = GLM4(MA * MB);


  /* v4 A0 = A[0]; */
  /* v4 A1 = A[1]; */
  /* v4 A2 = A[2]; */
  /* v4 A3 = A[3]; */

  /* v4 B0 = B[0]; */
  /* v4 B1 = B[1]; */
  /* v4 B2 = B[2]; */
  /* v4 B3 = B[3]; */


  m4 Result = {};

  // OpenGL matrices 
  Result[0].w = A[0][0]*B[0][0] + A[0][1]*B[1][0] + A[0][2]*B[2][0] + A[0][3]*B[3][0];
  Result[1].w = A[0][0]*B[0][1] + A[0][1]*B[1][1] + A[0][2]*B[2][1] + A[0][3]*B[3][1];
  Result[2].w = A[0][0]*B[0][2] + A[0][1]*B[1][2] + A[0][2]*B[2][2] + A[0][3]*B[3][2];
  Result[3].w = A[0][0]*B[0][3] + A[0][1]*B[1][3] + A[0][2]*B[2][3] + A[0][3]*B[3][3];


  GlmResult = GLM4(MA * MB);
  return GlmResult;
}

inline m4
Translate( v3 v )
{
#if 1
  m4 Result;

  Result.E[0] = v4(1,0,0,v.x);
  Result.E[1] = v4(0,1,0,v.y);
  Result.E[2] = v4(0,0,1,v.z);
  Result.E[3] = v4(0,0,0,1);
#else
  glm::mat4 TransMatrix = glm::translate(glm::mat4(), glm::vec3(v.x, v.y, v.z));
  m4 Result = GLM4(TransMatrix);


#endif

  return Result;
}

