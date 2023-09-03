struct ray
{
  v3 Origin;
  v3 Dir;
};

ray Ray(v3 Origin, v3 Dir)
{
  ray Result;
  Result.Origin = Origin;
  Result.Dir = Dir;
  return Result;
}

enum maybe_tag
{
  Maybe_No,
  Maybe_Yes,
};

struct maybe_ray
{
  maybe_tag Tag;
  ray Ray;
};


struct aabb_intersect_result
{
  face_index Face;
  r32 t;
};

// TODO(Jesse, id: 92, tags: speed, aabb): This could be SIMD optimized ?
//
// NOTE(Jesse): Result is tmin value along the ray .. ie closest intersection
// to origin of the ray.  f32_MAX means no intersection
// 
// TODO(Jesse): Should this not take an aabb pointer?!
link_internal aabb_intersect_result
Intersect(aabb AABB, ray *Ray)
{
  v3 AABBMin = AABB.Center - AABB.Radius;
  v3 AABBMax = AABB.Center + AABB.Radius;

  // Intersection of the the xMin plane
  r32 tmin = (AABBMin.x - Ray->Origin.x) / Ray->Dir.x;
  face_index Face = FaceIndex_Left;

  // Intersection of the the xMax plane
  r32 tmax = (AABBMax.x - Ray->Origin.x) / Ray->Dir.x;

  if (tmin > tmax)
  {
    r32 temp = tmin;
    tmin = tmax;
    tmax = temp;

    Face = FaceIndex_Right;
  }

  r32 tymin = (AABBMin.y - Ray->Origin.y) / Ray->Dir.y;
  r32 tymax = (AABBMax.y - Ray->Origin.y) / Ray->Dir.y;

  face_index yFace = FaceIndex_Front;
  if (tymin > tymax)
  {
    r32 temp = tymin;
    tymin = tymax;
    tymax = temp;
    yFace = FaceIndex_Back;
  }

  if ((tmin > tymax) || (tymin > tmax))
  return { FaceIndex_None, f32_MAX };

  // Intersected the yMin face before the xMin face
  if (tymin > tmin) { tmin = tymin; Face = yFace; }

  // Intersected the yMax face before the xMax face
  if (tymax < tmax) tmax = tymax;

  r32 tzmin = (AABBMin.z - Ray->Origin.z) / Ray->Dir.z;
  r32 tzmax = (AABBMax.z - Ray->Origin.z) / Ray->Dir.z;

  face_index zFace = FaceIndex_Bot;
  if (tzmin > tzmax)
  {
    r32 temp = tzmin;
    tzmin = tzmax;
    tzmax = temp;
    zFace = FaceIndex_Top;
  }

  if ((tmin > tzmax) || (tzmin > tmax))
  return { FaceIndex_None, f32_MAX };

  // Intersected the zMin face before the yMin face
  if (tzmin > tmin) { tmin = tzmin; Face = zFace; }

  // Intersected the zMax face before the yMax face
  if (tzmax < tmax) tmax = tzmax;

  return {Face, tmin};
}

/* // TODO(Jesse): Remove this? */
/* link_internal f32 */
/* Intersect(aabb AABB, ray Ray) */
/* { */
/*   f32 Result = Intersect(AABB, &Ray); */
/*   return Result; */
/* } */

