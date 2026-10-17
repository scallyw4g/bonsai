// callsite
// external/bonsai_stdlib/src/work_queue.cpp:6:0

// def (poof_builtin.for_datatypes)
// external/bonsai_stdlib/src/work_queue.cpp:6:0
















link_internal work_queue_task
MakeTexture_RGB_Task(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                            
);

link_internal work_queue_job *
MakeTexture_RGB_Job(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
MakeTexture_RGB_Async(
  work_queue *Queue
  , v2i Dim , v3 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);
































link_internal work_queue_task
SetupShader_Task(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId                            
);

link_internal work_queue_job *
SetupShader_Job(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
SetupShader_Async(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);





























































































































link_internal work_queue_task
InitializeNoiseBuffer_Task(
  work_queue *Queue
  , octree_node *Node , work_queue_job *Job                            
);

link_internal work_queue_job *
InitializeNoiseBuffer_Job(
  work_queue *Queue
  , octree_node *Node , work_queue_job *Job                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
InitializeNoiseBuffer_Async(
  work_queue *Queue
  , octree_node *Node , work_queue_job *Job 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);










































link_internal work_queue_task
DoRenderStuff_Task(
  work_queue *Queue
                             
);

link_internal work_queue_job *
DoRenderStuff_Job(
  work_queue *Queue
                             
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
DoRenderStuff_Async(
  work_queue *Queue
  
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);














































link_internal work_queue_task
FinalizeShitAndFuckinDoStuff_Task(
  work_queue *Queue
  , gen_chunk *GenChunk , octree_node *DestNode                            
);

link_internal work_queue_job *
FinalizeShitAndFuckinDoStuff_Job(
  work_queue *Queue
  , gen_chunk *GenChunk , octree_node *DestNode                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
FinalizeShitAndFuckinDoStuff_Async(
  work_queue *Queue
  , gen_chunk *GenChunk , octree_node *DestNode 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);
















































































































link_internal work_queue_task
CompileNewShader_Task(
  work_queue *Queue
  , shader *Shader , cs ShaderName , cs VertPath , cs FragPath , memory_arena *PermMem , b32 DumpErrors , b32 RegisterForHotReload                            
);

link_internal work_queue_job *
CompileNewShader_Job(
  work_queue *Queue
  , shader *Shader , cs ShaderName , cs VertPath , cs FragPath , memory_arena *PermMem , b32 DumpErrors , b32 RegisterForHotReload                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
CompileNewShader_Async(
  work_queue *Queue
  , shader *Shader , cs ShaderName , cs VertPath , cs FragPath , memory_arena *PermMem , b32 DumpErrors , b32 RegisterForHotReload 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);















































































































































link_internal work_queue_task
ClearFramebuffers_Task(
  work_queue *Queue
  , graphics *Graphics , render_to_texture_group *RTTGroup                            
);

link_internal work_queue_job *
ClearFramebuffers_Job(
  work_queue *Queue
  , graphics *Graphics , render_to_texture_group *RTTGroup                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
ClearFramebuffers_Async(
  work_queue *Queue
  , graphics *Graphics , render_to_texture_group *RTTGroup 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);


















































































































































































































































link_internal work_queue_task
CompileShaderPair_Task(
  work_queue *Queue
  , shader *Shader , b32 DumpErrors , b32 RegisterForHotReload                            
);

link_internal work_queue_job *
CompileShaderPair_Job(
  work_queue *Queue
  , shader *Shader , b32 DumpErrors , b32 RegisterForHotReload                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
CompileShaderPair_Async(
  work_queue *Queue
  , shader *Shader , b32 DumpErrors , b32 RegisterForHotReload 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);
















































































































































































































link_internal work_queue_task
InitializeEasingFunctionVisualizerRenderPass_Task(
  work_queue *Queue
  , easing_function_visualizer_render_pass *Element , easing_function *Func , memory_arena *PermMemory                            
);

link_internal work_queue_job *
InitializeEasingFunctionVisualizerRenderPass_Job(
  work_queue *Queue
  , easing_function_visualizer_render_pass *Element , easing_function *Func , memory_arena *PermMemory                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
InitializeEasingFunctionVisualizerRenderPass_Async(
  work_queue *Queue
  , easing_function_visualizer_render_pass *Element , easing_function *Func , memory_arena *PermMemory 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);
















































link_internal work_queue_task
RenderDrawList_Task(
  work_queue *Queue
  , engine_resources *Engine , octree_node_ptr_paged_list *DrawList , shader *Shader , camera *Camera                            
);

link_internal work_queue_job *
RenderDrawList_Job(
  work_queue *Queue
  , engine_resources *Engine , octree_node_ptr_paged_list *DrawList , shader *Shader , camera *Camera                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
RenderDrawList_Async(
  work_queue *Queue
  , engine_resources *Engine , octree_node_ptr_paged_list *DrawList , shader *Shader , camera *Camera 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);



























































































































































link_internal work_queue_task
AllocateTexture_Task(
  work_queue *Queue
  , texture *Texture , void  *Data                            
);

link_internal work_queue_job *
AllocateTexture_Job(
  work_queue *Queue
  , texture *Texture , void  *Data                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
AllocateTexture_Async(
  work_queue *Queue
  , texture *Texture , void  *Data 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);




















link_internal work_queue_task
CheckOcclusionQuery_Task(
  work_queue *Queue
  , world_chunk *Chunk                            
);

link_internal work_queue_job *
CheckOcclusionQuery_Job(
  work_queue *Queue
  , world_chunk *Chunk                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
CheckOcclusionQuery_Async(
  work_queue *Queue
  , world_chunk *Chunk 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);










































































































































































link_internal work_queue_task
TeardownShader_Task(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId                            
);

link_internal work_queue_job *
TeardownShader_Job(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
TeardownShader_Async(
  work_queue *Queue
  , bonsai_render_command_shader_id ShaderId 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);



















































































link_internal work_queue_task
CheckNoiseReadbackJob_Task(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , v3i NoiseDim , octree_node *DestNode                            
);

link_internal work_queue_job *
CheckNoiseReadbackJob_Job(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , v3i NoiseDim , octree_node *DestNode                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
CheckNoiseReadbackJob_Async(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , v3i NoiseDim , octree_node *DestNode 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);


















































































































































































































































link_internal work_queue_task
UnmapAndDeallocatePBO_Task(
  work_queue *Queue
  , gpu_readback_buffer PBOBuf                            
);

link_internal work_queue_job *
UnmapAndDeallocatePBO_Job(
  work_queue *Queue
  , gpu_readback_buffer PBOBuf                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
UnmapAndDeallocatePBO_Async(
  work_queue *Queue
  , gpu_readback_buffer PBOBuf 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);









































































link_internal work_queue_task
RenderToTexture_gpu_mapped_element_buffer_Task(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_mapped_element_buffer *Src , v3 Offset , camera *Camera                            
);

link_internal work_queue_job *
RenderToTexture_gpu_mapped_element_buffer_Job(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_mapped_element_buffer *Src , v3 Offset , camera *Camera                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
RenderToTexture_gpu_mapped_element_buffer_Async(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_mapped_element_buffer *Src , v3 Offset , camera *Camera 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);



link_internal work_queue_task
DrawEntities_Task(
  work_queue *Queue
  , shader *Shader                            
);

link_internal work_queue_job *
DrawEntities_Job(
  work_queue *Queue
  , shader *Shader                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
DrawEntities_Async(
  work_queue *Queue
  , shader *Shader 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

























































































































































link_internal work_queue_task
RenderToTexture_gpu_heap_allocation_Task(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_heap_allocation *Src , v3 Offset , camera *Camera                            
);

link_internal work_queue_job *
RenderToTexture_gpu_heap_allocation_Job(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_heap_allocation *Src , v3 Offset , camera *Camera                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
RenderToTexture_gpu_heap_allocation_Async(
  work_queue *Queue
  , engine_resources *Engine , asset_thumbnail *Thumb , gpu_heap_allocation *Src , v3 Offset , camera *Camera 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);



































































































































































link_internal work_queue_task
FinalizeNoiseValues_Task(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , u32 *NoiseData , v3i NoiseDim , octree_node *DestNode                            
);

link_internal work_queue_job *
FinalizeNoiseValues_Job(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , u32 *NoiseData , v3i NoiseDim , octree_node *DestNode                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
FinalizeNoiseValues_Async(
  work_queue *Queue
  , work_queue_job *Job , gpu_readback_buffer PBOBuf , u32 *NoiseData , v3i NoiseDim , octree_node *DestNode 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);





































































link_internal work_queue_task
MakeTexture_RGBA_Task(
  work_queue *Queue
  , v2i Dim , v4 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                            
);

link_internal work_queue_job *
MakeTexture_RGBA_Job(
  work_queue *Queue
  , v2i Dim , v4 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat                            
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);

link_internal global_job_index
MakeTexture_RGBA_Async(
  work_queue *Queue
  , v2i Dim , v4 *Data , cs DebugName , u32 Slices , texture_storage_format StorageFormat 
  , work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_None
);
























































































