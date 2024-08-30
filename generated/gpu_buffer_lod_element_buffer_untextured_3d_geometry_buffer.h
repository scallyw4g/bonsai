// src/engine/render.cpp:1043:0


link_internal void
CopyBufferIntoBuffer( untextured_3d_geometry_buffer *Src, untextured_3d_geometry_buffer *Dest)
{
  BufferVertsChecked(Src, Dest, {}, V3(1.0f));
}

link_internal void
CopyToGpuBuffer( untextured_3d_geometry_buffer *Mesh, gpu_mapped_untextured_3d_geometry_buffer *GpuBuffer)
{
  gpu_mapped_untextured_3d_geometry_buffer Dest = MapGpuBuffer_untextured_3d_geometry_buffer(&GpuBuffer->Handles);
  CopyBufferIntoBuffer(Mesh, &Dest.Buffer);
  FlushBuffersToCard(&Dest);
}

link_internal void
CopyToGpuBuffer( untextured_3d_geometry_buffer *Mesh, gpu_element_buffer_handles *Handles)
{
  gpu_mapped_untextured_3d_geometry_buffer Dest = MapGpuBuffer_untextured_3d_geometry_buffer(Handles);
  CopyBufferIntoBuffer(Mesh, &Dest.Buffer);
  FlushBuffersToCard(&Dest);
}

link_internal b32
SyncGpuBuffersAsync(engine_resources *Engine, lod_element_buffer *Meshes)
{
  b32 Result = False;
  Assert(ThreadLocal_ThreadIndex != 1);

  RangeIterator(MeshIndex, MeshIndex_Count)
  {
    world_chunk_mesh_bitfield MeshBit = world_chunk_mesh_bitfield(1 << MeshIndex);
    if (HasMesh(Meshes, MeshBit))
    {
      gpu_element_buffer_handles *Handles = &Meshes->GpuBufferHandles[MeshIndex];

      untextured_3d_geometry_buffer *Mesh = AtomicReplaceMesh( Meshes, MeshBit, 0, u64_MAX );
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
SyncGpuBuffersImmediate(engine_resources *Engine, lod_element_buffer *Meshes)
{
  b32 Result = False;
  Assert(ThreadLocal_ThreadIndex == 1);

  RangeIterator(MeshIndex, MeshIndex_Count)
  {
    world_chunk_mesh_bitfield MeshBit = world_chunk_mesh_bitfield(1 << MeshIndex);
    if (HasMesh(Meshes, MeshBit))
    {
      gpu_element_buffer_handles *Handles = &Meshes->GpuBufferHandles[MeshIndex];

      untextured_3d_geometry_buffer *Mesh = AtomicReplaceMesh( Meshes, MeshBit, 0, u64_MAX );
      if (Mesh)
      {
        if (Mesh->At)
        {
          ReallocateAndSyncGpuBuffers(Handles, Mesh);
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

