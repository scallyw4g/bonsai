
struct linei
{
  voxel_position MinP;
  voxel_position MaxP;

  linei() {}

  linei(voxel_position MinP_in, voxel_position MaxP_in)
  {
    this->MinP = MinP_in;
    this->MaxP = MaxP_in;
  }

  linei(v3 MinP_in, v3 MaxP_in)
  {
    this->MinP = Voxel_Position(MinP_in);
    this->MaxP = Voxel_Position(MaxP_in);
  }

};

struct line
{
  v3 MinP;
  v3 MaxP;

  line() {}

  line(v3 MinP_in, v3 MaxP_in)
  {
    this->MinP = MinP_in;
    this->MaxP = MaxP_in;
  }

  line(voxel_position MinP_in, voxel_position MaxP_in)
  {
    this->MinP = V3(MinP_in);
    this->MaxP = V3(MaxP_in);
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

