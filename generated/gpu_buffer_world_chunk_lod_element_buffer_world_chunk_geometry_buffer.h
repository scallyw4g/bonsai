// src/engine/render.cpp:998:0


link_internal void
CopyBufferIntoBuffer( world_chunk_geometry_buffer *Src, world_chunk_geometry_buffer *Dest)
{
  BufferVertsChecked(Src, Dest, {}, V3(1.0f));
}

link_internal void
CopyToGpuBuffer( world_chunk_geometry_buffer *Mesh, gpu_mapped_element_buffer *GpuBuffer)
{
  GpuBuffer->Buffer = MapGpuElementBuffer(&GpuBuffer->Handles);
  CopyBufferIntoBuffer(Mesh, &GpuBuffer->Buffer);
  FlushBuffersToCard(GpuBuffer);
}

link_internal void
CopyToGpuBuffer( world_chunk_geometry_buffer *Mesh, gpu_element_buffer_handles *Handles)
{
  world_chunk_geometry_buffer Dest = MapGpuElementBuffer(Handles);
  CopyBufferIntoBuffer(Mesh, &Dest);
  FlushBuffersToCard(Handles);
}

link_internal b32
SyncGpuBuffersAsync(engine_resources *Engine, world_chunk_lod_element_buffer *Meshes)
{
  b32 Result = False;
  Assert(ThreadLocal_ThreadIndex != 1);

  RangeIterator(MeshIndex, MeshIndex_Count)
  {
    world_chunk_mesh_bitfield MeshBit = world_chunk_mesh_bitfield(1 << MeshIndex);
    if (HasMesh(Meshes, MeshBit))
    {
      gpu_element_buffer_handles *Handles = &Meshes->GpuBufferHandles[MeshIndex];

      world_chunk_geometry_buffer *Mesh = AtomicReplaceMesh( Meshes, MeshBit, 0, u64_MAX );
      if (Mesh && Mesh->At)
      {
        PushReallocateBuffersCommand(&Engine->Stdlib.Plat.RenderQ, Handles, Mesh);
        Result = True;
      }
      else
      {
        PushDeallocateBuffersCommand(&Engine->Stdlib.Plat.RenderQ, Handles);
      }
    }
  }

  // TODO(Jesse): Is this actually a thing??
  FullBarrier;

  return Result;
}

link_internal b32
SyncGpuBuffersImmediate(engine_resources *Engine, world_chunk_lod_element_buffer *Meshes)
{
  b32 Result = False;
  Assert(ThreadLocal_ThreadIndex == 1);

  RangeIterator(MeshIndex, MeshIndex_Count)
  {
    world_chunk_mesh_bitfield MeshBit = world_chunk_mesh_bitfield(1 << MeshIndex);
    if (HasMesh(Meshes, MeshBit))
    {
      gpu_element_buffer_handles *Handles = &Meshes->GpuBufferHandles[MeshIndex];

      world_chunk_geometry_buffer *Mesh = AtomicReplaceMesh( Meshes, MeshBit, 0, u64_MAX );
      if (Mesh)
      {
        if (Mesh->At)
        {
          // @duplicate_realloc_code
          if (Handles->VertexHandle)
          {
            GL.DeleteBuffers(3, &Handles->VertexHandle);
            Clear(Handles);
            AssertNoGlErrors;
          }

          AllocateGpuElementBuffer(Handles, Mesh->At);
          CopyToGpuBuffer(Mesh, Handles);
          Result = True;
        }
        else
        {
          DeallocateGpuElementBuffer(Handles);
          AssertNoGlErrors;
        }

        DeallocateMesh(Engine, Mesh);
      }
    }
  }

  AssertNoGlErrors;

  // TODO(Jesse): Is this actually a thing??
  FullBarrier;

  return Result;
}

