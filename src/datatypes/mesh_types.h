struct loaded_collada_mesh
{
  untextured_3d_geometry_buffer Mesh;
  v3 Dim;
};

template <typename T> b32
BufferIsFull(T *Buffer, u32 VertsToPush)
{
  b32 Result = (Buffer->At + VertsToPush) > Buffer->End;
  return Result;
}

