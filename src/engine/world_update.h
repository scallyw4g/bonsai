
struct voxel_stack_element
{
  v3i VoxSimP;
  voxel_rule_direction Dir;
};

link_internal b32
AreEqual(voxel_stack_element E0, voxel_stack_element E1)
{
  NotImplemented;
  return False;
}

poof(gen_constructor(voxel_stack_element))
#include <generated/gen_constructor$voxel_stack_element$ShFXbCFX.h>

poof(block_array(voxel_stack_element, {32}))
#include <generated/block_array$voxel_stack_element.688853862$hU2foeae.h>

poof(generate_cursor(voxel_stack_element))
#include <generated/generate_cursor$voxel_stack_element$x3Qsodb6.h>


struct apply_world_edit_params
{
  world_edit_blend_mode Mode;
  rect3i SSRect;
  rect3i SimSpaceUpdateBounds;
  world_chunk *CopiedChunk;
  b32 Invert;
  b32 PersistWhitespace;
  u16 Color;
  u8 Transparency;
};


#define UNPACK_APPLY_WORLD_EDIT_PARAMS(P)                \
  world_edit_blend_mode Mode = P->Mode;                        \
  rect3i SSRect = P->SSRect;                             \
  rect3i SimSpaceUpdateBounds = P->SimSpaceUpdateBounds; \
  world_chunk *CopiedChunk = P->CopiedChunk;             \
  v3i UpdateDim = GetDim(SimSpaceUpdateBounds); \
  b32 Invert = P->Invert




link_internal v3i
ChunkCountForDim(v3i Dim, v3i ChunkDim)
{
  v3i Fixup = Min(V3i(1), Dim % ChunkDim);

  v3i Result = (Dim/ChunkDim) + Fixup;
  return Result;
}




/* struct work_queue_entry_update_world_region; */

/* link_internal void */
/* QueueWorldUpdateForRegion( engine_resources *Engine, world_edit_blend_mode  Mode, world_edit_blend_mode_modifier  Modifier, /1* world_edit_shape *Shape, *1/ v3 HSVColor, b32 PersistWhitespace, memory_arena *Memory ); */

/* link_internal void */
/* ApplyUpdateToRegion(thread_local_state *Thread, work_queue_entry_update_world_region *Job, rect3i SimSpaceUpdateBounds, world_chunk *CopiedChunk, b32 Invert = False, b32 PersistWhitespace = False); */

/* link_internal void */
/* DoWorldUpdate(work_queue *Queue, world *World, thread_local_state *Thread, work_queue_entry_update_world_region *Job); */

struct work_queue_entry_finalize_noise_values;

link_internal void
FinalizeNoiseValues( work_queue_job *Job, u32 *BatchNoiseData, v3i  NoiseDim, octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT Batch );

inline void
QueueChunkForInit(terrain_gen_job_context *Context, octree_node *Node);

link_internal terrain_gen_job_context *
GetOrReserveTerrainJobContext(graphics *Graphics, work_queue *Queue);

// Advances terrain gen jobs that have reached the Await state; call once per frame
link_internal void
ContinueTerrainJobs(graphics *Graphics);

// Submits the job being filled (or retires it if empty)
link_internal void
SubmitTerrainJob(terrain_gen_job_context *);

link_internal octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT *
GetCurrentTerrainBatch(terrain_gen_job_context *Context);

link_internal terrain_gen_job_context *
ReserveTerrainJob(work_queue *Queue);
