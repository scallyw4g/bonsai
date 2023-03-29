
global_variable r32 GameUnitsToMeters = 6.f;

// TODO(Jesse): Make this per-game
global_variable v3
Global_WorldGravity = V3(0.0f, 0.0f, -9.8f) * GameUnitsToMeters;

inline void
PhysicsUpdate(physics *Physics, r32 dt, b32 ApplyGravity = True)
{
  if (Physics->Speed > 0.f)
  {
    v3 Gravity = ApplyGravity ? Global_WorldGravity : V3(0);

    v3 Acceleration = SafeDivide(Physics->Force*Physics->Speed, Physics->Mass);

    Physics->Velocity += (Acceleration) + (Gravity * dt);

    v3 Drag = Physics->Mass * Physics->Velocity * V3(0.15f, 0.15f, 0.f) * dt;
    Physics->Velocity -= Drag;

    Physics->Delta = Physics->Velocity * dt;

    Physics->Force = V3(0);
  }
}

