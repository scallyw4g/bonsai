
#if PLATFORM_GL_IMPLEMENTATIONS

link_internal void
Bonsai_SimulateEntitiesAndWorld(engine_resources *Resources)
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  v2 MouseDelta = GetMouseDelta(Plat);
  input* GameInput = &Plat->Input;

#if 0 // DEBUG_SYSTEM_API
  if (GetDebugState()->UiGroup.PressedInteractionId != StringHash("GameViewport"))
  {
    GameInput = 0;
  }
#endif

  auto TargetP = Resources->CameraTargetP ? *Resources->CameraTargetP : Canonical_Position(0);
  UpdateGameCamera(MouseDelta, GameInput, TargetP, Camera, World->ChunkDim);
  SimulateEntities(Resources, Plat->dt, World->VisibleRegion, &GpuMap->Buffer, &Plat->HighPriority);
  /* DispatchSimulateParticleSystemJobs(&Plat->HighPriority, EntityTable, World->ChunkDim, &GpuMap->Buffer, Graphics, Plat->dt); */
}

#endif
