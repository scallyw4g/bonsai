
struct rect2
{
  v2 Min;
  v2 Max;
};

struct aabb
{
  v3 Center;
  v3 Radius;

  aabb(v3 Center, v3 Radius)
  {
    this->Center = Center;
    this->Radius = Radius;
  }

  aabb(world_position Center, world_position Radius)
  {
    this->Center = V3(Center);
    this->Radius = V3(Radius);
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

