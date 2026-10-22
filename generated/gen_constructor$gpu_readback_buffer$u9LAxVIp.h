// callsite
// src/engine/graphics.h:159:0

// def (gen_constructor)
// external/bonsai_stdlib/src/poof_functions.h:126:0
link_internal gpu_readback_buffer
GpuReadbackBuffer(  u32   PBO , GLsync   Fence , u32 *  MappedBuffer  )
{
  gpu_readback_buffer Reuslt = {
            .PBO = PBO

,        .Fence = Fence

,        .MappedBuffer = MappedBuffer


  };

    
  
  

  return Reuslt;
}

