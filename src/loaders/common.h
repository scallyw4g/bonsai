struct u32_static_array
{
  u32 *Elements;

  umm At;
  umm End;
};

struct v3_static_array
{
  v3 *Elements;

  umm At;
  umm End;
};

u32_static_array
U32_Static_Array(u32 Count, memory_arena *Memory)
{
  u32 *Elements = Allocate(u32, Memory, Count );
  u32_static_array Result= {Elements, 0, Count};

  return Result;
}

v3_static_array
V3_Static_Array(u32 Count, memory_arena *Memory)
{
  v3 *Elements = Allocate(v3, Memory, Count );
  v3_static_array Result= {Elements, 0, Count};

  return Result;
}

template <typename T, typename T_a>inline void
Push(T Vec, T_a *Array)
{
  Assert( Array->At < Array->End );
  Array->Elements[Array->At++] = Vec;
  return;
}

struct obj_stats
{
  u32 VertCount;
  u32 NormalCount;
  u32 UVCount;
  u32 FaceCount;
};

obj_stats
GetObjStats(ansi_stream Cursor, memory_arena *Memory)
{
  obj_stats Result = {};

  while (Cursor.At < Cursor.End)
  {
    char *LineType = PopWord(&Cursor, Memory);
    if (LineType == 0) { break; }

    if ( StringsMatch(LineType, "v"))
    {
      ++Result.VertCount;
    }
    else if ( StringsMatch(LineType, "vt") )
    {
      ++Result.UVCount;
    }
    else if ( StringsMatch(LineType, "vn") )
    {
      ++Result.NormalCount;
    }
    else if ( StringsMatch(LineType, "f") )
    {
      ++Result.FaceCount;
    }
    else
    {
      // Irrelevant.
    }
  }

  return Result;
}
