
// GPU resources for one in-flight terrain gen job.  Each job slot owns its own
// set so jobs never stomp on each other's intermediate results.
//
// Chunk k, voxel (x,y,z) is stored at texel (x + D*y, z + D*k), so the textures
// are (D*D, D*TERRAIN_BATCH_CHUNK_COUNT).  Shaping is (D+2) sized, the rest are D sized.
//
struct terrain_gen_job_texture_pack
{
  // Stage 0
  texture     ShapingTex;
  framebuffer ShapingFBO;

  // Stage 1
  texture     DerivsTex;
  framebuffer DerivsFBO;

  // Stage 2 (decoration) writes PingPong[0], world edits ping-pong between them
  rtt_framebuffer PingPong[3];

  // Final u32 packing, read back through the PBO
  texture     FinalizeTex;
  framebuffer FinalizeFBO;
};

struct terrain_gen_job_context
{
  work_queue_job *Job;

  gpu_readback_buffer ReadbackHandles;
  u32 *ReadbackBuffer;
  s64 ReadbackBufferByteLen;
  s64 ReadbackBufferElementLen;

  terrain_gen_job_texture_pack Textures;
};


link_internal void
FinalizeNoiseValues(            terrain_gen_job_context *Context,
                                                    v3i  NoiseDim,
octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT  Batch );

link_internal void
CheckNoiseReadbackJob(           terrain_gen_job_context *Context,
                                                     v3i  NoiseDim,
 octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT  Batch );

link_internal void
InitializeNoiseBuffer(              terrain_gen_job_context *Context,
    octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT  Batch);

link_internal void
FinalizeShitAndFuckinDoStuff(       terrain_gen_job_context *Context,
    octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT  Batch );
