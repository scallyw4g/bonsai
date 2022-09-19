
link_internal void
UpdateCameraP(canonical_position NewTarget, camera *Camera, chunk_dimension WorldChunkDim)
{
  TIMED_FUNCTION();
  if (Camera->DistanceFromTarget <= 0.1f)
  {
    Camera->DistanceFromTarget = 0.1f;
  }

  r32 Px = Sin(Camera->Yaw);
  r32 Py = Cos(Camera->Yaw);
  r32 Pz = Cos(Camera->Pitch);

  Camera->Front = Normalize(V3(Px, Py, Pz));

  Camera->Right = Normalize(Cross(V3(0,0,1), Camera->Front));
  Camera->Up = V3(0,0,1); // Normalize(Cross(Camera->Front, Camera->Right));

  Camera->ViewingTarget = NewTarget;

  Camera->TargetP = Canonicalize(WorldChunkDim, NewTarget - (Camera->Front*Camera->DistanceFromTarget));
  Camera->CurrentP = Lerp(0.25f, Camera->CurrentP, Camera->TargetP, WorldChunkDim);

  Camera->RenderSpacePosition = GetRenderP(WorldChunkDim, Camera->CurrentP, Camera);

#if 1

  //
  // Frustum computation
  //
  v3 FrustLength = V3(0.0f,0.0f, Camera->Frust.farClip);
  v3 FarHeight = ( V3( 0.0f, ((Camera->Frust.farClip - Camera->Frust.nearClip)/(r32)cos(Camera->Frust.FOV/2.0f)) * (r32)sin(Camera->Frust.FOV/2.0f), 0.0f));
  v3 FarWidth = V3( FarHeight.y, 0.0f, 0.0f);

  v3 MaxMax = FrustLength + FarHeight + FarWidth;
  v3 MaxMin = FrustLength + FarHeight - FarWidth;
  v3 MinMax = FrustLength - FarHeight + FarWidth;
  v3 MinMin = FrustLength - FarHeight - FarWidth;

  v3 Front = V3(0,0,1);
  v3 Target = Camera->Front;

  Quaternion GrossRotation = RotatePoint(Front, Target);

  // We've got to correct the rotation so it ends pointing the frustum in the cameras 'up' direction
  v3 UpVec = V3(0, 1, 0);
  v3 RotatedUp = Rotate(UpVec, GrossRotation);
  Quaternion DesiredUp = RotatePoint(RotatedUp, Camera->Up);

  Quaternion FinalRotation = DesiredUp * GrossRotation;

  MaxMin = Rotate(MaxMin, FinalRotation);
  MaxMax = Rotate(MaxMax, FinalRotation);
  MinMin = Rotate(MinMin, FinalRotation);
  MinMax = Rotate(MinMax, FinalRotation);

  v3 CameraRenderP = GetRenderP(WorldChunkDim, Camera->CurrentP, Camera);

  plane Top(CameraRenderP,   Normalize(Cross(MaxMax, MaxMin)));
  plane Bot(CameraRenderP,   Normalize(Cross(MinMin, MinMax)));
  plane Left(CameraRenderP,  Normalize(Cross(MinMax, MaxMax)));
  plane Right(CameraRenderP, Normalize(Cross(MaxMin, MinMin)));

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

inline v2
GetMouseDelta(platform *Plat)
{
  r32 DPToWorldModifier = -0.001f;
  v2 Result = Plat->MouseDP * DPToWorldModifier;
  return Result;
}

link_internal void
UpdateGameCamera(v2 MouseDelta, input *Input, canonical_position NewTarget, camera* Camera, chunk_dimension WorldChunkDim)
{
  // NOTE(Jesse): I changed this to a pointer such that we didn't have to know
  // the size of camera structs in the debug system, but I didn't bother to
  // allocate the actual struct.  If this is crashing just allocate a camera at
  // startup.
  //
  // @allocate_camera_at_startup
  Assert(Camera);

  if (Input)
  {
    if (Input->LMB.Pressed)
    {
      Camera->Yaw += MouseDelta.x;
      Camera->Pitch += MouseDelta.y;
      Camera->Pitch = ClampBetween(0.0, Camera->Pitch, PI32);
    }

    if (Input->RMB.Pressed)
    {
      Camera->DistanceFromTarget += MouseDelta.y*200.0f;
    }
  }

  UpdateCameraP(NewTarget, Camera, WorldChunkDim);
  return;
}

