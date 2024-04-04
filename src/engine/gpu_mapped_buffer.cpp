
inline b32
FlushBuffersToCard(gpu_element_buffer_handles* Handles)
{
  TIMED_FUNCTION();

  GL.EnableVertexAttribArray(0);
  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
  u32 BufferUnmapped = GL.UnmapBuffer(GL_ARRAY_BUFFER);
  GL.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  GL.EnableVertexAttribArray(1);
  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
  BufferUnmapped &= GL.UnmapBuffer(GL_ARRAY_BUFFER);
  GL.VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;



  const u32 MtlFloatElements = sizeof(matl)/sizeof(u8);
  CAssert(MtlFloatElements == 10);

  GL.EnableVertexAttribArray(2);
  GL.EnableVertexAttribArray(3);
  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
  BufferUnmapped &= GL.UnmapBuffer(GL_ARRAY_BUFFER);
  GL.VertexAttribPointer(2, 1, GL_SHORT, GL_FALSE, sizeof(matl), (void*)0); // Color
  GL.VertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(matl), (void*)12); // TransEmiss
  AssertNoGlErrors;

  if (BufferUnmapped == False) { Error("glUnmapBuffer Failed"); }

  // TODO(Jesse): The transparency code doesn't work if I uncomment these.. why??
  /* GL.BindBuffer(GL_ARRAY_BUFFER, 0); */
  /* GL.DisableVertexAttribArray(0); */
  /* GL.DisableVertexAttribArray(1); */
  /* GL.DisableVertexAttribArray(2); */
  /* GL.DisableVertexAttribArray(3); */

  return BufferUnmapped;
}

inline b32
FlushBuffersToCard(gpu_mapped_element_buffer* Buffer)
{
  TIMED_FUNCTION();
  b32 Result = FlushBuffersToCard(&Buffer->Handles);
  /* Clear(&Buffer->Buffer); */
  return Result;
}

void
AllocateGpuElementBuffer(gpu_element_buffer_handles *Handles, u32 ElementCount)
{
  u32 v2Size = sizeof(v2)*ElementCount;
  u32 v3Size = sizeof(v3)*ElementCount;
  u32 matlSize = sizeof(matl)*ElementCount;

  Assert(Handles->VertexHandle == 0);
  Assert(Handles->NormalHandle == 0);
  Assert(Handles->MatHandle == 0);

  GL.GenBuffers(3, &Handles->VertexHandle);
  Handles->ElementCount = ElementCount;

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
  GL.BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
  GL.BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
  GL.BufferData(GL_ARRAY_BUFFER, matlSize, 0, GL_STATIC_DRAW);

  GL.BindBuffer(GL_ARRAY_BUFFER, 0);
}

void
AllocateGpuElementBuffer(gpu_mapped_element_buffer *GpuMap, u32 ElementCount)
{
  AllocateGpuElementBuffer(&GpuMap->Handles, ElementCount);
  GpuMap->Buffer.End = ElementCount;
}

link_internal void 
DeallocateGpuElementBuffer(gpu_mapped_element_buffer *Buf)
{
  GL.DeleteBuffers(3, &Buf->Handles.VertexHandle);
  Clear(Buf);
}

link_internal untextured_3d_geometry_buffer
MapGpuElementBuffer(gpu_element_buffer_handles *Handles)
{
  TIMED_FUNCTION();

  untextured_3d_geometry_buffer Result = {};
  Result.End = Handles->ElementCount;

  u32 v2Size = sizeof(v2)*Handles->ElementCount;
  u32 v3Size = sizeof(v3)*Handles->ElementCount;
  u32 matlSize = sizeof(matl)*Handles->ElementCount;

  GL.EnableVertexAttribArray(0);
  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
  Result.Verts = (v3*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, v3Size, GL_MAP_WRITE_BIT);
  GL.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  GL.EnableVertexAttribArray(1);
  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
  Result.Normals = (v3*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, v3Size, GL_MAP_WRITE_BIT);
  GL.VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  // Color data
  GL.EnableVertexAttribArray(2);
  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
  Result.Mat = (matl*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, matlSize, GL_MAP_WRITE_BIT);
  GL.VertexAttribPointer(2, 1, GL_SHORT, GL_FALSE, sizeof(matl), (void*)0);
  AssertNoGlErrors;

  // transparency & emission data
  GL.EnableVertexAttribArray(3);
  GL.VertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(matl), (void*)12);
  AssertNoGlErrors;

  if (!Result.Verts)   { Error("Allocating gpu_mapped_element_buffer::Verts");   }
  if (!Result.Normals) { Error("Allocating gpu_mapped_element_buffer::Normals"); }
  if (!Result.Mat)     { Error("Allocating gpu_mapped_element_buffer::Mat");     }

  GL.BindBuffer(GL_ARRAY_BUFFER, 0);

  GL.DisableVertexAttribArray(0);
  GL.DisableVertexAttribArray(1);
  GL.DisableVertexAttribArray(2);
  GL.DisableVertexAttribArray(3);

  return Result;
}

link_internal void
MapGpuElementBuffer(gpu_mapped_element_buffer *GpuMap)
{
  TIMED_FUNCTION();

  Assert(GpuMap->Buffer.Parent == False);
  if (GpuMap->Buffer.BufferNeedsToGrow)
  {
    GpuMap->Buffer.End += GpuMap->Buffer.BufferNeedsToGrow;
    DeallocateGpuElementBuffer(GpuMap);
    AllocateGpuElementBuffer(GpuMap, GpuMap->Buffer.End);
    GpuMap->Buffer.BufferNeedsToGrow = 0;
  }


  GpuMap->Buffer = MapGpuElementBuffer(&GpuMap->Handles);
}

