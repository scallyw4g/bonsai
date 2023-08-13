link_internal entity *
MousePickEntity(engine_resources *Resources)
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  entity *Result = {};

  maybe_ray MaybeRay = ComputeRayFromCursor(Plat, &gBuffer->ViewProjection, Camera, World->ChunkDim);

  if (MaybeRay.Tag == Maybe_Yes)
  {

    v3 SimOrigin = GetSimSpaceP(World, Canonical_Position(World->ChunkDim, MaybeRay.Ray.Origin, V3i(0)));
    ray SimRay = {SimOrigin, MaybeRay.Ray.Dir};
    Result = RayTraceEntityCollision( Resources, &SimRay );
  }

  return Result;
}


