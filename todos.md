# Jesse
  ## font
    - Axe this!
    - Can we support these?

  ## cleanup
    - Axe this!
    - Perlin.h depends on this .. rewrite it.
    - Perlin.h depends on this .. rewrite it.
    - Redistribute this
    - Jettison this .. Can it be metaprogrammed?
    - Make this non-const?
    - Replace this expression everywhere!!!  This might be done.
    - Test this actually gets respected!!
    - Test this actually gets respected!!
    - Pass this in!
    - Can we compute this with MouseP to avoid a frame of input delay?
    - Do we have to pass the style to both of these functions, and is that a good idea?
    - Reset this in here?
    - Remove this?
    - This actually has nothing to do with the platform
    - Make this return a counted string and everything that depends on

  ## compiler_feature
    - Do we care about generating these?
    - Generate this?  Need a compiler feature to generate stuff from primitive types.

  ## metaprogramming
    - Do we care about generating these?
    - d_union-ify this
    - Jettison this .. Can it be metaprogrammed?
    - Generate this?  Need a compiler feature to generate stuff from primitive types.
    - Do we actually want this in here?
    - Proper expression parsing.  ie: enum_value_name = (1 << 4) or enum_value_name = SOME_MACRO(thing, ding)
    - Need a way of dynamically allocating more of these on demand

  ## robustness
    - How should we actually set this?
    - 257 seemed to produce slightly
    - Compress to 16 bit float?
    - Make allocating these on the stack work!
    - Test this actually gets respected!!
    - Test this actually gets respected!!
    - Reset this in here?
    - Does this work all the time on x64?
    - Prove this is unnecessary
    - Need a way of dynamically allocating more of these on demand
    - Actually read all the data!
    - Load models in multiple chunks instead of one
    - Are we really done?
    - Should the MaxP be initialized to f32_MIN?
    - Query max gpu textures?
    - This _should_ be able to be glTexImage3D, but the driver is
    - Is this necessary to avoid some pointer aliasing bug?
    - This could probably be made better by writing to a statically allocated buffer ..?
    - Make this return a counted string and everything that depends on

  ## over_allocation
    - How should we actually set this?
    - This seems like _way_ overkill..
    - Better or more accurate way of allocating this size?
    - Better way of allocating this?

  ## xml
    - This could be factored out of here if xml_tag::Properties

  ## back_burner
    - This could be factored out of here if xml_tag::Properties
    - Proper expression parsing.  ie: enum_value_name = (1 << 4) or enum_value_name = SOME_MACRO(thing, ding)
    - Transient arena for this instead of stack allocation ?

  ## rng
    - 257 seemed to produce slightly
    - This is LCG RNG - do we want a better one?

  ## hashing
    - 257 seemed to produce slightly
    - Profile this and see if it's reasonable

  ## completeness
    - This only gets used when computing the shadow map, so I'm not even sure if it works ATM
    - Can we support these?

  ## memory_consumption
    - Compress to 16 bit float?
    - Unneeded for projectiles. factor out of here?
    - 32F is only necessary for reprojection of Position for

  ## entity
    - Unneeded for projectiles. factor out of here?

  ## speed
    - Re-enable this
    - This could be optimized significantly
    - Vectorize for speed boost!
    - Vectorize for speed boost!
    - Profile and check collision rate of this!
    - Pass this in!
    - Can we compute this with MouseP to avoid a frame of input delay?
    - Allocate lights such that this swizzle is unneeded
    - Pretty sure we don't have to set the faces anymore??
    - Can we somehow atomically pull this one off the queue
    - Pretty sure we can do some XOR trickery or something
    - Why are these allocated on the heap?  Seems unnecessary..
    - Is this necessary to avoid some pointer aliasing bug?

  ## untagged
    - Do culling on these as well?

  ## theading
    - Can we somehow atomically pull this one off the queue

  ## gameplay_improvement
    - Can we somehow atomically pull this one off the queue

  ## be_smarter
    - This offset is only used to check if entities are grounded.

  ## not_implemented
    - This path needs to call CanBuildWorldChunkMesh or something similar
    - Pretty sure we can do some XOR trickery or something
    - Allocate in a more sensible way?

  ## texture
    - Query max gpu textures?

  ## ttf_rasterizer
    - Can we support these?
    - Temp-memory?

  ## format_counted_string_api
    - This could probably be made better by writing to a statically allocated buffer ..?

  ## platform
    - This actually has nothing to do with the platform
    - Make this use platform agnostic functions!
    - Should this be done elsewhere?

  ## platform_linux
    - Not getting vsync on my arch laptop...

  ## gross
    - Gross..

  ## hot_reload
    - Doing this properly requires some extra-credit work first.

  ## debug_lib
    - Doing this properly requires some extra-credit work first.

  ## immediate
    - d_union-ify this

  ## opengl
    - This only gets used when computing the shadow map, so I'm not even sure if it works ATM
    - Query max gpu textures?
    - 32F is only necessary for reprojection of Position for
    - Should this be done elsewhere?

  ## engine
    - This only gets used when computing the shadow map, so I'm not even sure if it works ATM

  ## cache_friendly
    - Re-enable this

  ## perlin
    - Perlin.h depends on this .. rewrite it.
    - Perlin.h depends on this .. rewrite it.

  ## format_counted_string
    - Jettison this .. Can it be metaprogrammed?

  ## aabb
    - This could be optimized significantly

  ## open_question
    - Make this non-const?
    - Do we actually want this in here?
    - Are we really done?
    - This _should_ be able to be glTexImage3D, but the driver is
    - Is there a better way of making that adjustment?
    - This could probably be made better by writing to a statically allocated buffer ..?
    - Not getting vsync on my arch laptop...

  ## improvement
    - This is LCG RNG - do we want a better one?
    - Write actual/working fModf!

  ## api_improvement
    - Make allocating these on the stack work!

  ## profile
    - Profile and check collision rate of this!

  ## hash
    - Profile and check collision rate of this!

  ## already_done_elsewhere
    - Profile and check collision rate of this!

  ## high_priority
    - Profile and check collision rate of this!

  ## math
    - Write actual/working fModf!

  ## potential_bug
    - Do we have to pass the style to both of these functions, and is that a good idea?
    - Should these 0s be set to s32_MIN??

  ## ui
    - Should we do something special when interacting with this thing instead of Ignored-ing it?

  ## semantic
    - Should we do something special when interacting with this thing instead of Ignored-ing it?

  ## bootstrap_debug_system
    - This is copy-pasted from teh callgraph tests .. should we be

  ## copy_paste
    - This is copy-pasted from teh callgraph tests .. should we be

  ## tests
    - Does this work all the time on x64?

  ## memory
    - Prove this is unnecessary

  ## transient_memory
    - Transient arena for this instead of stack allocation ?
    - Transient storage?
    - Transient storage
    - Temp-memory?

  ## allocation
    - Transient arena for this instead of stack allocation ?
    - Allocate lights such that this swizzle is unneeded
    - Allocate in a more sensible way?
    - Why are these allocated on the heap?  Seems unnecessary..

  ## open_gl
    - Not present on ES2 .. should we use them?

  ## es2
    - Not present on ES2 .. should we use them?

  ## vox_loader
    - Actually read all the data!
    - Should these 0s be set to s32_MIN??
    - Load models in multiple chunks instead of one
    - Are we really done?

  ## set_to_f32_min
    - Should these 0s be set to s32_MIN??
    - Should the MaxP be initialized to f32_MIN?

  ## collada_loader
    - Should the MaxP be initialized to f32_MIN?

  ## dead_code
    - Pretty sure we don't have to set the faces anymore??


