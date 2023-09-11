
inline void
FlushBuffersToCard(gpu_mapped_element_buffer* GpuMap)
{
  TIMED_FUNCTION();

  GL.EnableVertexAttribArray(0);
  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->VertexHandle);
  u32 BufferUnmapped = GL.UnmapBuffer(GL_ARRAY_BUFFER);
  GpuMap->Buffer.Verts = 0;
  GL.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  GL.EnableVertexAttribArray(1);
  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->NormalHandle);
  BufferUnmapped &= GL.UnmapBuffer(GL_ARRAY_BUFFER);
  GpuMap->Buffer.Normals = 0;
  GL.VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  GL.EnableVertexAttribArray(2);
  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->ColorHandle);
  BufferUnmapped &= GL.UnmapBuffer(GL_ARRAY_BUFFER);
  GpuMap->Buffer.Colors = 0;
  GL.VertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  if (BufferUnmapped == False) { Error("glUnmapBuffer Failed"); }

  /* GL.BindBuffer(GL_ARRAY_BUFFER, 0); */

  return;
}

link_internal untextured_3d_geometry_buffer *
MapGpuElementBuffer(gpu_mapped_element_buffer *GpuMap)
{
  TIMED_FUNCTION();
  u32 ElementCount = GpuMap->Buffer.End;

  u32 v3Size = sizeof(v3)*ElementCount;
  u32 v4Size = sizeof(v4)*ElementCount;

  GL.EnableVertexAttribArray(0);
  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->VertexHandle);
  GpuMap->Buffer.Verts = (v3*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, v3Size, GL_MAP_WRITE_BIT);
  GL.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  GL.EnableVertexAttribArray(1);
  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->NormalHandle);
  GpuMap->Buffer.Normals = (v3*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, v3Size, GL_MAP_WRITE_BIT);
  GL.VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  GL.EnableVertexAttribArray(2);
  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->ColorHandle);
  GpuMap->Buffer.Colors = (v4*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, v4Size, GL_MAP_WRITE_BIT);
  GL.VertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
  AssertNoGlErrors;

  if (!GpuMap->Buffer.Verts)   { Error("Allocating gpu_mapped_element_buffer::Verts"); }
  if (!GpuMap->Buffer.Normals) { Error("Allocating gpu_mapped_element_buffer::Normals"); }
  if (!GpuMap->Buffer.Colors)  { Error("Allocating gpu_mapped_element_buffer::Colors"); }

  GL.BindBuffer(GL_ARRAY_BUFFER, 0);

  return &GpuMap->Buffer;
}

void
AllocateGpuElementBuffer(gpu_mapped_element_buffer *GpuMap, u32 ElementCount)
{
  u32 v3Size = sizeof(v3)*ElementCount;
  u32 v4Size = sizeof(v4)*ElementCount;

  GL.GenBuffers(3, &GpuMap->VertexHandle);

  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->VertexHandle);
  GL.BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->NormalHandle);
  GL.BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->ColorHandle);
  GL.BufferData(GL_ARRAY_BUFFER, v4Size, 0, GL_STATIC_DRAW);

  GL.BindBuffer(GL_ARRAY_BUFFER, 0);

  GpuMap->Buffer.End = ElementCount;
}

