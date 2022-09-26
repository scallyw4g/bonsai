
// TODO(Jesse): Make this per-game
global_variable v3 Global_WorldGravity = V3(0.0f, 0.0f, -10.f);

inline void
PhysicsUpdate(physics *Physics, r32 dt, b32 ApplyGravity = True)
{
  v3 Gravity = ApplyGravity ? Global_WorldGravity : V3(0);

  /* v3 Acceleration = SafeDivide(1.f*Physics->Force*Physics->Speed, Physics->Mass) + (Gravity); */

/*   Physics->Velocity += (Acceleration * dt); */

  Physics->Velocity += (Gravity * dt);

  Physics->Delta = Physics->Velocity * dt;

/*   Physics->Force = Physics->Force - (Physics->Force*dt); */
  Physics->Force = V3(0);
}

