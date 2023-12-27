link_internal counted_string
ToString(entity_behavior_flags Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case EntityBehaviorFlags_None: { Result = CSz("EntityBehaviorFlags_None"); } break;
    case EntityBehaviorFlags_Gravity: { Result = CSz("EntityBehaviorFlags_Gravity"); } break;
    case EntityBehaviorFlags_WorldCollision: { Result = CSz("EntityBehaviorFlags_WorldCollision"); } break;
    case EntityBehaviorFlags_EntityCollision: { Result = CSz("EntityBehaviorFlags_EntityCollision"); } break;
    case EntityBehaviorFlags_UnspawnOnParticleSystemTerminate: { Result = CSz("EntityBehaviorFlags_UnspawnOnParticleSystemTerminate"); } break;
    case EntityBehaviorFlags_CameraGhost: { Result = CSz("EntityBehaviorFlags_CameraGhost"); } break;
    case EntityBehaviorFlags_Default: { Result = CSz("EntityBehaviorFlags_Default"); } break;
  }
  return Result;
}

