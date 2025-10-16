
struct work_queue_entry_copy_buffer_ref
{
  untextured_3d_geometry_buffer *Dest;

  lod_element_buffer *Buf;
  world_chunk_mesh_bitfield MeshBit;

  v3 Basis;
};
CAssert(sizeof(work_queue_entry_copy_buffer_ref) == 8*4);

struct particle_system;
struct work_queue_entry_sim_particle_system
{
  particle_system *System;
  untextured_3d_geometry_buffer *TranspDest;
  /* untextured_3d_geometry_buffer *EmissiveDest; */
  untextured_3d_geometry_buffer *SolidDest;
  v3 EntityDelta;
  v3 RenderSpaceP;
  r32 dt;
};
// TODO(Jesse)(poof): Make poof able to generate this!
/* poof(gen_constructor(work_queue_entry_sim_particle_system)) */

link_internal work_queue_entry_sim_particle_system
WorkQueueEntrySimParticleSystem( particle_system *System, v3 EntityDelta, v3 RenderSpaceP, r32 dt)
{
  UNPACK_ENGINE_RESOURCES( GetEngineResources() );

  untextured_3d_geometry_buffer *TranspDest = &Graphics->Transparency.GpuBuffer.Buffer;
  /* untextured_3d_geometry_buffer *EmissiveDest = &Graphics->Bloom.GpuBuffer.Buffer; */
  untextured_3d_geometry_buffer *SolidDest = &GpuMap->Buffer;

  work_queue_entry_sim_particle_system Result = {
    .System = System,

    .TranspDest = TranspDest,
    /* .EmissiveDest = EmissiveDest, */
    .SolidDest = SolidDest,

    .EntityDelta = EntityDelta,
    .RenderSpaceP = RenderSpaceP,
    .dt = dt,
  };
  return Result;
}

#define WORK_QUEUE_MAX_COPY_TARGETS 7
struct work_queue_entry_copy_buffer_set
{
  u32 Count;
  work_queue_entry_copy_buffer_ref CopyTargets[WORK_QUEUE_MAX_COPY_TARGETS];
};

struct world_chunk;
struct work_queue_entry_init_world_chunk
{
  world_chunk *Chunk;
};

struct work_queue_entry_finalize_noise_values
{
  gpu_readback_buffer PBOBuf;

  u16 *NoiseData;
  v3i  NoiseDim;

  octree_node *DestNode;
};

struct work_queue_entry_build_chunk_mesh
{
  world_chunk *SynChunk;
  octree_node *DestNode;
};

struct work_queue_entry_rebuild_mesh
{
  world_chunk *Chunk;
  chunk_init_flags Flags;
};






struct asset;
struct work_queue_entry_init_asset
{
  asset *Asset;
};

struct work_queue_entry__align_to_cache_line_helper
{
  // NOTE(Jesse): This ensures the union size is a multiple of a cache line.
  // Sometimes needs to be adjusted if the number of cache lines spanned grows
  //
  // Sub 8 for the type tag in the work_queue_entry
  u8 Pad[(CACHE_LINE_SIZE*4) -8];
};
CAssert( (sizeof(work_queue_entry__align_to_cache_line_helper)+8) % CACHE_LINE_SIZE == 0);





poof(
  func asyncify_render_function_h(func_t) @code_fragment
  {
    struct (func_t.name.to_snake_case)_async_params poof(@async_function_params)
    {
      func_t.value ? { func_t.value* Result; }
      func_t.map(arg) 
      {
        arg;
      }
    };
  }
)


poof(
  func asyncify_render_function_c(func_t)
  {
    link_internal void
    (func_t.name)_Async(work_queue *Queue, func_t.map(arg).sep(,) { arg } func_t.value? { , func_t.value* Result })
    {
      (func_t.name.to_snake_case)_async_params Params =
      {
        func_t.value? {  Result, } func_t.map(arg) { arg.name, }
      };

      work_queue_entry Entry = WorkQueueEntryAsyncFunction(&Params);
      PushWorkQueueEntry(Queue, &Entry);
    }

    link_internal void
    DoJob((func_t.name.to_snake_case)_async_params *Params)
    {
      func_t.value? { auto Result = } func_t.name((func_t.map(arg).sep(,) { Params->(arg.name) }));
      func_t.value? { if (Params->Result) { *Params->Result = Result; } }
    }
  }
)

poof(
  for_datatypes(all) @code_fragment
  func (struct_t) {}
  func (enum_t) {}
  func (func_t)
  {
    func_t.has_tag(async)?
    {
      func_t.has_tag(render)?
      {
        asyncify_render_function_h(func_t)
      }
    }
  }
)
#include <generated/for_datatypes_khY6kwEk.h>


// Genereate tagged_union for async functions

enum async_function_call_type
{
  poof(
    for_datatypes(all) @code_fragment
    func (struct_t)
    {
      struct_t.has_tag(async_function_params)?
      {
        type_(struct_t.name),
      }
    }
    func (enum_t) {}
  )
#include <generated/for_datatypes_kv3WBTai.h>
};

struct work_queue_entry_async_function_call
{
  async_function_call_type Type;
  union
  {
    poof(
      for_datatypes(all) @code_fragment
      func (struct_t)
      {
        struct_t.has_tag(async_function_params)?
        {
          struct_t.name struct_t.name;
        }
      }
      func (enum_t) {}
    )
#include <generated/for_datatypes_fkubhsYl.h>
  };
};



poof(
  d_union work_queue_entry
  {
    work_queue_entry_init_world_chunk
    work_queue_entry_finalize_noise_values
    work_queue_entry_build_chunk_mesh
    /* work_queue_entry_copy_buffer */
    work_queue_entry_copy_buffer_set
    work_queue_entry_copy_buffer_ref
    work_queue_entry_init_asset
    /* work_queue_entry_update_world_region */
    work_queue_entry_rebuild_mesh
    work_queue_entry_sim_particle_system


    // NOTE(Jesse): This is kind of a hack to put render commands onto the work
    // queue so I don't have to invent a whole generic system for having queues
    // with seperate work entry types.  I should probably do this sometime in
    // the future, but for now I'm just going to stuff it on here and call it good.
    work_queue_entry__bonsai_render_command

    work_queue_entry_async_function_call

    work_queue_entry__align_to_cache_line_helper
  }
)
#include <generated/d_union_work_queue_entry.h>

// nocheckin
// TODO(Jesse): Turn this on
/* CAssert(sizeof(work_queue_entry) % CACHE_LINE_SIZE == 0); */

poof(d_union_constructors(work_queue_entry))
#include <generated/d_union_constructors_work_queue_entry.h>



poof(
  for_datatypes(all) @code_fragment
  func (struct_t)
  {
    struct_t.has_tag(async_function_params)?
    {
      struct struct_t.name;
      link_internal work_queue_entry
      WorkQueueEntryAsyncFunction( (struct_t.name) *Params )
      {
        work_queue_entry Result = {};
        Result.Type = type_work_queue_entry_async_function_call;
        Result.work_queue_entry_async_function_call.Type = type_(struct_t.name);
        Result.work_queue_entry_async_function_call.(struct_t.name) = *Params;
        return Result;
      }
    }
  }
  func (enum_t) {}
)
#include <generated/for_datatypes_0XxWqGSZ.h>



poof(
  for_datatypes(all) @code_fragment
  func (struct_t) {}
  func (enum_t) {}
  func (func_t)
  {
    func_t.has_tag(async)?
    {
      func_t.has_tag(render)?
      {
        asyncify_render_function_c(func_t)
      }
    }
  }
)
#include <generated/for_datatypes_cx51CcgQ.h>

link_internal void
DispatchAsyncFunctionCall(work_queue_entry_async_function_call *Task)
{
  tswitch(Task)
  {
    poof(
      func (async_function_call_type tag_t)
      {
        tag_t.map(tag_v)
        {
          {
            tmatch( tag_v.name.strip_single_prefix, Task, Job );
            DoJob(Job);
          } break;
        }
      }
    )
#include <generated/anonymous_async_function_call_type_rQAZtEUA.h>
  }
}













link_internal s32
EventsCurrentlyInQueue(work_queue *Queue)
{
  u32 Enqueue = Queue->EnqueueIndex;
  u32 Dequeue = Queue->DequeueIndex;

  s32 Result = 0;
  if (Dequeue < Enqueue)
  {
    Result = s32(Enqueue - Dequeue);
  }

  if (Enqueue < Dequeue)
  {
    Result = s32((WORK_QUEUE_SIZE - Dequeue) + Enqueue);
  }

  Assert(Result >= 0);
  return Result;
}

// TODO(Jesse): Gen this from the constructors generator
link_internal work_queue_entry
WorkQueueEntry( particle_system *System, v3 EntityDelta, v3 RenderSpaceP, r32 dt)
{
  work_queue_entry Result = WorkQueueEntry(WorkQueueEntrySimParticleSystem(System, EntityDelta, RenderSpaceP, dt));
  return Result;
}



link_internal void
HandleJob(volatile work_queue_entry *Entry, thread_local_state *Thread, application_api *GameApi)
{
  if ( GameApi->WorkerMain &&
       GameApi->WorkerMain(Entry, Thread))
  {
    // Game exported a WorkerMain, and it handled the job
  }
  else if (WorkerThread_ApplicationDefaultImplementation)
  {
    WorkerThread_ApplicationDefaultImplementation(Entry, Thread);
  }
}


link_internal untextured_3d_geometry_buffer *
TakeOwnershipSync(lod_element_buffer *Buf, world_chunk_mesh_bitfield MeshBit);

link_internal void
ReleaseOwnership(lod_element_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf);

