
inline void
FlushBuffersToCard(gpu_mapped_element_buffer* Buffer)
{
  glBindBuffer(GL_ARRAY_BUFFER, Buffer->VertexHandle);
  u32 BufferUnmapped = glUnmapBuffer(GL_ARRAY_BUFFER);
  Buffer->VertexMemory = 0;

  glBindBuffer(GL_ARRAY_BUFFER, Buffer->ColorHandle);
  BufferUnmapped += glUnmapBuffer(GL_ARRAY_BUFFER);
  Buffer->ColorMemory = 0;

  glBindBuffer(GL_ARRAY_BUFFER, Buffer->NormalHandle);
  BufferUnmapped += glUnmapBuffer(GL_ARRAY_BUFFER);
  Buffer->NormalMemory = 0;

  if (!BufferUnmapped)
  {
    Error("glUnmapBuffer Failed");
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return;
}

void
MapGpuElementBuffer(gpu_mapped_element_buffer *ElementBuffer)
{
  TIMED_FUNCTION();

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, ElementBuffer->VertexHandle);
  ElementBuffer->VertexMemory = (v3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, ElementBuffer->NormalHandle);
  ElementBuffer->NormalMemory = (v3*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, ElementBuffer->ColorHandle);
  ElementBuffer->ColorMemory = (v4*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  if (!ElementBuffer->VertexMemory) { Error("Allocating gpu_mapped_element_buffer::Verts"); }
  if (!ElementBuffer->NormalMemory) { Error("Allocating gpu_mapped_element_buffer::Normals"); }
  if (!ElementBuffer->ColorMemory)  { Error("Allocating gpu_mapped_element_buffer::Colors"); }

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return;
}

void
AllocateGpuElementBuffer(gpu_mapped_element_buffer *ElementBuffer, u32 ElementCount)
{
  u32 v3Size = sizeof(v3)*ElementCount;
  u32 v4Size = sizeof(v4)*ElementCount;

  glGenBuffers(3, &ElementBuffer->VertexHandle);

  glBindBuffer(GL_ARRAY_BUFFER, ElementBuffer->VertexHandle);
  glBufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, ElementBuffer->NormalHandle);
  glBufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, ElementBuffer->ColorHandle);
  glBufferData(GL_ARRAY_BUFFER, v4Size, 0, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  ElementBuffer->ElementCount = ElementCount;

}

