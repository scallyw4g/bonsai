
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

  SimulateEntities(World, EntityTable, Plat->dt, World->VisibleRegion, Camera, Hotkeys);
  auto TargetP = Resources->CameraTargetP ? *Resources->CameraTargetP : Canonical_Position(0);
  UpdateGameCamera(MouseDelta, GameInput, TargetP, Camera, World->ChunkDim);
  SimulateAndRenderParticleSystems(EntityTable, World->ChunkDim, &GpuMap->Buffer, Graphics, Plat->dt);
}

#endif
