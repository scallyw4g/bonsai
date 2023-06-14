struct plane
{
  v3 P;
  v3 Normal;
  r32 d;

  plane( v3 P_in, v3 Normal_in )
  {
    this->P = P_in;
    this->Normal = Normal_in;

    this->d = -1.0f * (Normal.x*P.x + Normal.y*P.y + Normal.z*P.z);

    Assert(Normal.x*P.x + Normal.y*P.y + Normal.z*P.z + this->d == 0);
  }

  plane() {}
};

struct frustum
{
  float farClip;
  float nearClip;
  float width;
  float FOV;

  plane Top;
  plane Bot;
  plane Left;
  plane Right;
};

struct camera
{
  frustum Frust;

  canonical_position TargetP;
  canonical_position CurrentP;
  canonical_position ViewingTarget; // TODO(Jesse, id: 79, tags: over_allocation, ummmm) : Can this just be a v3?

  v3 RenderSpacePosition;

  r32 Pitch;
  r32 Roll;
  r32 Yaw;

  r32 DistanceFromTarget;

  v3 Front;
  v3 Right;
  v3 Up;
};

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
  r32 x = Plane->P.x;
  r32 y = Plane->P.y;
  r32 z = Plane->P.z;

  r32 a = Plane->Normal.x;
  r32 b = Plane->Normal.y;
  r32 c = Plane->Normal.z;

  r32 d = Plane->d;
  Assert(a*x + b*y + c*z + d == 0);

  r32 Distance = a*P.x + b*P.y + c*P.z + d;
  return Distance;
}


