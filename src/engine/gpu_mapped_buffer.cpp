
// NOTE(Jesse): This hooks up the vertex attribs because in some cases
// (immediate geo buffer) we flush and draw immediately afterwards.
// 
// Should probably move to using VAOs so we don't have to do this.
//
link_inline b32
FlushBuffersToCard(gpu_mapped_element_buffer *Buffer)
{
  TIMED_FUNCTION();
  gpu_element_buffer_handles* Handles = &Buffer->Handles;
  Assert(Handles->Mapped == True);
  Handles->Mapped = False;

  AssertNoGlErrors;

  GetGL()->EnableVertexAttribArray(VERTEX_POSITION_LAYOUT_LOCATION);
  GetGL()->EnableVertexAttribArray(VERTEX_NORMAL_LAYOUT_LOCATION);
  GetGL()->EnableVertexAttribArray(VERTEX_COLOR_LAYOUT_LOCATION);
  GetGL()->EnableVertexAttribArray(VERTEX_TRANS_EMISS_LAYOUT_LOCATION);

  AssertNoGlErrors;

  u32 BufferUnmapped = 0;
  switch (Handles->ElementType)
  {
    InvalidCase(DataType_Undefinded);

    case DataType_v3:
    {
      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
      GetGL()->VertexAttribPointer(VERTEX_POSITION_LAYOUT_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
      BufferUnmapped = GetGL()->UnmapBuffer(GL_ARRAY_BUFFER);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
      GetGL()->VertexAttribPointer(VERTEX_NORMAL_LAYOUT_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
      BufferUnmapped &= GetGL()->UnmapBuffer(GL_ARRAY_BUFFER);
      AssertNoGlErrors;
    } break;

    case DataType_v3_u8:
    {
      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
      GetGL()->VertexAttribIPointer(VERTEX_POSITION_LAYOUT_LOCATION, 3, GL_BYTE, 0, (void*)0);
      BufferUnmapped = GetGL()->UnmapBuffer(GL_ARRAY_BUFFER);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
      GetGL()->VertexAttribPointer(VERTEX_NORMAL_LAYOUT_LOCATION, 3, GL_BYTE, GL_TRUE, 0, (void*)0);
      BufferUnmapped &= GetGL()->UnmapBuffer(GL_ARRAY_BUFFER);
      AssertNoGlErrors;

    } break;
  }

  // NOTE(Jesse): This is just here to break when the size of these changes,
  // serving as a reminder to update this code.
  const u32 MtlFloatElements = sizeof(matl)/sizeof(u8);
  CAssert(MtlFloatElements == 4);

  GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
  GetGL()->VertexAttribIPointer(VERTEX_COLOR_LAYOUT_LOCATION,       1, GL_SHORT, sizeof(matl), Cast(void*, OffsetOf(  ColorIndex, matl)) );
  GetGL()->VertexAttribIPointer(VERTEX_TRANS_EMISS_LAYOUT_LOCATION, 2, GL_BYTE,  sizeof(matl), Cast(void*, OffsetOf(Transparency, matl)) ); // @vertex_attrib_I_pointer_transparency_offsetof
  BufferUnmapped &= GetGL()->UnmapBuffer(GL_ARRAY_BUFFER);
  AssertNoGlErrors;

  if (BufferUnmapped == False) { Error("glUnmapBuffer Failed"); }
  return BufferUnmapped;
}

link_internal void
AllocateGpuElementBuffer(gpu_element_buffer_handles *Handles, data_type Type, u32 ElementCount)
{
  Assert(ElementCount);
  Assert(Handles->Mapped == False);
  Assert(Handles->ElementType == DataType_Undefinded);

  u32 matlSize = sizeof(matl)*ElementCount;

  Assert(Handles->VertexHandle == 0);
  Assert(Handles->NormalHandle == 0);
  Assert(Handles->MatHandle == 0);

  Handles->ElementType = Type;
  Handles->ElementCount = ElementCount;

  switch (Type)
  {
    InvalidCase(DataType_Undefinded);

    case DataType_v3:
    {
      u32 v3Size   = sizeof(v3)*ElementCount;
      GetGL()->GenBuffers(3, &Handles->VertexHandle);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
      GetGL()->BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
      GetGL()->BufferData(GL_ARRAY_BUFFER, v3Size, 0, GL_STATIC_DRAW);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
      GetGL()->BufferData(GL_ARRAY_BUFFER, matlSize, 0, GL_STATIC_DRAW);
      AssertNoGlErrors;

    } break;

    case DataType_v3_u8:
    {
      u32 v3u8Size   = sizeof(v3_u8)*ElementCount;
      GetGL()->GenBuffers(3, &Handles->VertexHandle);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
      GetGL()->BufferData(GL_ARRAY_BUFFER, v3u8Size, 0, GL_STATIC_DRAW);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
      GetGL()->BufferData(GL_ARRAY_BUFFER, v3u8Size, 0, GL_STATIC_DRAW);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
      GetGL()->BufferData(GL_ARRAY_BUFFER, matlSize, 0, GL_STATIC_DRAW);
      AssertNoGlErrors;

    } break;
  }

  GetGL()->BindBuffer(GL_ARRAY_BUFFER, 0);
  AssertNoGlErrors;
}

link_internal void
AllocateGpuElementBuffer(gpu_mapped_element_buffer *GpuMap, data_type Type, u32 ElementCount)
{
  AllocateGpuElementBuffer(&GpuMap->Handles, Type, ElementCount);
  GpuMap->Buffer.End = ElementCount;
}

link_internal void 
DeallocateGpuElementBuffer(gpu_element_buffer_handles *Handles)
{
  GetGL()->DeleteBuffers(3, &Handles->VertexHandle);
  Clear(Handles);
}

link_internal void 
DeallocateGpuElementBuffer(gpu_mapped_element_buffer *Buf)
{
  DeallocateGpuElementBuffer(&Buf->Handles);
  Clear(&Buf->Buffer);
}

link_internal gpu_mapped_untextured_3d_geometry_buffer
MapGpuBuffer_untextured_3d_geometry_buffer(gpu_element_buffer_handles *Handles)
{
  TIMED_FUNCTION();
  AssertNoGlErrors;

  Assert(Handles->Mapped == False);
  Handles->Mapped = True;

  untextured_3d_geometry_buffer Buffer = {};
  Buffer.End = Handles->ElementCount;
  Buffer.Type = Handles->ElementType;

  u32 MaterialBufferSize = sizeof(matl)*Handles->ElementCount;

  switch (Handles->ElementType)
  {
    InvalidCase(DataType_Undefinded);

    case DataType_v3:
    {
      u32 BufferSize = sizeof(v3)*Handles->ElementCount;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
      AssertNoGlErrors;
      Buffer.Verts = (v3*) GetGL()->MapBufferRange(GL_ARRAY_BUFFER, 0, BufferSize, GL_MAP_WRITE_BIT);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
      AssertNoGlErrors;
      Buffer.Normals = (v3*) GetGL()->MapBufferRange(GL_ARRAY_BUFFER, 0, BufferSize, GL_MAP_WRITE_BIT);
      AssertNoGlErrors;

      // Color data
      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
      AssertNoGlErrors;
      Buffer.Mat = (matl*) GetGL()->MapBufferRange(GL_ARRAY_BUFFER, 0, MaterialBufferSize, GL_MAP_WRITE_BIT);
      AssertNoGlErrors;

    } break;

    case DataType_v3_u8:
    {
      u32 BufferSize = sizeof(v3_u8)*Handles->ElementCount;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->VertexHandle);
      AssertNoGlErrors;
      Buffer.Verts = (v3_u8*) GetGL()->MapBufferRange(GL_ARRAY_BUFFER, 0, BufferSize, GL_MAP_WRITE_BIT);
      AssertNoGlErrors;

      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->NormalHandle);
      AssertNoGlErrors;
      Buffer.Normals = (v3_u8*) GetGL()->MapBufferRange(GL_ARRAY_BUFFER, 0, BufferSize, GL_MAP_WRITE_BIT);
      AssertNoGlErrors;

      // Color data
      GetGL()->BindBuffer(GL_ARRAY_BUFFER, Handles->MatHandle);
      AssertNoGlErrors;
      Buffer.Mat = (matl*) GetGL()->MapBufferRange(GL_ARRAY_BUFFER, 0, MaterialBufferSize, GL_MAP_WRITE_BIT);
      AssertNoGlErrors;

    } break;

  }

  if (!Buffer.Verts)   { Error("Allocating gpu_mapped_element_buffer::Verts");   }
  if (!Buffer.Normals) { Error("Allocating gpu_mapped_element_buffer::Normals"); }
  if (!Buffer.Mat)     { Error("Allocating gpu_mapped_element_buffer::Mat");     }

  GetGL()->BindBuffer(GL_ARRAY_BUFFER, 0);

  return {*Handles, Buffer};
}

link_internal void
MapGpuBuffer_untextured_3d_geometry_buffer(gpu_mapped_element_buffer *GpuMap)
{
  TIMED_FUNCTION();

  Assert(GpuMap->Buffer.Parent == False);
  if (GpuMap->Buffer.BufferNeedsToGrow)
  {
    GpuMap->Buffer.End += GpuMap->Buffer.BufferNeedsToGrow;
    DeallocateGpuElementBuffer(GpuMap);
    AllocateGpuElementBuffer(GpuMap, GpuMap->Handles.ElementType, GpuMap->Buffer.End);
    GpuMap->Buffer.BufferNeedsToGrow = 0;
  }

  GpuMap->Buffer = MapGpuBuffer_untextured_3d_geometry_buffer(&GpuMap->Handles).Buffer;
}
