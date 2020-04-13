# Jesse
  ## untagged
    - #154 This is pretty shitty because whenever we copy one of these structs this field has to be manually zeroed out ..
    - #217 Disallow namespaces
    - #197 Have some way of coalescing this to our internal types?
    - #198 Do we ignore this?
    - #207 Emit domain-specific warning here?
    - #220 Disallow namespaces
    - #212 Should we check that the function was defined using the 'function' or 'exported_function' keywords and emit a warning otherwise?
    - #235 Does this belong here?
    - #240 Do we want to store this value?
    - #241 Does this belong here?
    - #242 Stream of variable instances with values??
    - #243 AST Node type for these keywords
    - #244 AST Node type for return value
    - #245 Change to variable_def or variable_decl
    - #246 Not really sure if this should be a pointer or not
    - #249 Get prototype by name for function here.
    - #250 Get arg types if we care?
    - #251 Implement this..

  ## immediate
    - #83 d_union-ify this
    - #222 Re-add [[nodiscard]] here
    - #238 Change name to BitwiseAnd
    - #239 Change name to BitwiseOr

  ## high_priority
    - #101 Profile and check collision rate of this!
    - #185 these should be printable!
    - #182 This should respect Args.Outpath passed in!
    - #183 This should respect Args.OutPath passed in!
    - #230 ParseVariable should work with function pointer types

  ## format_counted_string
    - #91 Jettison this .. Can it be metaprogrammed?

  ## compiler_feature
    - #99 Generate this?  Need a compiler feature to generate stuff from primitive types.

  ## memory_consumption
    - #85 Compress to 16 bit float?
    - #86 Unneeded for projectiles. factor out of here?
    - #138 32F is only necessary for reprojection of Position for calculating AO.  Consider passing this in when creating a Texture?

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
    - #129 Can we somehow atomically pull this one off the queue and initialize it on demand?
    - #132 Pretty sure we can do some XOR trickery or something here to avoid this branch, which could be a large perf win
    - #136 Why are these allocated on the heap?  Seems unnecessary..
    - #139 Is this necessary to avoid some pointer aliasing bug?

  ## cleanup
    - #77 Axe this!
    - #188 This should have the name property, instead of having the struct and enum defs have seperate names
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
    - #146 Make this return a counted string and everything that depends on it _NOT_ rely on the fact it's currently null terminated
    - #238 Change name to BitwiseAnd
    - #239 Change name to BitwiseOr

  ## rng
    - #82 257 seemed to produce slightly worse results, but the source literature seemed to indicate using a constant close to the total number of discrete characters you'll encounter is advisable.  I believe this to be somewhat higher than 53, but it would be worth testing on real-world data (source files).
    - #95 This is LCG RNG - do we want a better one?

  ## metaprogramming
    - #191 This can be generated, but it requires lazily evaluating functions once their dependant types have been generated.  Not sure if this is a good idea or not, but we could do it. * meta( generate_stream_struct(struct_member) ) * Currently the invariant of being able to remove and re-generate all meta output in a single run of the metaprogramming system is broken by this.
    - #194 Use bitflags
    - #193 Metaprogram this.  I've had bugs multiple times because of it.
    - #83 d_union-ify this
    - #91 Jettison this .. Can it be metaprogrammed?
    - #99 Generate this?  Need a compiler feature to generate stuff from primitive types.
    - #100 Do we actually want this in here?
    - #192 This is a function call or macro .. make sure it's actually constant.
    - #221 This is a function constructor, let's be done.
    - #200 Floating-point values should be parsed out in TokenizeAnsiStream()!!
    - #205 To do this properly, we need to parse out and expand macro definitions here.
    - #210 This is a function-macro call .. I think always..?
    - #226 Should we handle this differently?
    - #189 Parse out function names?
    - #222 Re-add [[nodiscard]] here
    - #121 Need a way of dynamically allocating more of these on demand if this unlink fails!
    - #230 ParseVariable should work with function pointer types
    - #160 Generate this!
    - #231 Does this make sense to put this into the type?
    - #232 Should this be the name if it's a constructor?
    - #236 Metaprogram a free_mesh_stream!
    - #237 Metaprogram this!!

  ## robustness
    - #78 How should we actually set this?
    - #82 257 seemed to produce slightly worse results, but the source literature seemed to indicate using a constant close to the total number of discrete characters you'll encounter is advisable.  I believe this to be somewhat higher than 53, but it would be worth testing on real-world data (source files).
    - #85 Compress to 16 bit float?
    - #98 Make allocating these on the stack work!
    - #104 Test this actually gets respected!!
    - #105 Test this actually gets respected!!
    - #109 Reset this in here?
    - #192 This is a function call or macro .. make sure it's actually constant.
    - #115 Does this work all the time on x64?
    - #116 Prove this is unnecessary
    - #121 Need a way of dynamically allocating more of these on demand if this unlink fails!
    - #122 Actually read all the data!
    - #124 Load models in multiple chunks instead of one monolithic one. The storage for chunks must be as large as the largest chunk we will EVER load, which should definately not be decided at compile time.
    - #125 Are we really done?
    - #126 Should the MaxP be initialized to f32_MIN?
    - #128 Do culling on these as well?
    - #135 Query max gpu textures?
    - #137 This _should_ be able to be glTexImage3D, but the driver is throwing an error .. why?!
    - #139 Is this necessary to avoid some pointer aliasing bug?
    - #143 This could probably be made better by writing to a statically allocated buffer ..?
    - #146 Make this return a counted string and everything that depends on it _NOT_ rely on the fact it's currently null terminated

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
    - #80 This could be factored out of here if xml_tag::Properties thing was a xml_token_stream* - which would require a two-pass parser most likely.  ie. Tokenizer and AST builder

  ## back_burner
    - #80 This could be factored out of here if xml_tag::Properties thing was a xml_token_stream* - which would require a two-pass parser most likely.  ie. Tokenizer and AST builder
    - #117 Transient arena for this instead of stack allocation ?
    - #161 Reinstate this!

  ## string_hash
    - #81 Theres' a null terminated string-hash function that should be replaced by this one most likely
    - #101 Profile and check collision rate of this!

  ## hashing
    - #82 257 seemed to produce slightly worse results, but the source literature seemed to indicate using a constant close to the total number of discrete characters you'll encounter is advisable.  I believe this to be somewhat higher than 53, but it would be worth testing on real-world data (source files).
    - #147 Profile this and see if it's reasonable

  ## opengl
    - #84 This only gets used when computing the shadow map, so I'm not even sure if it works ATM
    - #119 Not present on ES2 .. should we use them?
    - #135 Query max gpu textures?
    - #138 32F is only necessary for reprojection of Position for calculating AO.  Consider passing this in when creating a Texture?
    - #150 Should this be done elsewhere?

  ## completeness
    - #84 This only gets used when computing the shadow map, so I'm not even sure if it works ATM
    - #192 This is a function call or macro .. make sure it's actually constant.
    - #205 To do this properly, we need to parse out and expand macro definitions here.
    - #210 This is a function-macro call .. I think always..?
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
    - #137 This _should_ be able to be glTexImage3D, but the driver is throwing an error .. why?!
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
    - #112 This is copy-pasted from the callgraph tests .. should we be able to call this from anywhere?  It's also in the platform layer

  ## copy_paste
    - #112 This is copy-pasted from the callgraph tests .. should we be able to call this from anywhere?  It's also in the platform layer

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
    - #124 Load models in multiple chunks instead of one monolithic one. The storage for chunks must be as large as the largest chunk we will EVER load, which should definately not be decided at compile time.
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
    - #129 Can we somehow atomically pull this one off the queue and initialize it on demand?

  ## gameplay_improvement
    - #129 Can we somehow atomically pull this one off the queue and initialize it on demand?

  ## be_smarter
    - #130 This offset is only used to check if entities are grounded.  Can we do that in a more intelligent way?

  ## not_implemented
    - #131 This path needs to call CanBuildWorldChunkMesh or something similar
    - #132 Pretty sure we can do some XOR trickery or something here to avoid this branch, which could be a large perf win
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
    - #161 Reinstate this!

  ## ui
    - #110 Should we do something special when interacting with this thing instead of Ignored-ing it?

  ## semantic
    - #110 Should we do something special when interacting with this thing instead of Ignored-ing it?

  ## parsing
    - #194 Use bitflags
    - #213 There is a degenerate case here, what if the file ends without a newline? While we're at it, add tests that make sure these functions return sane stuff when files end with comments!
    - #214 There is a degenerate case here, what if the file ends with a malformed comment? While we're at it, add tests that make sure these functions return sane stuff when files end with comments!
    - #192 This is a function call or macro .. make sure it's actually constant.
    - #221 This is a function constructor, let's be done.
    - #200 Floating-point values should be parsed out in TokenizeAnsiStream()!!
    - #189 Parse out function names?
    - #222 Re-add [[nodiscard]] here
    - #230 ParseVariable should work with function pointer types

  ## bug
    - #185 these should be printable!
    - #213 There is a degenerate case here, what if the file ends without a newline? While we're at it, add tests that make sure these functions return sane stuff when files end with comments!
    - #214 There is a degenerate case here, what if the file ends with a malformed comment? While we're at it, add tests that make sure these functions return sane stuff when files end with comments!

  ## id_205
    - #218 See #205
    - #227 See #205
    - #219 See #205
    - #208 See #205
    - #203 See #205
    - #204 See #205
    - #210 This is a function-macro call .. I think always..?

  ## needs_tests
    - #213 There is a degenerate case here, what if the file ends without a newline? While we're at it, add tests that make sure these functions return sane stuff when files end with comments!
    - #214 There is a degenerate case here, what if the file ends with a malformed comment? While we're at it, add tests that make sure these functions return sane stuff when files end with comments!

  ## hack
    - #221 This is a function constructor, let's be done.

  ## todos
    - #225 Rewrite with string_from_parser

  ## output
    - #226 Should we handle this differently?

  ## easy
    - #225 Rewrite with string_from_parser

  ## type_resolution
    - #228 We should keep track of these!  Specifically, it's a struct alias
    - #229 see #228 Generic type alias

  ## id_229
    - #228 We should keep track of these!  Specifically, it's a struct alias

  ## function_parsing
    - #231 Does this make sense to put this into the type?
    - #232 Should this be the name if it's a constructor?

  ## id_104
    - #105 Test this actually gets respected!!

