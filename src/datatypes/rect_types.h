
struct rect2
{
  v2 Min;
  v2 Max;
};

rect2
RectMinDim(v2 Min, v2 Dim)
{
  rect2 Result = {Min, Min+Dim};
  return Result;
}

struct aabb
{
  v3 Center;
  v3 Radius;

  aabb(v3 Center_in, v3 Radius_in)
  {
    this->Center = Center_in;
    this->Radius = Radius_in;
  }

  aabb(world_position Center_in, world_position Radius_in)
  {
    this->Center = V3(Center_in);
    this->Radius = V3(Radius_in);
  }

  aabb()
  {
    this->Center = V3(0,0,0);
    this->Radius = V3(0,0,0);
  }

};

inline aabb
operator+(aabb AABB, v3 V)
{
  aabb Result = AABB;
  Result.Radius = AABB.Radius + V;
  return Result;
}

v3
HalfDim( v3 P1 )
{
  v3 Result = P1 / 2;
  return Result;
}

