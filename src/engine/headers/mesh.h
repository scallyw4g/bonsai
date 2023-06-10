struct loaded_collada_mesh
{
  untextured_3d_geometry_buffer Mesh;
  v3 Dim;
};

template <typename T> b32
BufferHasRoomFor(T *Buffer, u32 VertsToPush)
{
  b32 Result = (Buffer->At + VertsToPush) <= Buffer->End;
  return Result;
}

struct free_mesh
{
  untextured_3d_geometry_buffer* Mesh;
  volatile free_mesh* Next;
};

struct mesh_freelist
{
#if BONSAI_INTERNAL
  bonsai_futex DebugFutex;
#endif

  volatile free_mesh* FirstFree;
  volatile free_mesh* Containers;
};

