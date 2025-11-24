
link_internal void
UpdateCameraP(world *World, cp TargetViewP, camera *Camera)
{
}

link_inline v2
GetMouseDelta(platform *Plat)
{
  r32 DPToWorldModifier = -0.001f;
  v2 Result = Plat->MouseDP * DPToWorldModifier;
  return Result;
}

link_internal void
UpdateGameCamera( world *World,
                     v2  ScreenDim,
                     v2  MouseDelta,
                    r32  CameraZoomDelta,
                     cp  TargetViewP,
                 camera *Camera,
                    r32  Dt )
{
  TIMED_FUNCTION();

  // TODO(Jesse): Make these vary by DistanceFromTarget, such that the mouse feels the same amount of sensitive zoomed in as out.
  Camera->TargetYaw += MouseDelta.x;
  Camera->TargetPitch += MouseDelta.y;
  Camera->TargetPitch = ClampBetween(0.0, Camera->TargetPitch, PI32);

  r32 t = 1.f;
  if (Camera->Blend > 0.f && Dt > 0.f)
  {
    t = Min(1.f, Camera->Blend*Dt);
  }

  Camera->Yaw   = Lerp(t, Camera->Yaw,   Camera->TargetYaw);
  Camera->Pitch = Lerp(t, Camera->Pitch, Camera->TargetPitch);

  /* Camera->DistanceFromTarget += MouseDelta.y*Camera->DistanceFromTarget; */
  Camera->TargetDistanceFromTarget += CameraZoomDelta * Camera->DistanceFromTarget;

  Camera->DistanceFromTarget = Lerp(t, Camera->DistanceFromTarget, Camera->TargetDistanceFromTarget);

  Camera->DistanceFromTarget = ClampMin(Camera->DistanceFromTarget, Camera->Frust.nearClip);
  Camera->DistanceFromTarget = ClampMax(Camera->DistanceFromTarget, Camera->Frust.farClip);

  Camera->TargetDistanceFromTarget = ClampMin(Camera->TargetDistanceFromTarget, Camera->Frust.nearClip);
  Camera->TargetDistanceFromTarget = ClampMax(Camera->TargetDistanceFromTarget, Camera->Frust.farClip);

  // TODO(Jesse): Remove?
  if (Camera->DistanceFromTarget < 0.1f) { Camera->DistanceFromTarget = 0.1f; }
  if (Camera->TargetDistanceFromTarget < 0.1f) { Camera->TargetDistanceFromTarget = 0.1f; }

  r32 Px = Sin(Camera->Yaw);
  r32 Py = Cos(Camera->Yaw);
  r32 Pz = Cos(Camera->Pitch);

  Camera->Front = Normalize(V3(Px, Py, Pz));

  Camera->Right = Normalize(Cross(V3(0,0,1), Camera->Front));
  Camera->Up    = Normalize(Cross(Camera->Front, Camera->Right));

  auto NewCameraP = Canonicalize(World->ChunkDim, TargetViewP - (Camera->Front*Camera->DistanceFromTarget));
  Camera->CurrentP = NewCameraP;
  Camera->CurrentP = Canonicalize(World->ChunkDim, Camera->CurrentP);

  Camera->RenderSpacePosition = GetRenderP(World->ChunkDim, Camera->CurrentP, Camera);

  m4 View = ViewMatrix(World->ChunkDim, Camera);
  m4 Proj = ProjectionMatrix(Camera, ScreenDim);

  Camera->ViewProjection = Proj * View;
  Camera->InverseViewMatrix = Inverse(View);
  Camera->InverseProjectionMatrix = Inverse(Proj);




#if 1

  //
  // Frustum computation
  //
  /* v3 FrustLength = V3(0.0f, 0.0f, Camera->Frust.farClip); */
  /* v3 FarHeight = ( V3( 0.0f, */
  /*                     ((Camera->Frust.farClip - Camera->Frust.nearClip)/Cos(Camera->Frust.FOV/2.0f)) * Sin(Camera->Frust.FOV/2.0f), */
  /*                     0.0f)); */
  /* v3 FarWidth = V3( FarHeight.y, 0.0f, 0.0f); */

  /* v3 MaxMax = Normalize(FrustLength + FarHeight + FarWidth); */
  /* v3 MaxMin = Normalize(FrustLength + FarHeight - FarWidth); */
  /* v3 MinMax = Normalize(FrustLength - FarHeight + FarWidth); */
  /* v3 MinMin = Normalize(FrustLength - FarHeight - FarWidth); */

  v3 Front = V3(0,0,1);
  v3 Target = Camera->Front;

  Quaternion GrossRotation = RotatePoint(Front, Target);

  // We've got to correct the rotation so it ends pointing the frustum in the cameras 'up' direction
  v3 UpVec = V3(0, 1, 0);
  /* v3 UpVec = Camera->Up; */
  v3 RotatedUp = Rotate(UpVec, GrossRotation);
  Quaternion DesiredUp = RotatePoint(RotatedUp, Camera->Up);

  Quaternion FinalRotation = DesiredUp * GrossRotation;

  /* MaxMin = Normalize(Rotate(MaxMin, GrossRotation)); */
  /* MaxMax = Normalize(Rotate(MaxMax, GrossRotation)); */
  /* MinMin = Normalize(Rotate(MinMin, GrossRotation)); */
  /* MinMax = Normalize(Rotate(MinMax, GrossRotation)); */

  /* MaxMin = Normalize(Rotate(MaxMin, FinalRotation)); */
  /* MaxMax = Normalize(Rotate(MaxMax, FinalRotation)); */
  /* MinMin = Normalize(Rotate(MinMin, FinalRotation)); */
  /* MinMax = Normalize(Rotate(MinMax, FinalRotation)); */

#if DEBUG_DRAW_FRUSTUM_AT_ORIGIN
  {
    auto Resources = GetEngineResources();
    auto *GpuBuffer = &GetCurrentGpuMap(Resources->Graphics)->Buffer;
    auto Dest = ReserveBufferSpace(GpuBuffer, VERTS_PER_LINE*4);
    DEBUG_DrawLine(&Dest, line(V3(0), MaxMax*40), RED, 0.2f );
    DEBUG_DrawLine(&Dest, line(V3(0), MaxMin*40), BLUE, 0.2f );
    DEBUG_DrawLine(&Dest, line(V3(0), MinMax*40), GREEN, 0.2f );
    DEBUG_DrawLine(&Dest, line(V3(0), MinMin*40), YELLOW, 0.2f );
  }
#endif

  v3 CameraSimP = GetSimSpaceP(World, Camera->CurrentP);

#if 0
  plane Top   = Plane(CameraSimP, Normalize(Cross(MaxMax, MaxMin)));
  plane Bot   = Plane(CameraSimP, Normalize(Cross(MinMin, MinMax)));
  plane Left  = Plane(CameraSimP, Normalize(Cross(MinMax, MaxMax)));
  plane Right = Plane(CameraSimP, Normalize(Cross(MaxMin, MinMin)));

  Camera->Frust.Top = Top;
  Camera->Frust.Bot = Bot;
  Camera->Frust.Left = Left;
  Camera->Frust.Right = Right;
#else
  auto zNear = Camera->Frust.nearClip;
  auto zFar = Camera->Frust.farClip;
  auto fovY = Camera->Frust.FOV;
  auto aspect = ScreenDim.x/ScreenDim.y;

  f32 halfVSide = zFar * Tan(Rads(fovY*0.5f));
  f32 halfHSide = halfVSide * aspect;
  v3 frontMultFar = zFar * Camera->Front;

  auto SimP = GetSimSpaceP(World, Camera->CurrentP);

  /* Camera->Frust.Near   = Plane( Camera->RenderSpacePosition + zNear * Camera->Front, Camera->Front ); */
  /* Camera->Frust.Far    = Plane( Camera->RenderSpacePosition + frontMultFar, -1.f*Camera->Front ); */
  Camera->Frust.Right  = Plane( SimP, Normalize(Cross(frontMultFar - Camera->Right * halfHSide, Camera->Up)) );
  Camera->Frust.Left   = Plane( SimP, Normalize(Cross(Camera->Up,frontMultFar + Camera->Right * halfHSide)) );
  Camera->Frust.Top    = Plane( SimP, Normalize(Cross(Camera->Right, frontMultFar - Camera->Up * halfVSide)) );
  Camera->Frust.Bottom = Plane( SimP, Normalize(Cross(frontMultFar + Camera->Up * halfVSide, Camera->Right)) );
#endif

#endif

  // TODO(Jesse, id: 128, tags: correctness, robustness, speed): Do culling on these as well?
  /* plane Near; */
  /* plane Far; */


  return;
}


link_internal void
SetCameraTarget(v3 Target, camera *Camera)
{
  engine_resources *Engine = GetEngineResources();
  world *World = Engine->World;
  UpdateGameCamera(World, Engine->Stdlib.Plat.ScreenDim, {}, {}, Canonical_Position(World->ChunkDim, Target, {}), Camera, 0.33f);
}

link_internal void
UpdateGameCamera( world *World,
                  v2 MouseDelta,
                  input *Input,
                  canonical_position NewTarget,
                  camera *Camera,
                  r32 Dt,
                  b32 DoPositionDelta,
                  b32 DoZoomDelta )
{
  engine_resources *Engine = GetEngineResources();

  v2 UpdateMouseDelta = {};
  f32 CameraZoomDelta = {};
  if (Input) // TODO(Jesse): Assert here ..?
  {
    if (DoPositionDelta)
    {
      UpdateMouseDelta = Input->RMB.Pressed ? MouseDelta : V2(0);
    }

    if (DoZoomDelta)
    {
      CameraZoomDelta = -1.f*Input->MouseWheelDelta/500.f;
      /* if (Input->RMB.Pressed) { CameraZoomDelta += MouseDelta.y; } */
    }

  }

  UpdateGameCamera(World, Engine->Stdlib.Plat.ScreenDim, UpdateMouseDelta, CameraZoomDelta, NewTarget, Camera, Dt);
}

link_internal void
StandardCamera(camera* Camera, f32 FarClip, f32 DistanceFromTarget, f32 Blend)
{
  *Camera = {};

  Camera->Blend = Blend;

  Camera->Frust.farClip = FarClip;
  Camera->Frust.nearClip = 0.0005f;

  // Someone already set FOV .. probably when deserializing stored runtime settings.
  if (Camera->Frust.FOV == 0.f)
  {
    Camera->Frust.FOV = 75.f;
  }

  Camera->Up = WORLD_Z;
  Camera->Right = WORLD_X;

  Camera->TargetPitch = PI32 - (PI32*0.25f);
  Camera->TargetYaw = PI32*0.125f;

  Camera->TargetDistanceFromTarget = DistanceFromTarget;

  // NOTE(Jesse): Can't do this because this gets called before the world's
  // initialized.  I guess we could check and conditionall call it .. but .. meh
  // UpdateGameCamera(GetWorld(), {}, {}, {}, Camera, 0.f);
}

link_internal bool
IsInFrustum(world *World, camera *Camera, canonical_position P)
{
  bool Result = true;

  v3 TestP = GetSimSpaceP(World, P);

  Result &= (DistanceToPlane(&Camera->Frust.Top, TestP)    > 0.f);
  Result &= (DistanceToPlane(&Camera->Frust.Bottom, TestP) > 0.f);
  Result &= (DistanceToPlane(&Camera->Frust.Left, TestP)   > 0.f);
  Result &= (DistanceToPlane(&Camera->Frust.Right, TestP)  > 0.f);

  return Result;
}

link_internal f32
SignedDistanceToFrustum( camera *Camera, v3 P)
{
  f32 Result =            (DistanceToPlane(&Camera->Frust.Top,    P));
      Result = Max(Result, DistanceToPlane(&Camera->Frust.Bottom, P));
      Result = Max(Result, DistanceToPlane(&Camera->Frust.Left,   P));
      Result = Max(Result, DistanceToPlane(&Camera->Frust.Right,  P));
  return Result;
}

link_internal bool
IsInFrustum( world *World, camera *Camera, world_chunk *Chunk )
{
  Assert(Chunk->Dim == V3i(64));
  v3  Dim    = V3(Chunk->Dim)*V3(Chunk->DimInChunks);
  r32 Radius = Length(Dim)/2.f;
   v3 Center = GetSimSpaceP(World, Chunk) + Radius;
  b32 Result = SignedDistanceToFrustum(Camera, Center) < (Radius);
  /* b32 Result = True; */
  return Result;
}

link_internal bool
IsInFrustum( world *World, camera *Camera, octree_node *Node )
{
  r32 Radius = Length(V3(64)*V3(Node->Resolution))/2.f;
   v3 Center = GetSimSpaceP(World, Node->WorldP) + Radius;
  /* b32 Result = SignedDistanceToFrustum(Camera, Center) < Radius; */
  b32 Result = SignedDistanceToFrustum(Camera, Center) < (Radius);
  return Result;
}


link_internal v3
Unproject(v2 ScreenP, r32 ClipZDepth, v2 ScreenDim, m4 *InvViewProj)
{
  TIMED_FUNCTION();

  Assert(ClipZDepth <= 1.0f);
  Assert(ClipZDepth >= 0.0f);

  v4 ClipCoords = (2.0f * V4(V3(ScreenP / ScreenDim, ClipZDepth), 1.0f)) -1.0f;
  ClipCoords.y *= -1;

  v4 WorldSpace = TransformColumnMajor(*InvViewProj, ClipCoords);
  v3 Result = WorldSpace.xyz / WorldSpace.w;

  return Result;
}

// TODO(Jesse): Not sure where this goes.. maybe here?
//
// NOTE(Jesse): Returns a sim-space ray
link_internal maybe_ray
ComputeCameraSpaceRayFromCursor(engine_resources *Engine, camera *Camera, v3i WorldChunkDim)
{
  platform *Plat = &Engine->Stdlib.Plat;
  world *World = Engine->World;

  maybe_ray Result = {};
  if (Plat->ScreenDim > V2(0))
  {
    m4 InverseViewProjection;
    if (Inverse(&Camera->ViewProjection.E[0][0], &InverseViewProjection.E[0][0]))
    {
      v3 MouseMinWorldP = Unproject( Plat->MouseP,
                                     0.1f,
                                     Plat->ScreenDim,
                                    &InverseViewProjection);

      v3 MouseMaxWorldP = Unproject( Plat->MouseP,
                                     0.5f,
                                     Plat->ScreenDim,
                                    &InverseViewProjection);

      v3 RayDirection = Normalize(MouseMaxWorldP - MouseMinWorldP);

      /* DEBUG_HighlightVoxel(Engine, Camera->ViewingTarget, RED); */
      /* v3 CameraOffset = Camera->ViewingTarget.Offset + V3(Camera->ViewingTarget.WorldP * WorldChunkDim); */
      /* v3 CameraOffset = GetSimSpaceP(World, ComputeTarget(Camera)); */
      /* v3 CameraOffset = GetSimSpaceP(World, Camera->CurrentP); */
      /* v3 CameraOffset = Camera->RenderSpacePosition; */
      v3 CameraOffset = V3(0);
      Result.Ray = { .Origin = MouseMinWorldP + CameraOffset, .Dir = RayDirection };
      Result.Tag = Maybe_Yes;
    }
  }

  return Result;
}
