
struct keyframe
{
  r32 StartTime;
  r32 EndTime;
  v3 PositionInterp;
};

struct animation
{
  r32 Duration;
  u32 KeyframeCount;
  keyframe *Keyframes;
};

