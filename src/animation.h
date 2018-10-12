
struct keyframe
{
  r32 tStart;
  r32 tEnd;
  v3 PositionInterp;
};

struct animation
{
  r64 t;
  r64 tEnd;
  u32 KeyframeCount;
  keyframe *Keyframes;
};


v3
GetInterpolatedPosition(animation* Animation)
{

  if (Animation->t >= Animation->tEnd)
    { Animation->t -= Animation->tEnd; }

  v3 LastP = {};
  v3 P = {};

  for (u32 KeyframeIndex = 0;
      KeyframeIndex < Animation->KeyframeCount;
      ++KeyframeIndex)
  {
    keyframe* Keyframe = Animation->Keyframes + KeyframeIndex;
    if (Animation->t >= Keyframe->tStart  && Animation->t <= Keyframe->tEnd)
    {
      r32 t01 = Animation->t / Animation->tEnd;
      P = Lerp(t01, LastP, Keyframe->PositionInterp);
      break;
    }
    else
    {
      LastP = Keyframe->PositionInterp;
    }
  }

  return P;
}
