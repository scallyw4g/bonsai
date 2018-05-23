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

struct v3_array
{
  v3 *Elements;

  u32 At;
  u32 End;
};

v3_array
V3_Array(u32 Count, memory_arena *Memory)
{
  v3 *Elements = PUSH_STRUCT_CHECKED(v3, Memory, Count );
  v3_array Result= {Elements, 0, Count + 1};

  return Result;
}

inline void
Push(v3 Vec, v3_array *Array)
{
  Array->Elements[Array->At++] = Vec;
  Assert( Array->At < Array->End );
  return;
}

bool
LoadObj( const char * FilePath, memory_arena *Memory)
{
  Info("Loading .obj file : %s \n", FilePath);

  FILE * ObjFile = fopen(FilePath, "r");
  if(!ObjFile)
  {
    Error("Reading ObjFile : %s: ", FilePath);
    return false;
  }

  u32 VertCount   = 0;
  u32 UVCount     = 0;
  u32 NormalCount = 0;

  while(True)
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
      VertCount   += 3;
      UVCount     += 3;
      NormalCount += 3;
    }
    else
    {
      // Probably a comment, eat up the rest of the line
      char discard[1024];
      fgets(discard, 1024, ObjFile);
    }
  }

  /* Assert(VertCount == UVCount); */
  Assert(VertCount == NormalCount);

  v3_array TempVerts   = V3_Array(VertCount, Memory);
  v3_array TempNormals = V3_Array(NormalCount, Memory);


  while( True )
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
    else if ( strcmp( LineType, "f" ) == 0 )
    {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
      int matches = fscanf(ObjFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
      if (matches != 9)
      {
        Log("ObjFile can't be read by our simple parser :-( Try exporting with other options\n");
        return false;
      }
      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      uvIndices    .push_back(uvIndex[0]);
      uvIndices    .push_back(uvIndex[1]);
      uvIndices    .push_back(uvIndex[2]);
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);
    }
#endif
    else
    {
      // Probably a comment, eat up the rest of the line
      char discard[1000];
      fgets(discard, 1000, ObjFile);
    }

  }

#if 0
  -- Add back in when we do VAOs with indices --

  // For each vertex of each triangle
  for( u32 VertIndex  = 0; VertIndex < vertexIndices.size(); ++VertIndex )
  {

    // Get the indices of its attributes
    unsigned int vertexIndex = vertexIndices[VertIndex];
    unsigned int uvIndex = uvIndices[VertIndex];
    unsigned int normalIndex = normalIndices[VertIndex];

    // Get the attributes thanks to the index
    glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
    glm::vec2 uv = temp_uvs[ uvIndex-1 ];
    glm::vec3 normal = temp_normals[ normalIndex-1 ];

    // Put the attributes in buffers
    out_vertices.push_back(vertex);
    out_uvs     .push_back(uv);
    out_normals .push_back(normal);

  }
#endif

  return true;
}
