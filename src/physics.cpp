

inline v3
PhysicsUpdate(physics *Physics, r32 dt, b32 Print = False)
{
  v3 Acceleration = SafeDivide(Physics->Force*Physics->Speed, Physics->Mass);

  Physics->Force -=
    Physics->Force*Physics->Drag*dt;

  Physics->Velocity -=
    Physics->Velocity*Physics->Drag*dt;

  v3 Delta =
    (Physics->Velocity*dt) + (0.5f*Acceleration*Square(dt));

  Physics->Velocity = (Delta/dt);

  return Delta;
}

