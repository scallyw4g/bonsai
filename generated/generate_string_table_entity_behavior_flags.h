// src/engine/entity.h:39:0

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
    case EntityBehaviorFlags_FitCollisionVolumeToModel: { Result = CSz("EntityBehaviorFlags_FitCollisionVolumeToModel"); } break;
    case EntityBehaviorFlags_DefatulCameraGhostBehavior: { Result = CSz("EntityBehaviorFlags_DefatulCameraGhostBehavior"); } break;
    case EntityBehaviorFlags_WorldCenter: { Result = CSz("EntityBehaviorFlags_WorldCenter"); } break;
    case EntityBehaviorFlags_Default: { Result = CSz("EntityBehaviorFlags_Default"); } break;

    // TODO(Jesse): This is pretty barf and we could do it in a single allocation,
    // but the metaprogram might have to be a bit fancier..
    default:
    {
      u32 CurrentFlags = u32(Type);

      u32 FirstValue = UnsetLeastSignificantSetBit(&CurrentFlags);
      Result = ToString(entity_behavior_flags(FirstValue));

      while (CurrentFlags)
      {
        u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
        cs Next = ToString(entity_behavior_flags(Value));
        Result = FSz("%S | %S", Result, Next);
      }
    } break;

  }
  return Result;
}

