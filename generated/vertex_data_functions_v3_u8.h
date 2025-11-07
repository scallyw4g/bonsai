// callsite
// src/engine/voxel_face.h:216:0

// def (vertex_data_functions)
// src/engine/voxel_face.h:72:0
inline void
RightFaceVertexData( v3_u8 MinP, v3 Diameter, v3_u8 *Result)
{
  v3_u8 MaxP = MinP + Diameter;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  0    1
  //
  //  2    3

  v3_u8 P0 = {{ MaxP.x, MinP.y, MaxP.z }};
  v3_u8 P1 = {{ MaxP.x, MaxP.y, MaxP.z }};
  v3_u8 P2 = {{ MaxP.x, MinP.y, MinP.z }};
  v3_u8 P3 = {{ MaxP.x, MaxP.y, MinP.z }};

  v3_u8 Temp[] = { P2, P1, P0, P2, P3, P1 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));
}

inline void
LeftFaceVertexData( v3_u8 MinP, v3 Diameter, v3_u8 *Result)
{
  v3_u8 MaxP = MinP + Diameter;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  0    1
  //
  //  2    3

  v3_u8 P0 = {{ MinP.x, MaxP.y, MaxP.z }};
  v3_u8 P1 = {{ MinP.x, MinP.y, MaxP.z }};
  v3_u8 P2 = {{ MinP.x, MaxP.y, MinP.z }};
  v3_u8 P3 = {{ MinP.x, MinP.y, MinP.z }};

  v3_u8 Temp[] = { P2, P1, P0, P2, P3, P1 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

  return;
}

inline void
BackFaceVertexData( v3_u8 MinP, v3 Diameter, v3_u8 *Result)
{
  v3_u8 MaxP = MinP + Diameter;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  0    3
  //
  //  1    2

  v3_u8 P0 = {{ MinP.x, MinP.y, MaxP.z }};
  v3_u8 P1 = {{ MaxP.x, MinP.y, MaxP.z }};
  v3_u8 P2 = {{ MinP.x, MinP.y, MinP.z }};
  v3_u8 P3 = {{ MaxP.x, MinP.y, MinP.z }};

  v3_u8 Temp[] = { P0, P2, P1, P1, P2, P3 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

  return;
}

inline void
FrontFaceVertexData( v3_u8 MinP, v3 Diameter, v3_u8 *Result)
{
  v3_u8 MaxP = MinP + Diameter;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  7    4
  //
  //  6    5

  v3_u8 P7 = {{ MaxP.x, MaxP.y, MaxP.z }};
  v3_u8 P4 = {{ MinP.x, MaxP.y, MaxP.z }};
  v3_u8 P6 = {{ MaxP.x, MaxP.y, MinP.z }};
  v3_u8 P5 = {{ MinP.x, MaxP.y, MinP.z }};

  v3_u8 Temp[] = { P7, P6, P4, P4, P6, P5 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));


  return;
}

inline void
TopFaceVertexData( v3_u8 MinP, v3 Diameter, v3_u8 *Result)
{
  v3_u8 MaxP = MinP + Diameter;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  4    7
  //
  //  0    3

  v3_u8 P4 = {{ MinP.x, MaxP.y, MaxP.z }};
  v3_u8 P7 = {{ MaxP.x, MaxP.y, MaxP.z }};
  v3_u8 P0 = {{ MinP.x, MinP.y, MaxP.z }};
  v3_u8 P3 = {{ MaxP.x, MinP.y, MaxP.z }};

  v3_u8 Temp[] = { P4, P0, P7, P7, P0, P3 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

  return;
}

inline void
BottomFaceVertexData( v3_u8 MinP, v3 Diameter, v3_u8 *Result)
{
  /* TIMED_FUNCTION(); */
  v3_u8 MaxP = MinP + Diameter;

  /* Round(MaxP); */
  /* Round(MinP); */

  v3_u8 Temp[] = {
    {{MaxP.x , MinP.y , MinP.z}},
    {{MinP.x , MinP.y , MinP.z}},
    {{MaxP.x , MaxP.y , MinP.z}},
    {{MinP.x , MaxP.y , MinP.z}},
    {{MaxP.x , MaxP.y , MinP.z}},
    {{MinP.x , MinP.y , MinP.z}},
  };

  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

  return;
}

