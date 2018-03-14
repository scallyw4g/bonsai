
struct m4
{
  v4 E[4];

  v4&
  operator[](int index)
  {
    v4& Result = this->E[index];
    return Result;
  }

  bool
  operator==(m4 &M2)
  {
    NotImplemented;
    return true;
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
    this->E[0] = v4(0,0,0,0);
    this->E[1] = v4(0,0,0,0);
    this->E[2] = v4(0,0,0,0);
    this->E[3] = v4(0,0,0,0);
  }
};

m4
operator*(m4 A, m4 B)
{
  m4 Result;

  for(u32 Row = 0; Row < 4; ++Row)
  {
    for(u32 Column = 0; Column < 4; ++Column)
    {
      for(u32 ResultIndex = 0; ResultIndex < 4; ++ResultIndex)
      {
        Result[Row][Column] += A[ResultIndex][Column] * B[Row][ResultIndex];
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

