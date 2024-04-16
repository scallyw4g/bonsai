
// NOTE(Jesse): This hooks up the vertex attribs because in some cases
// (immediate geo buffer) we flush and draw immediately afterwards.
// 
// Should probably move to using VAOs so we don't have to do this.
link_inline b32
FlushBuffersToCard(gpu_element_buffer_handles* Handles)
{
  TIMED_FUNCTION();

  Assert(Handles->Mapped == True);
  Handles->Mapped = False;

  AssertNoGlErrors;

  GL.EnableVertexAttribArray(VERTEX_POSITION_LAYOUT_LOCATION);
  GL.EnableVertexAttribArray(VERTEX_NORMAL_LAYOUT_LOCATION);
  GL.EnableVertexAttribArray(VERTEX_COLOR_LAYOUT_LOCATION);
  GL.EnableVertexAttribArray(VERTEX_TRANS_EMISS_LAYOUT_LOCATION);

  AssertNoGlErrors;

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
  GL.VertexAttribPointer(VERTEX_POSITION_LAYOUT_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  u32 BufferUnmapped = GL.UnmapBuffer(GL_ARRAY_BUFFER);
  AssertNoGlErrors;

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
  GL.VertexAttribPointer(VERTEX_NORMAL_LAYOUT_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  BufferUnmapped &= GL.UnmapBuffer(GL_ARRAY_BUFFER);
  AssertNoGlErrors;


  // NOTE(Jesse): This is just here to break when the size of these changes,
  // serving as a reminder to update this code.
  const u32 MtlFloatElements = sizeof(matl)/sizeof(u8);
  CAssert(MtlFloatElements == 4);

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
  GL.VertexAttribIPointer(VERTEX_COLOR_LAYOUT_LOCATION,       1, GL_SHORT, sizeof(matl), Cast(void*, OffsetOf(  ColorIndex, matl)));
  GL.VertexAttribIPointer(VERTEX_TRANS_EMISS_LAYOUT_LOCATION, 2,  GL_BYTE, sizeof(matl), Cast(void*, OffsetOf(Transparency, matl)) ); // @vertex_attrib_I_pointer_transparency_offsetof
  BufferUnmapped &= GL.UnmapBuffer(GL_ARRAY_BUFFER);
  AssertNoGlErrors;

  if (BufferUnmapped == False) { Error("glUnmapBuffer Failed"); }
  return BufferUnmapped;
}

link_inline b32
FlushBuffersToCard(gpu_mapped_element_buffer* Buffer)
{
  TIMED_FUNCTION();
  b32 Result = FlushBuffersToCard(&Buffer->Handles);
  /* Clear(&Buffer->Buffer); */
  return Result;
}

link_internal void
AllocateGpuElementBuffer(gpu_element_buffer_handles *Handles, u32 ElementCount)
{
  Assert(ElementCount);

  u32 v2Size   = sizeof(v2)*ElementCount;
  u32 v3Size   = sizeof(v3)*ElementCount;
  u32 matlSize = sizeof(matl)*ElementCount;

  Assert(Handles->VertexHandle == 0);
  Assert(Handles->NormalHandle == 0);
  Assert(Handles->MatHandle == 0);

  GL.GenBuffers(3, &Handles->VertexHandle);
  Handles->ElementCount = ElementCount;
  AssertNoGlErrors;

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
  GL.BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);
  AssertNoGlErrors;

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
  GL.BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);
  AssertNoGlErrors;

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
  GL.BufferData(GL_ARRAY_BUFFER, matlSize, 0, GL_STATIC_DRAW);
  AssertNoGlErrors;

  GL.BindBuffer(GL_ARRAY_BUFFER, 0);
  AssertNoGlErrors;
}

link_internal void
AllocateGpuElementBuffer(gpu_mapped_element_buffer *GpuMap, u32 ElementCount)
{
  AllocateGpuElementBuffer(&GpuMap->Handles, ElementCount);
  GpuMap->Buffer.End = ElementCount;
}

link_internal void 
DeallocateGpuElementBuffer(gpu_element_buffer_handles *Handles)
{
  GL.DeleteBuffers(3, &Handles->VertexHandle);
  Clear(Handles);
}

link_internal void 
DeallocateGpuElementBuffer(gpu_mapped_element_buffer *Buf)
{
  DeallocateGpuElementBuffer(&Buf->Handles);
  Clear(&Buf->Buffer);
}

link_internal untextured_3d_geometry_buffer
MapGpuElementBuffer(gpu_element_buffer_handles *Handles)
{
  TIMED_FUNCTION();
  AssertNoGlErrors;

  Assert(Handles->Mapped == False);
  Handles->Mapped = True;

  untextured_3d_geometry_buffer Result = {};
  Result.End = Handles->ElementCount;

  u32 v2Size   = sizeof(v2)*Handles->ElementCount;
  u32 v3Size   = sizeof(v3)*Handles->ElementCount;
  u32 matlSize = sizeof(matl)*Handles->ElementCount;

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
  AssertNoGlErrors;
  Result.Verts = (v3*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, v3Size, GL_MAP_WRITE_BIT);
  AssertNoGlErrors;

  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
  AssertNoGlErrors;
  Result.Normals = (v3*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, v3Size, GL_MAP_WRITE_BIT);
  AssertNoGlErrors;

  // Color data
  GL.BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
  AssertNoGlErrors;
  Result.Mat = (matl*)GL.MapBufferRange(GL_ARRAY_BUFFER, 0, matlSize, GL_MAP_WRITE_BIT);
  AssertNoGlErrors;

  if (!Result.Verts)   { Error("Allocating gpu_mapped_element_buffer::Verts");   }
  if (!Result.Normals) { Error("Allocating gpu_mapped_element_buffer::Normals"); }
  if (!Result.Mat)     { Error("Allocating gpu_mapped_element_buffer::Mat");     }

  GL.BindBuffer(GL_ARRAY_BUFFER, 0);

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

