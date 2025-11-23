// callsite
// external/bonsai_stdlib/src/framebuffer.h:18:0

// def (circular_buffer_h)
// external/bonsai_stdlib/src/poof_functions.h:1773:0
/* generate_cursor_struct(element_t) */
link_internal b32
AreEqual(rtt_framebuffer *Thing1, rtt_framebuffer *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( rtt_framebuffer ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(rtt_framebuffer Thing1, rtt_framebuffer Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( rtt_framebuffer ) );

  return Result;
}



typedef rtt_framebuffer_static_cursor_3 rtt_framebuffer_circular_buffer;

