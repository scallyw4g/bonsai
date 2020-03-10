
# Jesse
  ## font
    - #1 Axe this!
    - #63 Can we support these?

  ## cleanup
    - #1 Axe this!
    - #12 Perlin.h depends on this .. rewrite it.
    - #13 Perlin.h depends on this .. rewrite it.
    - #14 Redistribute this
    - #15 Jettison this .. Can it be metaprogrammed?
    - #18 Make this non-const?
    - #27 Replace this expression everywhere!!!  This might be done.
    - #28 Test this actually gets respected!!
    - #29 Test this actually gets respected!!
    - #30 Pass this in!
    - #31 Can we compute this with MouseP to avoid a frame of input delay?
    - #32 Do we have to pass the style to both of these functions, and is that a good idea?
    - #33 Reset this in here?
    - #37 Remove this?
    - #67 This actually has nothing to do with the platform
    - #69 Make this return a counted string and everything that depends on

  ## compiler_feature
    - #42 Do we care about generating these?
    - #23 Generate this?  Need a compiler feature to generate stuff from primitive types.

  ## metaprogramming
    - #42 Do we care about generating these?
    - #7 d_union-ify this
    - #15 Jettison this .. Can it be metaprogrammed?
    - #23 Generate this?  Need a compiler feature to generate stuff from primitive types.
    - #24 Do we actually want this in here?
    - #35 Proper expression parsing.  ie: enum_value_name = (1 << 4) or enum_value_name = SOME_MACRO(thing, ding)
    - #44 Need a way of dynamically allocating more of these on demand

  ## robustness
    - #2 How should we actually set this?
    - #6 257 seemed to produce slightly
    - #9 Compress to 16 bit float?
    - #22 Make allocating these on the stack work!
    - #28 Test this actually gets respected!!
    - #29 Test this actually gets respected!!
    - #33 Reset this in here?
    - #38 Does this work all the time on x64?
    - #39 Prove this is unnecessary
    - #44 Need a way of dynamically allocating more of these on demand
    - #45 Actually read all the data!
    - #47 Load models in multiple chunks instead of one
    - #48 Are we really done?
    - #49 Should the MaxP be initialized to f32_MIN?
    - #51 Do culling on these as well?
    - #58 Query max gpu textures?
    - #60 This _should_ be able to be glTexImage3D, but the driver is
    - #62 Is this necessary to avoid some pointer aliasing bug?
    - #66 This could probably be made better by writing to a statically allocated buffer ..?
    - #69 Make this return a counted string and everything that depends on

  ## over_allocation
    - #2 How should we actually set this?
    - #3 Can this just be a v3?
    - #17 This seems like _way_ overkill..
    - #71 Better or more accurate way of allocating this size?
    - #72 Better way of allocating this?

  ## ummmm
    - #3 Can this just be a v3?

  ## xml
    - #4 This could be factored out of here if xml_tag::Properties

  ## back_burner
    - #4 This could be factored out of here if xml_tag::Properties
    - #35 Proper expression parsing.  ie: enum_value_name = (1 << 4) or enum_value_name = SOME_MACRO(thing, ding)
    - #40 Transient arena for this instead of stack allocation ?

  ## string_hash
    - #5 Theres' a null terminated string-hash
    - #25 Profile and check collision rate of this!

  ## rng
    - #6 257 seemed to produce slightly
    - #19 This is LCG RNG - do we want a better one?

  ## hashing
    - #6 257 seemed to produce slightly
    - #70 Profile this and see if it's reasonable

  ## immediate
    - #7 d_union-ify this

  ## opengl
    - #8 This only gets used when computing the shadow map, so I'm not even sure if it works ATM
    - #58 Query max gpu textures?
    - #61 32F is only necessary for reprojection of Position for
    - #73 Should this be done elsewhere?

  ## completeness
    - #8 This only gets used when computing the shadow map, so I'm not even sure if it works ATM
    - #63 Can we support these?

  ## engine
    - #8 This only gets used when computing the shadow map, so I'm not even sure if it works ATM

  ## memory_consumption
    - #9 Compress to 16 bit float?
    - #10 Unneeded for projectiles. factor out of here?
    - #61 32F is only necessary for reprojection of Position for

  ## entity
    - #10 Unneeded for projectiles. factor out of here?

  ## speed
    - #11 Re-enable this
    - #16 This could be optimized significantly
    - #20 Vectorize for speed boost!
    - #21 Vectorize for speed boost!
    - #25 Profile and check collision rate of this!
    - #30 Pass this in!
    - #31 Can we compute this with MouseP to avoid a frame of input delay?
    - #43 Allocate lights such that this swizzle is unneeded
    - #50 Pretty sure we don't have to set the faces anymore??
    - #51 Do culling on these as well?
    - #52 Can we somehow atomically pull this one off the queue
    - #55 Pretty sure we can do some XOR trickery or something
    - #59 Why are these allocated on the heap?  Seems unnecessary..
    - #62 Is this necessary to avoid some pointer aliasing bug?

  ## cache_friendly
    - #11 Re-enable this

  ## perlin
    - #12 Perlin.h depends on this .. rewrite it.
    - #13 Perlin.h depends on this .. rewrite it.

  ## format_counted_string
    - #15 Jettison this .. Can it be metaprogrammed?

  ## aabb
    - #16 This could be optimized significantly

  ## open_question
    - #18 Make this non-const?
    - #24 Do we actually want this in here?
    - #48 Are we really done?
    - #60 This _should_ be able to be glTexImage3D, but the driver is
    - #65 Is there a better way of making that adjustment?
    - #66 This could probably be made better by writing to a statically allocated buffer ..?
    - #74 Not getting vsync on my arch laptop...

  ## improvement
    - #19 This is LCG RNG - do we want a better one?
    - #26 Write actual/working fModf!

  ## api_improvement
    - #22 Make allocating these on the stack work!

  ## profile
    - #25 Profile and check collision rate of this!

  ## hash
    - #25 Profile and check collision rate of this!

  ## already_done_elsewhere
    - #25 Profile and check collision rate of this!

  ## high_priority
    - #25 Profile and check collision rate of this!

  ## math
    - #26 Write actual/working fModf!

  ## potential_bug
    - #32 Do we have to pass the style to both of these functions, and is that a good idea?
    - #46 Should these 0s be set to s32_MIN??

  ## ui
    - #34 Should we do something special when interacting with this thing instead of Ignored-ing it?

  ## semantic
    - #34 Should we do something special when interacting with this thing instead of Ignored-ing it?

  ## bootstrap_debug_system
    - #36 This is copy-pasted from teh callgraph tests .. should we be

  ## copy_paste
    - #36 This is copy-pasted from teh callgraph tests .. should we be

  ## tests
    - #38 Does this work all the time on x64?

  ## memory
    - #39 Prove this is unnecessary

  ## transient_memory
    - #40 Transient arena for this instead of stack allocation ?
    - #41 Transient storage?
    - #57 Transient storage
    - #64 Temp-memory?

  ## allocation
    - #40 Transient arena for this instead of stack allocation ?
    - #43 Allocate lights such that this swizzle is unneeded
    - #56 Allocate in a more sensible way?
    - #59 Why are these allocated on the heap?  Seems unnecessary..

  ## open_gl
    - #42 Not present on ES2 .. should we use them?

  ## es2
    - #42 Not present on ES2 .. should we use them?

  ## vox_loader
    - #45 Actually read all the data!
    - #46 Should these 0s be set to s32_MIN??
    - #47 Load models in multiple chunks instead of one
    - #48 Are we really done?

  ## set_to_f32_min
    - #46 Should these 0s be set to s32_MIN??
    - #49 Should the MaxP be initialized to f32_MIN?

  ## collada_loader
    - #49 Should the MaxP be initialized to f32_MIN?

  ## dead_code
    - #50 Pretty sure we don't have to set the faces anymore??

  ## correctness
    - #51 Do culling on these as well?

  ## theading
    - #52 Can we somehow atomically pull this one off the queue

  ## gameplay_improvement
    - #52 Can we somehow atomically pull this one off the queue

  ## be_smarter
    - #53 This offset is only used to check if entities are grounded.

  ## not_implemented
    - #54 This path needs to call CanBuildWorldChunkMesh or something similar
    - #55 Pretty sure we can do some XOR trickery or something
    - #56 Allocate in a more sensible way?

  ## texture
    - #58 Query max gpu textures?

  ## ttf_rasterizer
    - #63 Can we support these?
    - #64 Temp-memory?

  ## format_counted_string_api
    - #66 This could probably be made better by writing to a statically allocated buffer ..?

  ## platform
    - #67 This actually has nothing to do with the platform
    - #68 Make this use platform agnostic functions!
    - #73 Should this be done elsewhere?

  ## platform_linux
    - #74 Not getting vsync on my arch laptop...

  ## gross
    - #75 Gross..

  ## hot_reload
    - #76 Doing this properly requires some extra-credit work first.

  ## debug_lib
    - #76 Doing this properly requires some extra-credit work first.
