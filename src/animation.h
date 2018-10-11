
struct keyframe
{
  r32 tEnd;
  v3 PositionInterp;
};

struct animation
{
  r32 t;
  u32 KeyframeCount;
  keyframe *Keyframes;
};

