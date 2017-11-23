
struct linei
{
  voxel_position MinP;
  voxel_position MaxP;

  linei() {}

  linei(voxel_position MinP, voxel_position MaxP)
  {
    this->MinP = MinP;
    this->MaxP = MaxP;
  }

  linei(v3 MinP, v3 MaxP)
  {
    this->MinP = Voxel_Position(MinP);
    this->MaxP = Voxel_Position(MaxP);
  }

};

struct line
{
  v3 MinP;
  v3 MaxP;

  line() {}

  line(v3 MinP, v3 MaxP)
  {
    this->MinP = MinP;
    this->MaxP = MaxP;
  }

  line(voxel_position MinP, voxel_position MaxP)
  {
    this->MinP = V3(MinP);
    this->MaxP = V3(MaxP);
  }
};


inline line
operator+(line A, v3 B)
{
  line Result;

  Result.MinP.x = A.MinP.x + B.x;
  Result.MinP.y = A.MinP.y + B.y;
  Result.MinP.z = A.MinP.z + B.z;

  Result.MaxP.x = A.MaxP.x + B.x;
  Result.MaxP.y = A.MaxP.y + B.y;
  Result.MaxP.z = A.MaxP.z + B.z;

  return Result;
}

inline line
operator-(line A, v3 B)
{
  line Result;

  Result.MinP.x = A.MinP.x - B.x;
  Result.MinP.y = A.MinP.y - B.y;
  Result.MinP.z = A.MinP.z - B.z;

  Result.MaxP.x = A.MaxP.x - B.x;
  Result.MaxP.y = A.MaxP.y - B.y;
  Result.MaxP.z = A.MaxP.z - B.z;

  return Result;
}

