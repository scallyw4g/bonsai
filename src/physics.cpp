

inline void
PhysicsUpdate(physics *Physics, r32 dt)
{
  v3 Acceleration = SafeDivide(Physics->Force*Physics->Speed, Physics->Mass) + V3(0.0f,0.0f, -98.0f);

  Physics->Force -=
    Physics->Force*Physics->Drag*dt;

  Physics->Velocity -=
    Physics->Velocity*Physics->Drag*dt;

  Physics->Delta =
    (Physics->Velocity*dt) + (0.5f*Acceleration*Square(dt));

  Physics->Velocity = SafeDivide(Physics->Delta, dt);

  return;
}

