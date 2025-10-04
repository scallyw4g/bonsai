
poof(
  func mesh_allocator(mesh_t, allocator_t, type_poof_symbol allocate_proc)
  {
    link_internal void
    AllocateMesh( mesh_t.name *Mesh, data_type Type, u32 NumVerts, allocator_t.name *Memory)
    {
      switch (Type)
      {
        InvalidCase(DataType_Undefinded); 

        case DataType_v3:
        {
          Mesh->Verts   = allocate_proc(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);
          Mesh->Normals = allocate_proc(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);
          Mesh->Mat     = allocate_proc(matl, Memory, NumVerts, CACHE_LINE_SIZE, False);
        } break;

        case DataType_v3_u8:
        {
          Mesh->Verts   = allocate_proc(v3_u8, Memory, NumVerts, CACHE_LINE_SIZE, False);
          Mesh->Normals = allocate_proc(v3_u8, Memory, NumVerts, CACHE_LINE_SIZE, False);
          Mesh->Mat     = allocate_proc(matl, Memory, NumVerts, CACHE_LINE_SIZE, False);
        } break;
      }

      Mesh->End = NumVerts;
      Mesh->At = 0;
      Mesh->Type = Type;
    }

    link_internal mesh_t.name*
    Allocate_(mesh_t.name)( (allocator_t.name)* Allocator, data_type Type, u32 NumVerts)
    {
      mesh_t.name *Result = allocate_proc((mesh_t.name), Allocator, 1, CACHE_LINE_SIZE, False);
      AllocateMesh(Result, Type, NumVerts, Allocator);
      return Result;
    }

  }
)


poof(mesh_allocator(untextured_3d_geometry_buffer, memory_arena, {AllocateAlignedProtection}))
#include <generated/mesh_allocator_untextured_3d_geometry_buffer_memory_arena_372399955.h>

poof(mesh_allocator(untextured_3d_geometry_buffer, heap_allocator, {HeapAlloc}))
#include <generated/mesh_allocator_untextured_3d_geometry_buffer_heap_allocator_190647831.h>

/* poof(mesh_allocator(world_chunk_geometry_buffer, memory_arena, {AllocateAlignedProtection})) */
/* #include <generated/mesh_allocator_world_chunk_geometry_buffer_memory_arena_821677495.h> */

link_internal void
DeallocateMesh(untextured_3d_geometry_buffer* Mesh, mesh_freelist *MeshFreelist)
{
  Assert(Mesh);
  Assert(Mesh->End <= MeshFreelist->MeshSize);

#if BONSAI_INTERNAL
  AcquireFutex(&MeshFreelist->DebugFutex);
#endif

  Mesh->At = 0;

  Link_TS(&MeshFreelist->FirstFreeMesh, Cast(freelist_entry*, Mesh));

#if BONSAI_INTERNAL
  ReleaseFutex(&MeshFreelist->DebugFutex);
#endif
}

link_internal void
DeallocateMesh(void *VoidMesh, tiered_mesh_freelist* MeshFreelist)
{
  untextured_3d_geometry_buffer *Mesh = Cast(untextured_3d_geometry_buffer*, VoidMesh);
  mesh_freelist *Freelist = TryGetTierForSize(MeshFreelist, Mesh->End);
  if (Freelist)
  {
    u32 Tier = 1+ (Mesh->End/WORLD_CHUNK_MESH_MIN_SIZE);
    u32 Size = Tier*WORLD_CHUNK_MESH_MIN_SIZE;

    if (Freelist->MeshSize == 0)
    {
      Freelist->MeshSize = Size;
    }

    Assert(Freelist->MeshSize >= Mesh->End);
    DeallocateMesh(Mesh, Freelist);
  }
  else
  {
    Leak("Large mesh detected!");
  }
}

link_internal void
DeallocateMesh(engine_resources *Engine, untextured_3d_geometry_buffer* Mesh)
{
  DeallocateMesh(Mesh, &Engine->geo_u3d_MeshFreelist);
}


#if 0
link_internal void
DeallocateMesh(world_chunk_geometry_buffer* Mesh, mesh_freelist *MeshFreelist)
{
  NotImplemented;
  /* DeallocateMesh(Mesh, MeshFreelist); */
}

link_internal void
DeallocateMesh(engine_resources *Engine, world_chunk_geometry_buffer* Mesh)
{
  DeallocateMesh(Mesh, &Engine->world_chunk_MeshFreelist);
}
#endif



poof(
  func threadsafe_mesh_container(container_t, internal_buffer_t)
  {
    link_internal internal_buffer_t.name *
    TakeOwnershipSync( (container_t.name) *Buf, world_chunk_mesh_bitfield MeshBit)
    {
      AcquireFutex(&Buf->Locks[ToIndex(MeshBit)]);
      internal_buffer_t.name *Result = ((internal_buffer_t.name) *)Buf->E[ToIndex(MeshBit)];
      return Result;
    }

    link_internal void
    ReleaseOwnership( (container_t.name) *Src, world_chunk_mesh_bitfield MeshBit, internal_buffer_t.name *Buf)
    {
      /* if (Buf) { Assert(Src->MeshMask & MeshBit);  } */
      ReleaseFutex(&Src->Locks[ToIndex(MeshBit)]);
    }

    link_internal internal_buffer_t.name *
    ReplaceMesh( (container_t.name) *Meshes,
                 world_chunk_mesh_bitfield MeshBit,
                 internal_buffer_t.name *Buf,
                 u64 BufTimestamp )
    {
      Assert( Meshes->Locks[ToIndex(MeshBit)].SignalValue == (u32)ThreadLocal_ThreadIndex );
      /* if (Buf) { Assert(Buf->At); } */

      internal_buffer_t.name *Result = {};
      internal_buffer_t.name *CurrentMesh = ((internal_buffer_t.name)*)Meshes->E[ToIndex(MeshBit)];

      if (CurrentMesh)
      {
        if (CurrentMesh->Timestamp < BufTimestamp)
        {
          Meshes->E[ToIndex(MeshBit)] = Buf;
          Result = CurrentMesh;
        }
        else
        {
          // NOTE(Jesse): If we don't swap this in, we have to return it so it gets freed
          Result = Buf;
        }
      }
      else
      {
        Meshes->E[ToIndex(MeshBit)] = Buf;
      }

      if (Meshes->E[ToIndex(MeshBit)]) { Meshes->MeshMask |= MeshBit; }
      else                             { Meshes->MeshMask &= ~MeshBit; }

      return Result;
    }

    link_internal internal_buffer_t.name *
    AtomicReplaceMesh( (container_t.name) *Meshes,
                       world_chunk_mesh_bitfield MeshBit,
                       internal_buffer_t.name *Buf,
                       u64 BufTimestamp )
    {
      TakeOwnershipSync(Meshes, MeshBit);
      auto Replace = ReplaceMesh(Meshes, MeshBit, Buf, BufTimestamp);
      ReleaseOwnership(Meshes, MeshBit, Buf);
      return Replace;
    }

    link_internal void
    DeallocateMeshes((container_t.name) *Buf, tiered_mesh_freelist* MeshFreelist)
    {
      if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod0,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
      if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod1,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
      if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod2,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
      if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod3,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
      if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Lod4,   0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); }
      /* if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Debug, 0, __rdtsc()) )        { DeallocateMesh(Mesh, MeshFreelist); } */
      /* if ( auto Mesh = AtomicReplaceMesh(Buf, MeshBit_Transparency, 0, __rdtsc()) ) { DeallocateMesh(Mesh, MeshFreelist); } */

      Buf->MeshMask = 0;
    }

    link_internal b32
    HasGpuMesh((container_t.name) *Buf, world_chunk_mesh_bitfield MeshBit)
    {
      b32 Result = (Buf->GpuBufferHandles[ToIndex(MeshBit)].Handles[mesh_VertexHandle] != 0);
      return Result;
    }


    link_internal b32
    HasGpuMesh((container_t.name) *Buf)
    {
      b32 Result = False;
      RangeIterator(MeshIndex, MeshIndex_Count)
      {
        Result |= (Buf->GpuBufferHandles[MeshIndex].Handles[mesh_VertexHandle] != 0);
      }
      return Result;
    }

    link_internal b32
    HasCpuMesh((container_t.name) *Buf)
    {
      b32 Result = False;
      RangeIterator(MeshIndex, MeshIndex_Count)
      {
        Result |= (Buf->E[MeshIndex] != 0);
      }
      return Result;
    }

    link_internal b32
    HasMesh((container_t.name) *Buf, world_chunk_mesh_bitfield MeshBit)
    {
      b32 Result = (Buf->GpuBufferHandles[ToIndex(MeshBit)].Handles[0] != 0);
      return Result;
    }
  }
)

poof(threadsafe_mesh_container(lod_element_buffer, untextured_3d_geometry_buffer))
#include <generated/take_release_sync_lod_element_buffer.h>

/* poof(threadsafe_mesh_container(world_chunk_lod_element_buffer, world_chunk_geometry_buffer)) */
/* #include <generated/threadsafe_mesh_container_world_chunk_lod_element_buffer.h> */


link_internal b32
HasGpuMesh(gpu_mapped_element_buffer *Buf)
{
  b32 Result = (Buf->Handles.Handles[0] != 0);
  return Result;
}

