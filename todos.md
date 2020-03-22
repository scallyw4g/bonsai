# Jesse
  ## untagged
    - #154 This is pretty shitty because whenever we copy one of these
    - #189 Parse out function names?

  ## immediate
    - #83 d_union-ify this
    - #184 Reinstate memory protection!  Need to turn it off
    - #157 Should support multi-line comments as well
    - #156 This should actually concat all comments such that
    - #155 This should eat the comment if
    - #159 Rename to Members
    - #190 Does this make more sense named 'Type'

  ## high_priority
    - #101 Profile and check collision rate of this!
    - #182 This should respect Args.Outpath passed in!
    - #183 This should respect Args.OutPath passed in!
    - #185 these should be printable!

  ## format_counted_string
    - #91 Jettison this .. Can it be metaprogrammed?

  ## compiler_feature
    - #99 Generate this?  Need a compiler feature to generate stuff from primitive types.

  ## memory_consumption
    - #85 Compress to 16 bit float?
    - #86 Unneeded for projectiles. factor out of here?
    - #138 32F is only necessary for reprojection of Position for

  ## speed
    - #87 Re-enable this
    - #92 This could be optimized significantly
    - #96 Vectorize for speed boost!
    - #97 Vectorize for speed boost!
    - #101 Profile and check collision rate of this!
    - #106 Pass this in!
    - #107 Can we compute this with MouseP to avoid a frame of input delay?
    - #120 Allocate lights such that this swizzle is unneeded
    - #127 Pretty sure we don't have to set the faces anymore??
    - #128 Do culling on these as well?
    - #129 Can we somehow atomically pull this one off the queue
    - #132 Pretty sure we can do some XOR trickery or something
    - #136 Why are these allocated on the heap?  Seems unnecessary..
    - #139 Is this necessary to avoid some pointer aliasing bug?

  ## cleanup
    - #77 Axe this!
    - #88 Perlin.h depends on this .. rewrite it.
    - #89 Perlin.h depends on this .. rewrite it.
    - #90 Redistribute this
    - #91 Jettison this .. Can it be metaprogrammed?
    - #94 Make this non-const?
    - #103 Replace this expression everywhere!!!  This might be done.
    - #104 Test this actually gets respected!!
    - #105 Test this actually gets respected!!
    - #106 Pass this in!
    - #107 Can we compute this with MouseP to avoid a frame of input delay?
    - #108 Do we have to pass the style to both of these functions, and is that a good idea?
    - #109 Reset this in here?
    - #113 Remove these?
    - #144 This actually has nothing to do with the platform
    - #146 Make this return a counted string and everything that depends on
    - #159 Rename to Members
    - #188 This should have the name property, instead of

  ## rng
    - #82 257 seemed to produce slightly
    - #95 This is LCG RNG - do we want a better one?

  ## metaprogramming
    - #83 d_union-ify this
    - #91 Jettison this .. Can it be metaprogrammed?
    - #99 Generate this?  Need a compiler feature to generate stuff from primitive types.
    - #100 Do we actually want this in here?
    - #121 Need a way of dynamically allocating more of these on demand
    - #160 Generate this!
    - #180 Can we use Allocate() here instead?
    - #191 This can be generated, but it requires
    - #192 This is a function call or macro .. make sure it's actually constant.

  ## robustness
    - #78 How should we actually set this?
    - #82 257 seemed to produce slightly
    - #85 Compress to 16 bit float?
    - #98 Make allocating these on the stack work!
    - #104 Test this actually gets respected!!
    - #105 Test this actually gets respected!!
    - #109 Reset this in here?
    - #115 Does this work all the time on x64?
    - #116 Prove this is unnecessary
    - #121 Need a way of dynamically allocating more of these on demand
    - #122 Actually read all the data!
    - #124 Load models in multiple chunks instead of one
    - #125 Are we really done?
    - #126 Should the MaxP be initialized to f32_MIN?
    - #128 Do culling on these as well?
    - #135 Query max gpu textures?
    - #137 This _should_ be able to be glTexImage3D, but the driver is
    - #139 Is this necessary to avoid some pointer aliasing bug?
    - #143 This could probably be made better by writing to a statically allocated buffer ..?
    - #146 Make this return a counted string and everything that depends on

  ## font
    - #77 Axe this!
    - #140 Can we support these?

  ## over_allocation
    - #78 How should we actually set this?
    - #79 Can this just be a v3?
    - #93 This seems like _way_ overkill..
    - #148 Better or more accurate way of allocating this size?
    - #149 Better way of allocating this?

  ## ummmm
    - #79 Can this just be a v3?

  ## xml
    - #80 This could be factored out of here if xml_tag::Properties

  ## back_burner
    - #161 Reinstate this! */
    - #80 This could be factored out of here if xml_tag::Properties
    - #117 Transient arena for this instead of stack allocation ?

  ## string_hash
    - #81 Theres' a null terminated string-hash
    - #101 Profile and check collision rate of this!

  ## hashing
    - #82 257 seemed to produce slightly
    - #147 Profile this and see if it's reasonable

  ## opengl
    - #84 This only gets used when computing the shadow map, so I'm not even sure if it works ATM
    - #119 Not present on ES2 .. should we use them?
    - #135 Query max gpu textures?
    - #138 32F is only necessary for reprojection of Position for
    - #150 Should this be done elsewhere?

  ## completeness
    - #84 This only gets used when computing the shadow map, so I'm not even sure if it works ATM
    - #140 Can we support these?

  ## engine
    - #84 This only gets used when computing the shadow map, so I'm not even sure if it works ATM

  ## entity
    - #86 Unneeded for projectiles. factor out of here?

  ## cache_friendly
    - #87 Re-enable this

  ## perlin
    - #88 Perlin.h depends on this .. rewrite it.
    - #89 Perlin.h depends on this .. rewrite it.

  ## aabb
    - #92 This could be optimized significantly

  ## open_question
    - #94 Make this non-const?
    - #100 Do we actually want this in here?
    - #125 Are we really done?
    - #137 This _should_ be able to be glTexImage3D, but the driver is
    - #142 Is there a better way of making that adjustment?
    - #143 This could probably be made better by writing to a statically allocated buffer ..?
    - #151 Not getting vsync on my arch laptop...

  ## improvement
    - #95 This is LCG RNG - do we want a better one?
    - #102 Write actual/working fModf!

  ## api_improvement
    - #98 Make allocating these on the stack work!

  ## profile
    - #101 Profile and check collision rate of this!

  ## hash
    - #101 Profile and check collision rate of this!

  ## already_done_elsewhere
    - #101 Profile and check collision rate of this!

  ## math
    - #102 Write actual/working fModf!

  ## potential_bug
    - #108 Do we have to pass the style to both of these functions, and is that a good idea?
    - #123 Should these 0s be set to s32_MIN??

  ## bootstrap_debug_system
    - #112 This is copy-pasted from the callgraph tests .. should we be

  ## copy_paste
    - #112 This is copy-pasted from the callgraph tests .. should we be

  ## tests
    - #115 Does this work all the time on x64?

  ## memory
    - #116 Prove this is unnecessary

  ## transient_memory
    - #117 Transient arena for this instead of stack allocation ?
    - #118 Transient storage?
    - #134 Transient storage
    - #141 Temp-memory?

  ## allocation
    - #117 Transient arena for this instead of stack allocation ?
    - #120 Allocate lights such that this swizzle is unneeded
    - #133 Allocate in a more sensible way?
    - #136 Why are these allocated on the heap?  Seems unnecessary..

  ## es2
    - #119 Not present on ES2 .. should we use them?

  ## vox_loader
    - #122 Actually read all the data!
    - #123 Should these 0s be set to s32_MIN??
    - #124 Load models in multiple chunks instead of one
    - #125 Are we really done?

  ## set_to_f32_min
    - #123 Should these 0s be set to s32_MIN??
    - #126 Should the MaxP be initialized to f32_MIN?

  ## collada_loader
    - #126 Should the MaxP be initialized to f32_MIN?

  ## dead_code
    - #127 Pretty sure we don't have to set the faces anymore??

  ## correctness
    - #128 Do culling on these as well?

  ## theading
    - #129 Can we somehow atomically pull this one off the queue

  ## gameplay_improvement
    - #129 Can we somehow atomically pull this one off the queue

  ## be_smarter
    - #130 This offset is only used to check if entities are grounded.

  ## not_implemented
    - #131 This path needs to call CanBuildWorldChunkMesh or something similar
    - #132 Pretty sure we can do some XOR trickery or something
    - #133 Allocate in a more sensible way?

  ## texture
    - #135 Query max gpu textures?

  ## ttf_rasterizer
    - #140 Can we support these?
    - #141 Temp-memory?

  ## format_counted_string_api
    - #143 This could probably be made better by writing to a statically allocated buffer ..?

  ## platform
    - #144 This actually has nothing to do with the platform
    - #145 Make this use platform agnostic functions!
    - #150 Should this be done elsewhere?

  ## platform_linux
    - #151 Not getting vsync on my arch laptop...

  ## gross
    - #52 Gross..

  ## hot_reload
    - #153 Doing this properly requires some extra-credit work first.

  ## debug_lib
    - #153 Doing this properly requires some extra-credit work first.

  ## hashtable
    - #160 Generate this!

  ## debug_recording
    - #161 Reinstate this! */

  ## ui
    - #110 Should we do something special when interacting with this thing instead of Ignored-ing it?

  ## semantic
    - #110 Should we do something special when interacting with this thing instead of Ignored-ing it?

  ## parsing
    - #192 This is a function call or macro .. make sure it's actually constant.

  ## bug
    - #185 these should be printable!

