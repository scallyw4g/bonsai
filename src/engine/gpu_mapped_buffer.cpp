
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



  const u32 MtlFloatElements = sizeof(matl)/sizeof(f32);
  CAssert(MtlFloatElements == 5);

  GL.EnableVertexAttribArray(2);
  GL.EnableVertexAttribArray(3);
  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->MatHandle);
  BufferUnmapped &= GL.UnmapBuffer(GL_ARRAY_BUFFER);
  GpuMap->Buffer.Mat = 0;
  GL.VertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(matl), (void*)0);
  GL.VertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(matl), (void*)12);
  AssertNoGlErrors;

  /* GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->TransEmissHandle); */
  /* BufferUnmapped &= GL.UnmapBuffer(GL_ARRAY_BUFFER); */
  /* GpuMap->Buffer.TransEmiss = 0; */
  /* GL.VertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); */
  /* AssertNoGlErrors; */


  if (BufferUnmapped == False) { Error("glUnmapBuffer Failed"); }

  /* GL.BindBuffer(GL_ARRAY_BUFFER, 0); */

  return;
}

void
AllocateGpuElementBuffer(gpu_mapped_element_buffer *GpuMap, u32 ElementCount)
{
  u32 v2Size = sizeof(v2)*ElementCount;
  u32 v3Size = sizeof(v3)*ElementCount;
  u32 matlSize = sizeof(matl)*ElementCount;

  GL.GenBuffers(4, &GpuMap->VertexHandle);

  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->VertexHandle);
  GL.BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->NormalHandle);
  GL.BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->MatHandle);
  GL.BufferData(GL_ARRAY_BUFFER, matlSize, 0, GL_STATIC_DRAW);

  /* GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->TransEmissHandle); */
  /* GL.BufferData(GL_ARRAY_BUFFER, v2Size, 0, GL_STATIC_DRAW); */

  GL.BindBuffer(GL_ARRAY_BUFFER, 0);

  GpuMap->Buffer.End = ElementCount;
}

link_internal void 
DeallocateGpuElementBuffer(gpu_mapped_element_buffer *Buf)
{
  GL.DeleteBuffers(4, &Buf->VertexHandle);
}

link_internal untextured_3d_geometry_buffer *
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


  u32 ElementCount = GpuMap->Buffer.End;

  u32 v2Size = sizeof(v2)*ElementCount;
  u32 v3Size = sizeof(v3)*ElementCount;
  u32 matlSize = sizeof(matl)*ElementCount;

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


  // Color data
  GL.EnableVertexAttribArray(2);
  GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->MatHandle);
  GpuMap->Buffer.Mat = (matl*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, matlSize, GL_MAP_WRITE_BIT);
  GL.VertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(matl), (void*)0);
  AssertNoGlErrors;

  // transparency & emission data
  GL.EnableVertexAttribArray(3);
  /* GL.BindBuffer(GL_ARRAY_BUFFER, GpuMap->MatHandle); */
  /* GpuMap->Buffer.Mat = (matl*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, matlSize, GL_MAP_WRITE_BIT); */
  GL.VertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(matl), (void*)12);
  AssertNoGlErrors;

  if (!GpuMap->Buffer.Verts)      { Error("Allocating gpu_mapped_element_buffer::Verts"); }
  if (!GpuMap->Buffer.Normals)    { Error("Allocating gpu_mapped_element_buffer::Normals"); }
  if (!GpuMap->Buffer.Mat)        { Error("Allocating gpu_mapped_element_buffer::Mat"); }

  GL.BindBuffer(GL_ARRAY_BUFFER, 0);

  return &GpuMap->Buffer;
}

