// callsite
// src/engine/graphics.h:168:0

// def (gen_constructor)
// external/bonsai_stdlib/src/poof_functions.h:115:0
link_internal gpu_readback_buffer
GpuReadbackBuffer(  u32   PBO , GLsync   Fence  )
{
  gpu_readback_buffer Reuslt = {
            .PBO = PBO

,        .Fence = Fence


  };

    
  

  return Reuslt;
}

