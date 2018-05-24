#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include "objloader.hpp"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide :
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

struct u32_array
{
  u32 *Elements;

  u32 At;
  u32 End;
};

struct v3_array
{
  v3 *Elements;

  u32 At;
  u32 End;
};

u32_array
U32_Array(u32 Count, memory_arena *Memory)
{
  u32 *Elements = PUSH_STRUCT_CHECKED(u32, Memory, Count );
  u32_array Result= {Elements, 0, Count + 1};

  return Result;
}
v3_array
V3_Array(u32 Count, memory_arena *Memory)
{
  v3 *Elements = PUSH_STRUCT_CHECKED(v3, Memory, Count );
  v3_array Result= {Elements, 0, Count + 1};

  return Result;
}

template <typename T, typename T_a>inline void
Push(T Vec, T_a *Array)
{
  Array->Elements[Array->At++] = Vec;
  Assert( Array->At < Array->End );
  return;
}

model
LoadObj(memory_arena *Memory, const char * FilePath)
{
  Info("Loading .obj file : %s \n", FilePath);

  FILE * ObjFile = fopen(FilePath, "r");
  if(!ObjFile)
  {
    Error("Reading ObjFile : %s: ", FilePath);
    model Result = {};
    return Result;
  }

  u32 VertCount   = 0;
  u32 UVCount     = 0;
  u32 NormalCount = 0;
  u32 FaceCount   = 0;

  while (True)
  {
    char LineType[32] = {};
    int Type = fscanf(ObjFile, "%s", LineType);
    if (Type == EOF) { break; }

    if ( strcmp( LineType, "v" ) == 0 )
    {
      ++VertCount;
    }
    else if ( strcmp( LineType, "vt" ) == 0 )
    {
      ++UVCount;
    }
    else if ( strcmp( LineType, "vn" ) == 0 )
    {
      ++NormalCount;
    }
    else if ( strcmp( LineType, "f" ) == 0 )
    {
      ++FaceCount;
    }
    else
    {
      // Probably a comment, eat up the rest of the line
      char discard[1024];
      fgets(discard, 1024, ObjFile);
    }
  }

  rewind(ObjFile);

  /* Assert(VertCount == UVCount); */
  /* Assert(VertCount == NormalCount); */

  // FIXME(Jesse): Use TranArena for these
  v3_array TempVerts       = V3_Array(VertCount, Memory);
  v3_array TempNormals     = V3_Array(NormalCount, Memory);

  u32_array VertIndicies   = U32_Array(FaceCount*3, Memory);
  u32_array NormalIndicies = U32_Array(FaceCount*3, Memory);

  while (True)
  {
    char LineType[32] = {};
    int Type = fscanf(ObjFile, "%s", LineType);
    if (Type == EOF) { break; }

    if ( strcmp( LineType, "v" ) == 0 )
    {
      v3 Vert = {};
      fscanf(ObjFile, "%f %f %f\n", &Vert.x, &Vert.y, &Vert.z );
      Push(Vert, &TempVerts);
    }
    else if ( strcmp( LineType, "vn" ) == 0 )
    {
      v3 Normal = {};
      fscanf(ObjFile, "%f %f %f\n", &Normal.x, &Normal.y, &Normal.z );
      Push(Normal, &TempNormals);
    }
#if 0
    else if ( strcmp( LineType, "vt" ) == 0 )
    {
      glm::vec2 uv;
      fscanf(ObjFile, "%f %f\n", &uv.x, &uv.y );
      uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted.
      temp_uvs.push_back(uv);
    }
#endif
    else if ( strcmp( LineType, "f" ) == 0 )
    {
      u32 vIndex[3], discard[3], nIndex[3];
      int matches = fscanf(ObjFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vIndex[0], &discard[0], &nIndex[0],
                                                                    &vIndex[1], &discard[1], &nIndex[1],
                                                                    &vIndex[2], &discard[2], &nIndex[2] );

      if (matches != 9)
      {
        Error("ObjFile can't be read");
        model Result = {};
        return Result;
      }

      Push(vIndex[0], &VertIndicies );
      Push(vIndex[1], &VertIndicies );
      Push(vIndex[2], &VertIndicies );

      Push(nIndex[0], &NormalIndicies );
      Push(nIndex[1], &NormalIndicies );
      Push(nIndex[2], &NormalIndicies );

    }
    else
    {
      // Probably a comment, eat up the rest of the line
      char discard[1000];
      fgets(discard, 1000, ObjFile);
    }

  }

  untextured_3d_geometry_buffer Mesh = {};
  AllocateMesh(&Mesh, FaceCount*3, Memory);

#if 1
  for( u32 Index = 0;
       Index < VertIndicies.At;
       ++Index )
  {
    u32 TempVertIndex = VertIndicies.Elements[Index];
    u32 TempNormalIndex = NormalIndicies.Elements[Index];

    v3 Vertex = TempVerts.Elements[TempVertIndex-1];
    v3 Normal = TempNormals.Elements[TempNormalIndex-1];

    Mesh.Verts[Mesh.At] = Vertex;
    Mesh.Normals[Mesh.At] = Normal;
    Mesh.At++;
    Assert(Mesh.At < Mesh.End);
  }
#endif

  model Result = {};
  Result.Chunk = PUSH_STRUCT_CHECKED(chunk_data, Memory, 1);;
  Result.Chunk->Mesh = Mesh;
  SetFlag(&Result, Chunk_Initialized);

  return Result;
}
