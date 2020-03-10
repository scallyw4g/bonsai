
struct rect2
{
  v2 Min;
  v2 Max;
};

function rect2
InvertedInfinityRectangle()
{
  rect2 Result = {
    .Min = V2(f32_MAX),
    .Max = V2(-f32_MAX),
  };
  return Result;
}

function rect2
RectMinMax(v2 Min, v2 Max)
{
  rect2 Result = { .Min = Min, .Max = Max };
  return Result;
}

function rect2
RectMinDim(v2 Min, v2 Dim)
{
  rect2 Result = {Min, Min+Dim};
  return Result;
}

function v2
BottomLeft(rect2 Rect)
{
  v2 Result = V2(Rect.Min.x, Rect.Max.y);
  return Result;
}

function v2
TopRight(rect2 Rect)
{
  v2 Result = V2(Rect.Max.x, Rect.Min.y);
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

  aabb() { Clear(this); }
};

inline b32
Intersect(aabb *First, aabb *Second)
{
  b32 Result = True;

  Result &= (Abs(First->Center.x - Second->Center.x) < (First->Radius.x + Second->Radius.x));
  Result &= (Abs(First->Center.y - Second->Center.y) < (First->Radius.y + Second->Radius.y));
  Result &= (Abs(First->Center.z - Second->Center.z) < (First->Radius.z + Second->Radius.z));

  return Result;
}

function aabb
MinMaxAABB(v3 Min, v3 Max)
{
  v3 Radius = (Max - Min)/2.0f;
  v3 Center = Min + Radius;
  aabb Result(Center, Radius);
  return Result;
}

function rect2
operator-(rect2 R1, rect2 R2)
{
  rect2 Result = {
    .Min = R1.Min - R2.Min,
    .Max = R1.Max - R2.Max,
  };
  return Result;
}

function aabb
operator+(aabb AABB, v3 V)
{
  aabb Result = AABB;
  Result.Radius = AABB.Radius + V;
  return Result;
}

function v3
HalfDim( v3 P1 )
{
  v3 Result = P1 / 2;
  return Result;
}

struct ray
{
  v3 Origin;
  v3 Dir;
};

// TODO(Jesse, speed, aabb): This could be optimized significantly
function b32
Intersect(aabb AABB, ray Ray)
{
  v3 min = AABB.Center - AABB.Radius;;
  v3 max = AABB.Center + AABB.Radius;;

  r32 tmin = (min.x - Ray.Origin.x) / Ray.Dir.x;
  r32 tmax = (max.x - Ray.Origin.x) / Ray.Dir.x;

  if (tmin > tmax)
  {
    r32 temp = tmin;
    tmin = tmax;
    tmax = temp;
  }

  r32 tymin = (min.y - Ray.Origin.y) / Ray.Dir.y;
  r32 tymax = (max.y - Ray.Origin.y) / Ray.Dir.y;

  if (tymin > tymax)
  {
    r32 temp = tymin;
    tymin = tymax;
    tymax = temp;
  }

  if ((tmin > tymax) || (tymin > tmax))
  return false;

  if (tymin > tmin)
  tmin = tymin;

  if (tymax < tmax)
  tmax = tymax;

  r32 tzmin = (min.z - Ray.Origin.z) / Ray.Dir.z;
  r32 tzmax = (max.z - Ray.Origin.z) / Ray.Dir.z;

  if (tzmin > tzmax)
  {
    r32 temp = tzmin;
    tzmin = tzmax;
    tzmax = temp;
  }

  if ((tmin > tzmax) || (tzmin > tmax))
  return false;

  if (tzmin > tmin)
  tmin = tzmin;

  if (tzmax < tmax)
  tmax = tzmax;

  return true;
}

function b32
IsInsideRect(rect2 Rect, v2 P)
{
  b32 Result = (P >= Rect.Min && P < Rect.Max);
  return Result;
}

function v3
GetMin(aabb Rect)
{
  v3 Result = Rect.Center - Rect.Radius;
  return Result;
}

function v3
GetMax(aabb Rect)
{
  v3 Result = Rect.Center + Rect.Radius;
  return Result;
}

v2
GetDim(rect2 Rect)
{
  v2 Dim = Rect.Max - Rect.Min;
  return Dim;
}
