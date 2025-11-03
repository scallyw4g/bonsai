struct plane
{
  v3 Normal;
  f32 DistanceToOrigin;
};

link_internal plane
Plane( v3 Point, v3 Normal )
{
  plane Reuslt = { Normal, Dot(Normal, Point) };
  return Reuslt;
}

struct frustum
{
  f32 farClip;
  f32 nearClip;
  f32 FOV;

  plane Top;
  plane Bottom;
  plane Left;
  plane Right;
};

struct entity;

struct camera poof(@version(2))
{
  frustum Frust;

  cp CurrentP;

  v3 RenderSpacePosition;

  r32 Pitch;              poof(@no_serialize)
  r32 Roll;               poof(@no_serialize)
  r32 Yaw;                poof(@no_serialize)
  r32 DistanceFromTarget; poof(@no_serialize)

  r32 TargetPitch;
  r32 TargetRoll;
  r32 TargetYaw;
  r32 TargetDistanceFromTarget;

  r32 Blend; // How fast the camera interpolates.  0 is instant, 0.1f is verrry slow
  r32 Speed = 3.0f; // How fast the camera ghost moves

  v3 Front;
  v3 Right;
  v3 Up;

  entity_id GhostId;

  m4 ViewProjection;
  m4 InverseViewMatrix;
  m4 InverseProjectionMatrix;
};

struct camera_1
{
  frustum Frust;

  cp CurrentP;

  v3 RenderSpacePosition;

  r32 Pitch;
  r32 Roll;
  r32 Yaw;

  r32 TargetPitch;
  r32 TargetRoll;
  r32 TargetYaw;

  r32 Blend; // How fast the camera interpolates.  0 is instant, 0.1f is verrry slow
  r32 DistanceFromTarget;

  v3 Front;
  v3 Right;
  v3 Up;

  entity_id GhostId;
};

struct camera_0
{
  frustum Frust;

  cp CurrentP;

  v3 RenderSpacePosition;

  r32 Pitch;
  r32 Roll;
  r32 Yaw;

  r32 DistanceFromTarget;

  v3 Front;
  v3 Right;
  v3 Up;

  entity_id GhostId;
};

#define DEFAULT_CAMERA_BLENDING (18.f)

inline v3
GetCameraRelativeInput(hotkeys *Hotkeys, camera *Camera)
{
  v3 Right = -1.0f*Camera->Right;
  v3 Forward = Camera->Front;

  v3 UpdateDir = V3(0,0,0);

  if ( Hotkeys->Forward )
    UpdateDir += Forward;

  if ( Hotkeys->Backward )
    UpdateDir -= Forward;

  if ( Hotkeys->Right )
    UpdateDir += Right;

  if ( Hotkeys->Left )
    UpdateDir -= Right;

  UpdateDir = Normalize(UpdateDir, Length(UpdateDir));

  return UpdateDir;
}

inline r32
DistanceToPlane(plane *Plane, v3 P)
{
  // https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
  r32 Result = Dot(Plane->Normal, P) - Plane->DistanceToOrigin;
#if 0
  r32 x = Plane->P.x;
  r32 y = Plane->P.y;
  r32 z = Plane->P.z;

  r32 a = Plane->Normal.x;
  r32 b = Plane->Normal.y;
  r32 c = Plane->Normal.z;

  r32 d = Plane->d;
  Assert(a*x + b*y + c*z + d == 0);

  r32 Result = a*P.x + b*P.y + c*P.z + d;
#endif
  return Result;
}

link_internal v3
GetFrustumCenter(camera *Camera)
{
  v3 Result = {};
  // TODO(Jesse): Implement me!
  return Result;
}

link_inline v2
GetMouseDelta(platform *Plat);

link_internal void
StandardCamera(camera* Camera, f32 FarClip, f32 DistanceFromTarget, f32 Blend = DEFAULT_CAMERA_BLENDING);

