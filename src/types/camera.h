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

  canonical_position ViewingTarget; // TODO : Can this just be a v3?

  r32 Pitch;
  r32 Roll;
  r32 Yaw;

  r32 DistanceFromTarget;

  v3 Front;
  v3 Right;
  v3 Up;
};

