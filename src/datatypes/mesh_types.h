struct loaded_collada_mesh
{
  chunk_data* ChunkData;
  v3 Dim;
};

template <typename T> b32
BufferIsFull(T *Buffer, u32 VertsToPush)
{
  b32 Result = (Buffer->At + VertsToPush) > Buffer->End;
  return Result;
}

