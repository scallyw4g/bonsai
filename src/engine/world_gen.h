
// GPU resources for one in-flight terrain gen job.  Each job slot owns its own
// set so jobs never stomp on each other's intermediate results.
//
// Chunk k, voxel (x,y,z) is stored at texel (x + D*y, z + D*k), so the textures
// are (D*D, D*TERRAIN_BATCH_CHUNK_COUNT).  Shaping is (D+2) sized, the rest are D sized.
//
struct terrain_gen_job_texture_pack
poof(@do_editor_ui)
{
  // Stage 0
  texture     ShapingTex;
  framebuffer ShapingFBO;

  // Stage 1
  texture     DerivsTex;
  framebuffer DerivsFBO;

  // Stage 2 (decoration) writes PingPong[0], world edits ping-pong between them
  rtt_framebuffer PingPong[3];

  // Final u32 packing is per job; see terrain_gen_job_context
};

// Terrain gen jobs are pipelined across three frames (see graphics::LaunchJobsIndex).
// The job is reserved with the Await flag, and the first stage runs up to the end
// of the draw, at which point the job reaches the Await state.  The next frame we
// push the tasks that consume the readback and resubmit the job, which reaches
// Await a second time when it's done.  The frame after that we unawait and retire it.
//
enum terrain_gen_job_stage
{
  TerrainGenJobStage_Launch,
  TerrainGenJobStage_Readback,
  TerrainGenJobStage_Finalize,
};

struct terrain_gen_job_context
poof(@do_editor_ui)
{
  work_queue_job *Job;
  terrain_gen_job_stage Stage;

  GLsync RenderFence;

  gpu_readback_buffer ReadbackHandles;
  u32 *ReadbackBuffer;
  s64 ReadbackBufferByteLen;
  s64 ReadbackBufferElementLen;

  // Final u32 packing, read back through the PBO.  Per job because the transfer
  // thread reads it asynchronously.
  texture     FinalizeTex;
  framebuffer FinalizeFBO;

  octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT Batch;
};


link_internal void
LaunchTerrainGenJob(terrain_gen_job_context *Context);

link_internal void
FinalizeNoiseValues(terrain_gen_job_context *Context, v3i  NoiseDim);

link_internal void
CheckNoiseReadbackJob( terrain_gen_job_context *Context, v3i  NoiseDim );


link_internal void
FinalizeShitAndFuckinDoStuff( terrain_gen_job_context *Context );

link_internal void
WaitForFence(work_queue_job *Job, GLsync *Fence);

link_internal void
RequestTerrainReadback(terrain_gen_job_context *Context, v2i NoiseDim);

link_internal void
RecieveTerrainReadback(terrain_gen_job_context *Context, v3i NoiseDim);
