void
AllocateMesh(untextured_3d_geometry_buffer *Mesh, u32 NumVerts, memory_arena *Memory)
{
  Mesh->Verts      = AllocateAlignedProtection(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);
  Mesh->Normals    = AllocateAlignedProtection(v3, Memory, NumVerts, CACHE_LINE_SIZE, False);
  Mesh->Mat        = AllocateAlignedProtection(matl, Memory, NumVerts, CACHE_LINE_SIZE, False);

  Mesh->End = NumVerts;
  Mesh->At = 0;

  return;
}

#if 1
void
AllocateMesh(untextured_3d_geometry_buffer *Mesh, u32 NumVerts, heap_allocator *Heap)
{
  Mesh->Verts       = (v3*)HeapAllocate(Heap, sizeof(v3)*NumVerts);
  Mesh->Normals     = (v3*)HeapAllocate(Heap, sizeof(v3)*NumVerts);
  Mesh->Mat         = (matl*)HeapAllocate(Heap, sizeof(matl)*NumVerts);

  Mesh->End = NumVerts;
  Mesh->At = 0;

  return;
}

untextured_3d_geometry_buffer*
AllocateMesh(heap_allocator* Heap, u32 NumVerts)
{
  untextured_3d_geometry_buffer* Result = (untextured_3d_geometry_buffer*)HeapAllocate(Heap, sizeof(untextured_3d_geometry_buffer));
  AllocateMesh(Result, NumVerts, Heap);
  return Result;
}
#endif

untextured_3d_geometry_buffer*
AllocateMesh(memory_arena* Arena, u32 NumVerts)
{
  untextured_3d_geometry_buffer* Result = AllocateAlignedProtection(untextured_3d_geometry_buffer, Arena, 1, CACHE_LINE_SIZE, False);
  AllocateMesh(Result, NumVerts, Arena);
  return Result;
}

link_internal void
DeallocateMesh(untextured_3d_geometry_buffer* Mesh, mesh_freelist *MeshFreelist)
{
  Assert(Mesh);

#if BONSAI_INTERNAL
  if (MeshFreelist->MeshSize == 0)
  {
    MeshFreelist->MeshSize = Mesh->End;
  }
  else
  {
    Assert(MeshFreelist->MeshSize == Mesh->End);
  }

  AcquireFutex(&MeshFreelist->DebugFutex);
#endif

  Mesh->At = 0;

  // TODO(Jesse): This is some UULLLLLTRRARAAARRR garbage.. kill it.
  /* free_list_thing *Container = Unlink_TS(&MeshFreelist->Containers); */
  /* if (!Container) { Container = Allocate(free_mesh, Memory, 1); } */
  /* Container->Mesh = Mesh; */

  Link_TS(&MeshFreelist->FirstFreeMesh, Cast(free_list_thing*, Mesh));

#if BONSAI_INTERNAL
  ReleaseFutex(&MeshFreelist->DebugFutex);
#endif
}

link_internal void
DeallocateMesh(untextured_3d_geometry_buffer* Mesh, tiered_mesh_freelist* MeshFreelist)
{
  mesh_freelist *Freelist = TryGetTierForSize(MeshFreelist, Mesh->End);
  if (Freelist)
  {
    u32 Tier = 1+ (Mesh->End/WORLD_CHUNK_MESH_MIN_SIZE);
    u32 Size = Tier*WORLD_CHUNK_MESH_MIN_SIZE;

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
  DeallocateMesh(Mesh, &Engine->MeshFreelist);
}


poof(
  func threadsafe_mesh_container(type)
  {
    link_internal untextured_3d_geometry_buffer *
    TakeOwnershipSync( (type.name) *Buf, world_chunk_mesh_bitfield MeshBit)
    {
      AcquireFutex(&Buf->Locks[ToIndex(MeshBit)]);
      untextured_3d_geometry_buffer *Result = (untextured_3d_geometry_buffer *)Buf->E[ToIndex(MeshBit)];
      return Result;
    }

    link_internal void
    ReleaseOwnership( (type.name) *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf)
    {
      /* if (Buf) { Assert(Src->MeshMask & MeshBit);  } */
      ReleaseFutex(&Src->Locks[ToIndex(MeshBit)]);
    }

    link_internal untextured_3d_geometry_buffer *
    ReplaceMesh( (type.name) *Meshes,
                 world_chunk_mesh_bitfield MeshBit,
                 untextured_3d_geometry_buffer *Buf,
                 u64 BufTimestamp )
    {
      Assert( Meshes->Locks[ToIndex(MeshBit)].SignalValue == (u32)ThreadLocal_ThreadIndex );
      /* if (Buf) { Assert(Buf->At); } */

      untextured_3d_geometry_buffer *Result = {};

      untextured_3d_geometry_buffer *CurrentMesh = (untextured_3d_geometry_buffer*)Meshes->E[ToIndex(MeshBit)];


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

    link_internal untextured_3d_geometry_buffer *
    AtomicReplaceMesh( (type.name) *Meshes,
                       world_chunk_mesh_bitfield MeshBit,
                       untextured_3d_geometry_buffer *Buf,
                       u64 BufTimestamp )
    {
      TakeOwnershipSync(Meshes, MeshBit);
      auto Replace = ReplaceMesh(Meshes, MeshBit, Buf, BufTimestamp);
      ReleaseOwnership(Meshes, MeshBit, Buf);
      return Replace;
    }

    link_internal void
    DeallocateMeshes((type.name) *Buf, tiered_mesh_freelist* MeshFreelist)
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
  }
)

poof(threadsafe_mesh_container(lod_element_buffer))
#include <generated/take_release_sync_lod_element_buffer.h>

poof(threadsafe_mesh_container(world_chunk_lod_element_buffer))
#include <generated/threadsafe_mesh_container_world_chunk_lod_element_buffer.h>


