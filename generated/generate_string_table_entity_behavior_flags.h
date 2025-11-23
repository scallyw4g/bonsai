// callsite
// src/engine/entity.h:39:0

// def (generate_string_table)
// external/bonsai_stdlib/src/poof_functions.h:1821:0
link_internal b32
IsValid(entity_behavior_flags Value)
{
  b32 Result = False;
  switch (Value)
  {
        case EntityBehaviorFlags_None:
    case EntityBehaviorFlags_Gravity:
    case EntityBehaviorFlags_WorldCollision:
    case EntityBehaviorFlags_EntityCollision:
    case EntityBehaviorFlags_UnspawnOnParticleSystemTerminate:
    case EntityBehaviorFlags_FitCollisionVolumeToModel:
    case EntityBehaviorFlags_DefatulCameraGhostBehavior:
    case EntityBehaviorFlags_WorldCenter:
    case EntityBehaviorFlags_Default:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(entity_behavior_flags Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case EntityBehaviorFlags_None: { Result = CSz("None"); } break;
      case EntityBehaviorFlags_Gravity: { Result = CSz("Gravity"); } break;
      case EntityBehaviorFlags_WorldCollision: { Result = CSz("WorldCollision"); } break;
      case EntityBehaviorFlags_EntityCollision: { Result = CSz("EntityCollision"); } break;
      case EntityBehaviorFlags_UnspawnOnParticleSystemTerminate: { Result = CSz("UnspawnOnParticleSystemTerminate"); } break;
      case EntityBehaviorFlags_FitCollisionVolumeToModel: { Result = CSz("FitCollisionVolumeToModel"); } break;
      case EntityBehaviorFlags_DefatulCameraGhostBehavior: { Result = CSz("DefatulCameraGhostBehavior"); } break;
      case EntityBehaviorFlags_WorldCenter: { Result = CSz("WorldCenter"); } break;
      case EntityBehaviorFlags_Default: { Result = CSz("Default"); } break;


            // TODO(Jesse): This is pretty barf and we could do it in a single allocation,
      // but the metaprogram might have to be a bit fancier..
      default:
      {
        u32 CurrentFlags = u32(Type);

        u32 BitsSet = CountBitsSet_Kernighan(CurrentFlags);
        switch(BitsSet)
        {
          case 0: // We likely passed 0 into this function, and the enum didn't have a 0 value
          case 1: // The value we passed in was outside the range of the valid enum values
          {
            Result = FSz("(invalid value (%d))", CurrentFlags);
          } break;

          default:
          {
            u32 FirstValue = UnsetLeastSignificantSetBit(&CurrentFlags);
            Result = ToStringPrefixless(entity_behavior_flags(FirstValue));

            while (CurrentFlags)
            {
              u32 Value = UnsetLeastSignificantSetBit(&CurrentFlags);
              cs Next = ToStringPrefixless(entity_behavior_flags(Value));
              Result = FSz("%S | %S", Result, Next);
            }
          } break;
        }
      } break;

    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(entity_behavior_flags Type)
{
  Assert(IsValid(Type));

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
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

