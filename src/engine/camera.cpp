
link_internal void
UpdateCameraP(world *World, cp TargetViewP, camera *Camera)
{
  TIMED_FUNCTION();
  if (Camera->DistanceFromTarget < 0.1f) { Camera->DistanceFromTarget = 0.1f; }

  r32 Px = Sin(Camera->Yaw);
  r32 Py = Cos(Camera->Yaw);
  r32 Pz = Cos(Camera->Pitch);

  Camera->Front = Normalize(V3(Px, Py, Pz));

  Camera->Right = Normalize(Cross(V3(0,0,1), Camera->Front));
  Camera->Up = Normalize(Cross(Camera->Front, Camera->Right));

  /* Camera->ViewingTarget = TargetViewP; */

  auto NewCameraP = Canonicalize(World->ChunkDim, TargetViewP - (Camera->Front*Camera->DistanceFromTarget));
  Camera->CurrentP = NewCameraP;
  Camera->CurrentP = Canonicalize(World->ChunkDim, Camera->CurrentP);

  Camera->RenderSpacePosition = GetRenderP(World->ChunkDim, Camera->CurrentP, Camera);

#if 1

  //
  // Frustum computation
  //
  v3 FrustLength = V3(0.0f, 0.0f, Camera->Frust.farClip);
  v3 FarHeight = ( V3( 0.0f, ((Camera->Frust.farClip - Camera->Frust.nearClip)/Cos(Camera->Frust.FOV/2.0f)) * Sin(Camera->Frust.FOV/2.0f), 0.0f));
  v3 FarWidth = V3( FarHeight.y, 0.0f, 0.0f);

  v3 MaxMax = Normalize(FrustLength + FarHeight + FarWidth);
  v3 MaxMin = Normalize(FrustLength + FarHeight - FarWidth);
  v3 MinMax = Normalize(FrustLength - FarHeight + FarWidth);
  v3 MinMin = Normalize(FrustLength - FarHeight - FarWidth);

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

  MaxMin = Normalize(Rotate(MaxMin, FinalRotation));
  MaxMax = Normalize(Rotate(MaxMax, FinalRotation));
  MinMin = Normalize(Rotate(MinMin, FinalRotation));
  MinMax = Normalize(Rotate(MinMax, FinalRotation));

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

  plane Top(CameraSimP,   Normalize(Cross(MaxMax, MaxMin)));
  plane Bot(CameraSimP,   Normalize(Cross(MinMin, MinMax)));
  plane Left(CameraSimP,  Normalize(Cross(MinMax, MaxMax)));
  plane Right(CameraSimP, Normalize(Cross(MaxMin, MinMin)));

  Camera->Frust.Top = Top;
  Camera->Frust.Bot = Bot;
  Camera->Frust.Left = Left;
  Camera->Frust.Right = Right;

#endif

  // TODO(Jesse, id: 128, tags: correctness, robustness, speed): Do culling on these as well?
  /* plane Near; */
  /* plane Far; */


  return;
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
                  v2 MouseDelta,
                  r32 CameraZoomDelta,
                  cp TargetViewP,
                  camera *Camera,
                  r32 Dt )
{
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
  Camera->DistanceFromTarget += CameraZoomDelta * Camera->DistanceFromTarget;

  Camera->DistanceFromTarget = ClampMin(Camera->DistanceFromTarget, Camera->Frust.nearClip);
  Camera->DistanceFromTarget = ClampMax(Camera->DistanceFromTarget, Camera->Frust.farClip);

  UpdateCameraP(World, TargetViewP, Camera);
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
  v2 UpdateMouseDelta = {};
  f32 CameraZoomDelta = {};
  if (Input) // TODO(Jesse): Assert here ..?
  {
    if (DoPositionDelta)
    {
      UpdateMouseDelta = Input->LMB.Pressed ? MouseDelta : V2(0);
    }

    if (DoZoomDelta)
    {
      CameraZoomDelta = Input->MouseWheelDelta/200.f;
    }

    if (Input->RMB.Pressed) { CameraZoomDelta += MouseDelta.y; }
  }

  UpdateGameCamera(World, UpdateMouseDelta, CameraZoomDelta, NewTarget, Camera, Dt);
}

link_internal void
StandardCamera(camera* Camera, f32 FarClip, f32 DistanceFromTarget, f32 Blend, cp InitialTarget)
{
  Clear(Camera);

  Camera->Blend = Blend;

  Camera->Frust.farClip = FarClip;
  Camera->Frust.nearClip = 1.0f;
  Camera->Frust.width = 30.0f;
  Camera->Frust.FOV = 45.0f;

  Camera->Up = WORLD_Z;
  Camera->Right = WORLD_X;

  Camera->TargetPitch = PI32 - (PI32*0.25f);
  Camera->TargetYaw = -PI32*0.15f;

  Camera->DistanceFromTarget = DistanceFromTarget;
}

link_internal bool
IsInFrustum(world *World, camera *Camera, canonical_position P)
{
  bool Result = true;

  v3 TestP = GetSimSpaceP(World, P);

  // This says if we're on the back-side of the plane by more than the dim of a
  // world chunk, we're outside the frustum
  /* Result &= (DistanceToPlane(&Camera->Frust.Top, TestP)   < -1*World->ChunkDim.y); */
  /* Result &= (DistanceToPlane(&Camera->Frust.Bot, TestP)   < -1*World->ChunkDim.y); */
  /* Result &= (DistanceToPlane(&Camera->Frust.Left, TestP)  < -1*World->ChunkDim.x); */
  /* Result &= (DistanceToPlane(&Camera->Frust.Right, TestP) < -1*World->ChunkDim.x); */

  Result &= (DistanceToPlane(&Camera->Frust.Top, TestP)   > 0);
  Result &= (DistanceToPlane(&Camera->Frust.Bot, TestP)   > 0);
  Result &= (DistanceToPlane(&Camera->Frust.Left, TestP)  > 0);
  Result &= (DistanceToPlane(&Camera->Frust.Right, TestP) > 0);

  return Result;
}

link_internal bool
IsInFrustum( world *World, camera *Camera, world_chunk *Chunk )
{
  v3 ChunkMid = World->ChunkDim/2.0f;
  canonical_position P1 = Canonical_Position(ChunkMid, Chunk->WorldP );
  bool Result = IsInFrustum(World, Camera, P1);
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
ComputeRayFromCursor(engine_resources *Engine, m4* ViewProjection, camera *Camera, v3i WorldChunkDim)
{
  platform *Plat = &Engine->Stdlib.Plat;
  world *World = Engine->World;

  maybe_ray Result = {};

  m4 InverseViewProjection = {};
  b32 Inverted = Inverse((r32*)ViewProjection, (r32*)&InverseViewProjection);
  if (Inverted)
  {
    v3 MouseMinWorldP = Unproject( Plat->MouseP,
                                   0.0f,
                                   Plat->ScreenDim,
                                   &InverseViewProjection);

    v3 MouseMaxWorldP = Unproject( Plat->MouseP,
                                   1.0f,
                                   Plat->ScreenDim,
                                   &InverseViewProjection);

    v3 RayDirection = Normalize(MouseMaxWorldP - MouseMinWorldP);

    /* DEBUG_HighlightVoxel(Engine, Camera->ViewingTarget, RED); */
    /* v3 CameraOffset = Camera->ViewingTarget.Offset + V3(Camera->ViewingTarget.WorldP * WorldChunkDim); */
    v3 CameraOffset = GetSimSpaceP(World, ComputeTarget(Camera));
    /* v3 CameraOffset = V3(0); */
    Result.Ray = { .Origin = MouseMinWorldP + CameraOffset, .Dir = RayDirection };
    Result.Tag = Maybe_Yes;
  }

  return Result;
}
