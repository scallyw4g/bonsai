
#if PLATFORM_GL_IMPLEMENTATIONS

link_internal void
Bonsai_SimulateEntitiesAndWorld(engine_resources *Resources)
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

#if 0 // DEBUG_SYSTEM_API
  if (GetDebugState()->UiGroup.PressedInteractionId != StringHash("GameViewport"))
  {
    GameInput = 0;
  }
#endif

  SimulateEntities(Resources, Plat->dt, World->VisibleRegion, &GpuMap->Buffer, &Plat->HighPriority);
  /* DispatchSimulateParticleSystemJobs(&Plat->HighPriority, EntityTable, World->ChunkDim, &GpuMap->Buffer, Graphics, Plat->dt); */
}

#endif
