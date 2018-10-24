
struct keyframe
{
  r32 tEnd;
  r32 Value;
};

struct animation
{
  r64 t;
  r64 tEnd;

  u32 xKeyframeCount;
  keyframe* xKeyframes;

  u32 yKeyframeCount;
  keyframe* yKeyframes;

  u32 zKeyframeCount;
  keyframe* zKeyframes;
};


r32
Interpolate(animation* Animation, keyframe* Keyframes, u32 KeyframeCount)
{
  r32 PrevKeyframeValue = Keyframes[KeyframeCount-1].Value;
  r32 P = {};
  r32 tStart = {};

  for (u32 KeyframeIndex = 0;
      KeyframeIndex < KeyframeCount;
      ++KeyframeIndex)
  {
    keyframe* Keyframe = Keyframes + KeyframeIndex;
    if (Animation->t >= tStart  && Animation->t <= Keyframe->tEnd)
    {
      r32 tAt = Animation->t - tStart;
      r32 t01 = SafeDivide0(tAt, (Keyframe->tEnd - tStart));
      P = Lerp(t01, PrevKeyframeValue, Keyframe->Value);
      break;
    }
    else
    {
      PrevKeyframeValue = Keyframe->Value;
      tStart = Keyframe->tEnd;
    }
  }

  return P;
}

v3
GetInterpolatedPosition(animation* Animation)
{
  if (Animation->t >= Animation->tEnd) { Animation->t -= Animation->tEnd; }

  r32 xP = Interpolate(Animation, Animation->xKeyframes, Animation->xKeyframeCount);
  r32 yP = Interpolate(Animation, Animation->yKeyframes, Animation->yKeyframeCount);
  r32 zP = Interpolate(Animation, Animation->zKeyframes, Animation->zKeyframeCount);

  v3 Result = V3(xP, yP, zP);
  return Result;
}
