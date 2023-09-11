link_internal work_queue_entry_sim_particle_system
WorkQueueEntrySimParticleSystem( particle_system System , untextured_3d_geometry_buffer Dest , v3 EntityDelta , v3 RenderSpaceP , r32 Dt  )
{
  work_queue_entry_sim_particle_system Reuslt = {
    .System = System,
    .Dest = Dest,
    .EntityDelta = EntityDelta,
    .RenderSpaceP = RenderSpaceP,
    .dt = Dt
  };
  return Reuslt;
}

