// callsite
// external/bonsai_stdlib/src/gpu_mapped_buffer.h:122:0

// def (triple_buffered)
// external/bonsai_stdlib/src/gpu_mapped_buffer.h:2:0


struct triple_buffered_gpu_mapped_ui_buffer
{
  u32 CurrentIndex;
  gpu_element_buffer_handles  Handles[3];
  ui_geometry_buffer  Buffer;
};

link_internal gpu_element_buffer_handles *
CurrentHandles( triple_buffered_gpu_mapped_ui_buffer *Buf )
{
  Assert(Buf->CurrentIndex < 3);
  gpu_element_buffer_handles *Result = Buf->Handles + Buf->CurrentIndex;
  return Result;
}

link_internal void               
MapGpuBuffer( triple_buffered_gpu_mapped_ui_buffer *Buf )
{
  Buf->CurrentIndex = (Buf->CurrentIndex + 1) % 3;
  Buf->Buffer = MapGpuBuffer_gpu_mapped_ui_buffer(CurrentHandles(Buf)).Buffer;
}

link_internal void
DrawBuffer( triple_buffered_gpu_mapped_ui_buffer *Buf, v2 *ScreenDim )
{
  auto Handles = CurrentHandles(Buf);
  // TODO(Jesse): Probably smooth this out ..
  gpu_mapped_ui_buffer SyntheticBuffer = {
    *Handles,
    Buf->Buffer,
  };

  DrawBuffer(&SyntheticBuffer, ScreenDim);

  *Handles  = SyntheticBuffer.Handles;
}

link_internal void
AllocateGpuBuffer( triple_buffered_gpu_mapped_ui_buffer *Buf, data_type Type, u32 ElementCount )
{
  AllocateGpuBuffer_gpu_mapped_ui_buffer(Buf->Handles+0, Type, ElementCount);
  AllocateGpuBuffer_gpu_mapped_ui_buffer(Buf->Handles+1, Type, ElementCount);
  AllocateGpuBuffer_gpu_mapped_ui_buffer(Buf->Handles+2, Type, ElementCount);
  Buf->Buffer.End = ElementCount;
}

