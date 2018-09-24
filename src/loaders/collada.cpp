mesh_metadata
GetColladaMetadata(ansi_stream Stream, memory_arena *TranArena)
{
  mesh_metadata Result = {};
  return Result;
}


/*
 * This loader doesn't support ngon faces.  The mesh must be triangulated
 * before exporting from blender.
 *
 * Note that triangulating a mesh with bent normals at export time (in blender)
 * seems to not re-bend the triangulated normals and everything looks borked.
 */
model
LoadCollada(memory_arena *PermMem, const char * FilePath)
{
  Info("Loading .dae file : %s \n", FilePath);

  umm Length = 0;
  binary_stream_u8 BinaryStream = BinaryStreamFromFile(FilePath, PermMem);
  if (!BinaryStream.Start) { model M = {}; return M; }

  ansi_stream Stream = AnsiStream(&BinaryStream);
  mesh_metadata Stats = GetColladaMetadata(Stream, TranArena);



  model Result = {};
  return Result;
}
