struct triangle;

struct edge
{
  voxel_position P0;
  voxel_position P1;
  r32 LengthSq;
};

struct triangle
{
  voxel_position Points[3];
  u32 Index;
  /* edge* Edges[3]; */
};

struct boundary_voxels
{
  voxel_position* Points;
  u32 At;
  u32 End;

  voxel_position Min;
  voxel_position Max;
};

struct current_triangles
{
  triangle** Tris;
  u32 Max;
  u32 Count;

  //
  // Debug Values
  //

  u32 CurrentPointIndex;
  boundary_voxels* SurfacePoints;
};

bonsai_function void
PushTriangle(current_triangles* CurrentTris, triangle* ToPush)
{
  Assert(CurrentTris->Count < CurrentTris->Max);
  ToPush->Index = CurrentTris->Count;
  CurrentTris->Tris[CurrentTris->Count++] = ToPush;
  return;
}

bonsai_function void
DeleteFace(current_triangles* CurrentTris, u32 DeletionIndex)
{
  --CurrentTris->Count;

  CurrentTris->Tris[DeletionIndex] = CurrentTris->Tris[CurrentTris->Count];
  CurrentTris->Tris[DeletionIndex]->Index = DeletionIndex;

  return;
}

#if 0

#if 1

bonsai_function r32
ComputeMatchSum(triangle *Triangle, voxel_position* Point)
{
  r32 Result = 0.0f;

  voxel_position** Points = Triangle->Points;
  v3 P0 = Normalize(V3(*Points[0]));
  v3 P1 = Normalize(V3(*Points[1]));
  v3 P2 = Normalize(V3(*Points[2]));
  v3 PNormalized = Normalize(V3(*Point));

  Result += Dot(P0, PNormalized);
  Result += Dot(P1, PNormalized);
  Result += Dot(P2, PNormalized);

  return Result;
}

#else
bonsai_function r32
ComputeMatchSum(triangle *Triangle, voxel_position* Point)
{
  r32 Result = 0.0f;

  Result += LengthSq(*(Triangle->Points[0]) - *Point);
  Result += LengthSq(*(Triangle->Points[1]) - *Point);
  Result += LengthSq(*(Triangle->Points[2]) - *Point);

  return Result;
}
#endif

bonsai_function triangle*
FindClosestFace(current_triangles* CurrentTriangles, voxel_position* Point)
{
  triangle* BestMatch = CurrentTriangles->Tris[0];
  r32 BestMatchSum = f32_MAX;

  for (u32 FaceIndex = 0;
      FaceIndex < CurrentTriangles->Count;
      ++FaceIndex)
  {
    triangle **ThisMatch = CurrentTriangles->Tris + FaceIndex;
    r32 ThisMatchSum = ComputeMatchSum(*ThisMatch, Point);
    if (ThisMatchSum < BestMatchSum)
    {
      BestMatch = *ThisMatch;
      BestMatchSum = ThisMatchSum;
    }
  }

  Assert(BestMatch);
  return BestMatch;
}

bonsai_function void
SplitFace(current_triangles* CurrentTriangles, triangle* Closest, voxel_position* Point, memory_arena* Memory)
{
  triangle* NewTri0 = Triangle(Closest->Points[0], Closest->Points[1], Point, Memory);
  triangle* NewTri1 = Triangle(Closest->Points[1], Closest->Points[2], Point, Memory);
  triangle* NewTri2 = Triangle(Closest->Points[2], Closest->Points[0], Point, Memory);

  PushTriangle(CurrentTriangles, NewTri0);
  PushTriangle(CurrentTriangles, NewTri1);
  PushTriangle(CurrentTriangles, NewTri2);

  DeleteFace(CurrentTriangles, Closest->Index);

  return;
}

#endif

bonsai_function void
Swap(voxel_position* P1, voxel_position* P2)
{
  voxel_position Tmp = *P1;
  *P1 = *P2;
  *P2 = Tmp;

  return;
}

