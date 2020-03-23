struct m4
{
  v4 E[4];

  v4& operator[](int index)
  {
    v4& Result = this->E[index];
    return Result;
  }

  bool
  operator==(m4 &M2)
  {
    b32 Result = this->E[0] == M2.E[0];
    Result &= this->E[1] == M2.E[1];
    Result &= this->E[2] == M2.E[2];
    Result &= this->E[3] == M2.E[3];

    return Result;
  }

  m4(v4 Row0, v4 Row1, v4 Row2, v4 Row3)
  {
    this->E[0] = Row0;
    this->E[1] = Row1;
    this->E[2] = Row2;
    this->E[3] = Row3;
  }

  m4()
  {
    this->E[0] = {};
    this->E[1] = {};
    this->E[2] = {};
    this->E[3] = {};
  }
};

struct m_nxn
{
  r32* Elements;

  u32 Rows;
  u32 Columns;

  u32 At;
  u32 End;

  r32*
  operator[](u32 Row)
  {
    u32 Index = Row*this->Columns;
    Assert(Index < this->Columns*this->Rows);
    return this->Elements+Index;
  }

};

function  v4
TransformColumnMajor(m4 A, v4 P)
{
    v4 R;
    R.x = P.x*A.E[0][0] + P.y*A.E[1][0] + P.z*A.E[2][0] + P.w*A.E[3][0];
    R.y = P.x*A.E[0][1] + P.y*A.E[1][1] + P.z*A.E[2][1] + P.w*A.E[3][1];
    R.z = P.x*A.E[0][2] + P.y*A.E[1][2] + P.z*A.E[2][2] + P.w*A.E[3][2];
    R.w = P.x*A.E[0][3] + P.y*A.E[1][3] + P.z*A.E[2][3] + P.w*A.E[3][3];
    return(R);
}

function  v4
TransformRowMajor(m4 A, v4 P)
{
    v4 R;
    R.x = P.x*A.E[0][0] + P.y*A.E[0][1] + P.z*A.E[0][2] + P.w*A.E[0][3];
    R.y = P.x*A.E[1][0] + P.y*A.E[1][1] + P.z*A.E[1][2] + P.w*A.E[1][3];
    R.z = P.x*A.E[2][0] + P.y*A.E[2][1] + P.z*A.E[2][2] + P.w*A.E[2][3];
    R.w = P.x*A.E[3][0] + P.y*A.E[3][1] + P.z*A.E[3][2] + P.w*A.E[3][3];
    return(R);
}

m4
operator*(m4 A, m4 B)
{
  m4 Result;

  for(s32 Row = 0; Row < 4; ++Row)
  {
    for(s32 Column = 0; Column < 4; ++Column)
    {
      for(s32 ResultIndex = 0; ResultIndex < 4; ++ResultIndex)
      {
        Result[Row][Column] += A[ResultIndex][Column] * B[Row][ResultIndex];
      }
    }
  }

  return Result;
}

function  m4
Translate( v3 v )
{
  m4 Result;

  Result.E[0] = V4(1,0,0,v.x);
  Result.E[1] = V4(0,1,0,v.y);
  Result.E[2] = V4(0,0,1,v.z);
  Result.E[3] = V4(0,0,0,1);

  return Result;
}
