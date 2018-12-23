struct triangle;

struct edge
{
  voxel_position P0;
  voxel_position P1;
  r32 LengthSq;
};

struct triangle
{
  voxel_position* Points[3];
  u32 Index;
  /* edge* Edges[3]; */
};

struct boundary_voxels
{
  voxel_position* Points;
  u32 Count;
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

function current_triangles*
AllocateCurrentTriangles(u32 Count, memory_arena* Memory)
{
  current_triangles* Result = AllocateAlignedProtection(current_triangles, Memory, 1, 64, False);
  Result->Tris = AllocateAlignedProtection(triangle*, Memory, Count, 64, False);
  Result->Max = Count;

  return Result;
}

function triangle*
Triangle(voxel_position* P0, voxel_position* P1, voxel_position* P2, memory_arena* Memory)
{
  triangle* Result = AllocateProtection(triangle, Memory, 1, False);

  /* edge* E0 = Edge(P0, P1, Memory); */
  /* edge* E1 = Edge(P0, P2, Memory); */
  /* edge* E2 = Edge(P1, P2, Memory); */

  Result->Points[0] = P0;
  Result->Points[1] = P1;
  Result->Points[2] = P2;

  return Result;
}

function void
PushTriangle(current_triangles* CurrentTris, triangle* ToPush)
{
  Assert(CurrentTris->Count < CurrentTris->Max);
  ToPush->Index = CurrentTris->Count;
  CurrentTris->Tris[CurrentTris->Count++] = ToPush;
  return;
}

function void
DeleteFace(current_triangles* CurrentTris, u32 DeletionIndex)
{
  --CurrentTris->Count;

  CurrentTris->Tris[DeletionIndex] = CurrentTris->Tris[CurrentTris->Count];
  CurrentTris->Tris[DeletionIndex]->Index = DeletionIndex;

  return;
}

function r32
ComputeMatchSum(triangle *Triangle, voxel_position* Point)
{
  r32 Result = 0.0f;

  Result += LengthSq(*(Triangle->Points[0]) - *Point);
  Result += LengthSq(*(Triangle->Points[1]) - *Point);
  Result += LengthSq(*(Triangle->Points[2]) - *Point);

  return Result;
}

function triangle*
FindClosestFace(current_triangles* CurrentTriangles, voxel_position* Point)
{
  triangle* BestMatch = 0;
  r32 BestMatchSum = FLT_MAX;

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

  return BestMatch;
}

function void
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

function void
Swap(voxel_position* P1, voxel_position* P2)
{
  voxel_position Tmp = *P1;
  *P1 = *P2;
  *P2 = Tmp;

  return;
}

