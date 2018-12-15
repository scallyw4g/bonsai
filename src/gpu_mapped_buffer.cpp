
inline void
FlushBuffersToCard(gpu_mapped_element_buffer* GpuMap)
{
  glBindBuffer(GL_ARRAY_BUFFER, GpuMap->VertexHandle);
  u32 BufferUnmapped = glUnmapBuffer(GL_ARRAY_BUFFER);
  GpuMap->Buffer.Verts = 0;

  glBindBuffer(GL_ARRAY_BUFFER, GpuMap->ColorHandle);
  BufferUnmapped += glUnmapBuffer(GL_ARRAY_BUFFER);
  GpuMap->Buffer.Colors = 0;

  glBindBuffer(GL_ARRAY_BUFFER, GpuMap->NormalHandle);
  BufferUnmapped += glUnmapBuffer(GL_ARRAY_BUFFER);
  GpuMap->Buffer.Normals = 0;

  if (!BufferUnmapped)
  {
    Error("glUnmapBuffer Failed");
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return;
}

void
MapGpuElementBuffer(gpu_mapped_element_buffer *GpuMap)
{
  TIMED_FUNCTION();

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, GpuMap->VertexHandle);
  GpuMap->Buffer.Verts = (v3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, GpuMap->NormalHandle);
  GpuMap->Buffer.Normals = (v3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, GpuMap->ColorHandle);
  GpuMap->Buffer.Colors = (v4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  if (!GpuMap->Buffer.Verts) { Error("Allocating gpu_mapped_element_buffer::Verts"); }
  if (!GpuMap->Buffer.Normals) { Error("Allocating gpu_mapped_element_buffer::Normals"); }
  if (!GpuMap->Buffer.Colors)  { Error("Allocating gpu_mapped_element_buffer::Colors"); }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return;
}

void
AllocateGpuElementBuffer(gpu_mapped_element_buffer *GpuMap, u32 ElementCount)
{
  u32 v3Size = sizeof(v3)*ElementCount;
  u32 v4Size = sizeof(v4)*ElementCount;

  glGenBuffers(3, &GpuMap->VertexHandle);

  glBindBuffer(GL_ARRAY_BUFFER, GpuMap->VertexHandle);
  glBufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, GpuMap->NormalHandle);
  glBufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, GpuMap->ColorHandle);
  glBufferData(GL_ARRAY_BUFFER, v4Size, 0, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GpuMap->Buffer.End = ElementCount;

}

