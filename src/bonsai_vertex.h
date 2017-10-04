#ifndef BONSAI_VERTEX_H
#define BONSAI_VERTEX_H

inline void
RightFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  r32 Temp[] = {
    MinP.x + Diameter.x , MinP.y +  Diameter.y , MinP.z +  Diameter.z ,
    MinP.x + Diameter.x , MinP.y               , MinP.z               ,
    MinP.x + Diameter.x , MinP.y +  Diameter.y , MinP.z               ,
    MinP.x + Diameter.x , MinP.y               , MinP.z               ,
    MinP.x + Diameter.x , MinP.y +  Diameter.y , MinP.z +  Diameter.z ,
    MinP.x + Diameter.x , MinP.y               , MinP.z +  Diameter.z ,
  };

  memcpy(Result, Temp, sizeof(Temp));

  return;
}

v3 RightFaceNormalData[] =
{
  {1, 0, 0},
  {1, 0, 0},
  {1, 0, 0},
  {1, 0, 0},
  {1, 0, 0},
  {1, 0, 0}
};

inline void
LeftFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  r32 Temp[] = {
    MinP.x , MinP.y +  Diameter.y , MinP.z +  Diameter.z ,
    MinP.x , MinP.y               , MinP.z               ,
    MinP.x , MinP.y +  Diameter.y , MinP.z               ,
    MinP.x , MinP.y               , MinP.z               ,
    MinP.x , MinP.y +  Diameter.y , MinP.z +  Diameter.z ,
    MinP.x , MinP.y               , MinP.z +  Diameter.z ,
  };

  memcpy(Result, Temp, sizeof(Temp));

  return;
}

v3 LeftFaceNormalData[] =
{
  {-1, 0, 0},
  {-1, 0, 0},
  {-1, 0, 0},
  {-1, 0, 0},
  {-1, 0, 0},
  {-1, 0, 0},
};

inline void
BottomFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  r32 Temp[] = {
    MinP.x + Diameter.x , MinP.y , MinP.z +  Diameter.z ,
    MinP.x              , MinP.y , MinP.z               ,
    MinP.x + Diameter.x , MinP.y , MinP.z               ,
    MinP.x              , MinP.y , MinP.z               ,
    MinP.x + Diameter .x, MinP.y , MinP.z +  Diameter.z ,
    MinP.x              , MinP.y , MinP.z +  Diameter.z ,
  };

  memcpy(Result, Temp, sizeof(Temp));

  return;
}

v3 BottomFaceNormalData[] =
{
  {0,-1, 0},
  {0,-1, 0},
  {0,-1, 0},
  {0,-1, 0},
  {0,-1, 0},
  {0,-1, 0},
};

inline void
TopFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  r32 Temp[] = {
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z + Diameter.z ,
    MinP.x              , MinP.y + Diameter.y , MinP.z              ,
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z              ,
    MinP.x              , MinP.y + Diameter.y , MinP.z              ,
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z + Diameter.z ,
    MinP.x              , MinP.y + Diameter.y , MinP.z + Diameter.z ,
  };

  memcpy(Result, Temp, sizeof(Temp));

  return;
}

v3 TopFaceNormalData[] =
{
  {0, 1, 0},
  {0, 1, 0},
  {0, 1, 0},
  {0, 1, 0},
  {0, 1, 0},
  {0, 1, 0},
};

inline void
FrontFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  r32 Temp[] = {
    MinP.x +  Diameter.x , MinP.y +  Diameter.y , MinP.z + Diameter.z ,
    MinP.x               , MinP.y               , MinP.z + Diameter.z ,
    MinP.x +  Diameter.x , MinP.y               , MinP.z + Diameter.z ,
    MinP.x               , MinP.y               , MinP.z + Diameter.z ,
    MinP.x +  Diameter.x , MinP.y +  Diameter.y , MinP.z + Diameter.z ,
    MinP.x               , MinP.y +  Diameter.y , MinP.z + Diameter.z ,
  };

  memcpy(Result, Temp, sizeof(Temp));

  return;
}

v3 FrontFaceNormalData[] =
{
  {0, 0, 1},
  {0, 0, 1},
  {0, 0, 1},

  {0, 0, 1},
  {0, 0, 1},
  {0, 0, 1},
};

inline void
BackFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  r32 Temp[] = {
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z ,
    MinP.x              , MinP.y              , MinP.z ,
    MinP.x + Diameter.x , MinP.y              , MinP.z ,
    MinP.x              , MinP.y              , MinP.z ,
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z ,
    MinP.x              , MinP.y + Diameter.y , MinP.z ,
  };

  memcpy(Result, Temp, sizeof(Temp));

  return;
}

v3 BackFaceNormalData[] =
{
  {0, 0, -1},
  {0, 0, -1},
  {0, 0, -1},

  {0, 0, -1},
  {0, 0, -1},
  {0, 0, -1},
};


#endif
