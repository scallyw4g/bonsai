
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

struct world_chunk;

struct work_queue_entry_build_chunk_mesh
{
  gen_chunk   *GenChunk;
  octree_node *DestNode;
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
  func asyncify_function_h(func_t)
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
  func asyncify_function_c(func_t)
  {
    link_internal work_queue_entry
    (func_t.name)_Task(
        work_queue *Queue,
        func_t.map(arg).sep(,) { arg }                     /// Closure args
        func_t.value? { , func_t.value* FuncResultDest } ) /// Func result pointer (optional)
    {
      (func_t.name.to_snake_case)_async_params Params =
      {
        func_t.value?   {  FuncResultDest, }
        func_t.map(arg) { arg.name, }
      };

      work_queue_entry Result = WorkQueueEntryAsyncFunction(Queue, &Params);
      return Result;
    }

    link_internal void
    (func_t.name)_Async(
        work_queue *Queue,
        func_t.map(arg).sep(,) { arg }
        func_t.value? { , func_t.value* Result } )
    {
      auto Task = (func_t.name)_Task( Queue,
        func_t.map(arg).sep(,) { arg.name }
        func_t.value? { , Result }
      );
      SubmitSingleTask(Queue, &Task);
    }

    link_internal void
    ExecFunction((func_t.name.to_snake_case)_async_params *Params)
    {
      func_t.value? { auto Result = } func_t.name((func_t.map(arg).sep(,) { Params->(arg.name) }));
      func_t.value? { if (Params->Result) { *Params->Result = Result; } }
    }
  }
)

poof(
  for_datatypes(func)
  func (func_t)
  {
    func_t.has_tag(async)?
    {
      asyncify_function_h(func_t)
    }
  }
)
#include <generated/poof_builtin.for_datatypes$$RMp9Gsbc.h>


// Genereate tagged_union for async functions

enum async_function_call_type
{
  poof(
    for_datatypes(struct) @code_fragment
    func (struct_t)
    {
      struct_t.has_tag(async_function_params)?
      {
        type_(struct_t.name),
      }
    }
  )
#include <generated/poof_builtin.for_datatypes$$fOKTiPYO.h>
};

struct work_queue_entry_async_function_call
{
  async_function_call_type Type;
  union
  {
    poof(
      for_datatypes(struct) @code_fragment
      func (struct_t)
      {
        struct_t.has_tag(async_function_params)?
        {
          struct_t.name struct_t.name;
        }
      }
    )
#include <generated/poof_builtin.for_datatypes$$2hdQoKeJ.h>
  };
};

poof(string_and_value_tables(async_function_call_type))
#include <generated/string_and_value_tables$async_function_call_type$hJOrda0k.h>



poof(
  d_union work_queue_entry
  {
    work_queue_entry_build_chunk_mesh
    work_queue_entry_init_asset
    work_queue_entry_sim_particle_system


    // NOTE(Jesse): This is kind of a hack to put render commands onto the work
    // queue so I don't have to invent a whole generic system for having queues
    // with seperate work entry types.  I should probably do this sometime in
    // the future, but for now I'm just going to stuff it on here and call it good.
    work_queue_entry__bonsai_render_command

    work_queue_entry_async_function_call

    work_queue_entry__align_to_cache_line_helper
  },
  {
    // NOTE(Jesse): This is the queue the job needs to be submitted to to
    // complete this task.
    work_queue_ptr Queue;
  }
)
#include <generated/poof_builtin.d_union$$YdAfLGDb.h>

typedef work_queue_entry work_queue_task;

poof(string_and_value_tables(work_queue_entry_type))
#include <generated/string_and_value_tables$work_queue_entry_type$qLS3T3lb.h>

// nocheckin
// TODO(Jesse): Turn this on
/* CAssert(sizeof(work_queue_entry) % CACHE_LINE_SIZE == 0); */

poof(d_union_constructors(work_queue_entry))
#include <generated/d_union_constructors$work_queue_entry$L3AUuvyf.h>

poof(block_array_h(work_queue_entry, {8}, {}))
#include <generated/block_array_h$work_queue_entry.688856411.0$MUE9gaIj.h>

// TODO(Jesse): Do alignment and padding for cache lines
#define WORK_QUEUE_JOB_MAGIC_NUMBER (0x1337)
struct work_queue_job
{
  work_queue_job *Next;
  work_queue_entry_block_array Tasks;

  u16 Magic;              // WORK_QUEUE_JOB_MAGIC_NUMBER
  u16 NextTaskIndex;      // Index into Tasks for the next task to Pop

  global_job_index Index; // global index for this job; indexes into platform::Jobs
};

poof(
  for_datatypes(struct) @code_fragment
  func (struct_t)
  {
    struct_t.has_tag(async_function_params)?
    {
      struct struct_t.name;
      link_internal work_queue_entry
      WorkQueueEntryAsyncFunction( work_queue *Queue, (struct_t.name) *Params )
      {
        work_queue_entry Result = {};
        Result.Queue = Queue;
        Result.Type = type_work_queue_entry_async_function_call;
        Result.work_queue_entry_async_function_call.Type = type_(struct_t.name);
        Result.work_queue_entry_async_function_call.(struct_t.name) = *Params;
        return Result;
      }
    }
  }
)
#include <generated/poof_builtin.for_datatypes$$Xs04c1ly.h>



poof(
  for_datatypes(func) @code_fragment
  func (func_t)
  {
    func_t.has_tag(async)?
    {
      asyncify_function_c(func_t)
    }
  }
)
#include <generated/poof_builtin.for_datatypes$$Xst5mK32.h>

link_internal void
DispatchAsyncFunctionCall(work_queue_entry_async_function_call *WrappedTask)
{
  tswitch(WrappedTask)
  {
    poof(
      func (async_function_call_type tag_t) @code_fragment
      {
        tag_t.map(tag_v)
        {
          {
            tmatch( tag_v.name.strip_single_prefix, WrappedTask, FuncParams );
            ExecFunction(FuncParams);
          } break;
        }
      }
    )
#include <generated/poof_func.anonymous$async_function_call_type$xS6OHMBZ.h>
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
WorkQueueEntry( work_queue *Queue, particle_system *System, v3 EntityDelta, v3 RenderSpaceP, r32 dt)
{
  work_queue_entry Result = WorkQueueEntry(WorkQueueEntrySimParticleSystem(System, EntityDelta, RenderSpaceP, dt), Queue);
  return Result;
}

link_internal b32
MaybeResubmitJob(work_queue_job *Job)
{
  b32 Result = False;
  if (work_queue_entry *Next = PeekNextTask(Job))
  {
    Result = True;
    SubmitJob(Next->Queue, Job);
  }
  else
  {
    ReleaseWorkQueueJob(GetPlatform(), Job);
  }
  return Result;
}

link_internal void
HandleJob(work_queue_job *Job, thread_local_state *Thread, application_api *GameApi)
{
  if ( GameApi->WorkerMain &&
       GameApi->WorkerMain(Job, Thread))
  {
    // Game exported a WorkerMain, and it handled the job
  }
  else
  {
    WorkerThread_ApplicationDefaultImplementation(Job, Thread);
  }

  MaybeResubmitJob(Job);
}

link_internal untextured_3d_geometry_buffer *
TakeOwnershipSync(lod_element_buffer *Buf, world_chunk_mesh_bitfield MeshBit);

link_internal void
ReleaseOwnership(lod_element_buffer *Src, world_chunk_mesh_bitfield MeshBit, untextured_3d_geometry_buffer *Buf);

