

inline void
PhysicsUpdate(physics *Physics, r32 dt, b32 ApplyGravity = True)
{
  v3 Gravity = ApplyGravity ? V3(0.0f,0.0f, -1500.0f) : V3(0);
  v3 Acceleration = SafeDivide(Physics->Force*Physics->Speed, Physics->Mass) + Gravity;

  Physics->Force -=
    Physics->Force*Physics->Drag*dt;

  Physics->Velocity -=
    Physics->Velocity*Physics->Drag*dt;

  Physics->Delta =
    (Physics->Velocity*dt) + (0.5f*Acceleration*Square(dt));

  Physics->Velocity = SafeDivide(Physics->Delta, dt);

  return;
}

