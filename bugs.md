# Jan 10 2019 - 8:00 - closed - FlushBuffersToCard not calling glEnableVertexAttribArray
* opengl
* gpu-mapped-element-buffer
* nsight
I had an incomplete understanding of what exactly glEnableVertexAttribArray did
and as such failed to call it when unmapping vertex buffers for a new render to
texture path.  The other path happened to work because no other render calls
fall between the start and end of the buffer map-unmap pair.  Used nsight to
finally figure out what the issue was.

# Jan 8 2019 - 3:00 - closed - Unproject error due to column-vs-row major matrices
* inverse-projection
* matrix
* math
* opengl
Opengl matrices are column-major and the Bonsai application matrices are
row-major.  Confused by this when doing a row-major tranform on a point did not
produce the correct result.  Ended up remembering that opengl matrices are
column-major, implemented the opposite transform, and everything worked.


# Jun 4 2018 - 1:00 - closed - Collision detection offset weirdness
* collision detection
The GetCollision call had some old/crappy code that was calculating the
wrong bounds for the collision check.  Changing to a more sane inclusive
min exclusive max scenario fixed the issue.

# Jun 1 2018 - 5:00 - closed - Memory arenas rewinding to a bad point
* memory
* arenas
* allocation
Memory arena Start pointers were recently changed to be the first byte of the
entire system allocation, which when reset to, caused the actual arena to be
overwritten.  Adding a secondary pointer to the first byte used for the
allocations fixed the issue;

# May 29 2018 - 0:00 - closed - Geometry buffer over-allocated by 1
* off-by-one
* allocation
I noticed that geometry buffers were actually setting their End pointer to one
past the end of the actual memory.  I assume this never got written to (they're
memprotected) because we operate on triangles, thus access elements three at a
time, and one was not enough to cause an overwrite crash.

# May 29 2018 - 0:40 - closed - Using extra page on arena reallocation
* memory
* arenas
* allocator
This manifested when trying to initially allocate an arena with a very small
size (32 bytes).  Probably should not have been trying to allocate that little
of an arena in the first place, but it caught a bug.

# May 29 2018 - 7:00 - closed - Overflowing page table on linux
* memory
* allocator
On linux, memprotecting pages adds an entry to the virtual page table, which
eventually overflows with _many_ small allocations.  Turning off memprotect on
the transient arena fixes the issue for now.

# Apr 30 2018 - 2:00 - closed - Generic macro for copying color buffers
* opengl
* memory
* macro
Had an issue when sending data to the card.  Added an emission property to the
color data, which increased the stride from 3 floats to 4.  The stride value
was hard-coded, which resulted in a corruput-looking draw.

# Apr 18 2018 - 2:00 - closed - Socket blocking instead of non-blocking
* network
* init
Server and client sockets are created differently, which caused my assumption
that they were non-blocking to be incorrect.  I thought the server was hanging
on a infinite recv loop when in fact it was blocking on recv

# Mar 12 2018 - 18:00 - closed - Ghost geometry
* lifecycle
* concurrency
I was setting the chunk flag to build a mesh when a chunk was actually
un-initialized, and the thread queue was occationally getting to it before the
voxels were initialized.  This caused a bogus mesh to be built.

# Nov 30 2017 - 2:00 - closed - Fix clock bugs
* clock
I was dividing by an integer instead of an r64, which caused the clock to get
trucated on second boundaries. Whups

# Nov 29 2017 - 1:00 - closed - Bad reinitialization of debug state
* lifecycle
* init
CleanupScopeTree reinitialized the CurrentScope param to 0 when it should have
been &DebugState->RootScope

# Nov 23 2017 - 0:30 - closed - void ptr parameter
* texture
I was passing a void ptr to the MakeTexture_RGB to initialize the tex data,
which bit me in the ass when I went from passing the address of a
stack-allocated array to the address of a heap allocated array, which was v3**
instead of a stack allocated array degrading to v3*

# Sept 26 2017 - 4:00 - closed - Extra texture being bound
* opengl
* init
* lifecycle
* texture
I had an old depth texture which was not needed in the shader but was being
bound to its default uniform value of 0.  Very bad

# Sept 22 2017 - 2:00 - closed - Shadow Mapping
* opengl
* texture
* shadow mapping
I was not setting the shadowMap uniform in the lighting shader and it was
sampling the wrong texture.  FML.  I should probably make more type-safe
wrapped opengl calls

# Aug 31 2017 - 1:00 - closed - Screen resize bug
* opengl
* texture
I was setting the gbuffer texture size at startup to the width/height of the
screen, but then when drawing into it, reset the viewport size to the current
size of the screen, which could have changed if the user resized it. No bueno.

# Aug 28 2017 - 11:00 - closed - Fix shadow mapping on other hardware
* opengl
* shadow mapping
Somehow on my Lenovo laptop the scene depth texture was linear, however on
other hardware it was not.  This is a complete mystery and somewhat alarming.
I still have yet to investigate the root cause.

# Aug 21 2017 - 4:00 - closed - Rendering capabilities in the platform layer
* debug system
In order to properly time functions, the entire debug system needed to exist
outside of the game code (specifically, any timed functions).  The only place
where it could live is the platform layer, however it also needs to draw to the
screen, so the rendering code had to be lofted up into the platform layer.  3h
of refactoring and a while figuring out what was going wrong.

# Aug 13 2017 - 0:30 - closed - Actually use the new Destroyed state
* entity state
* lifecycle

# Aug 13 2017 - 0:30 - closed - Put player into entity table
* player
* entity system
* collision detection
Player update and the rest of entity update code was not exactly the same, thus
there was a chance for entities to tunnel through the Player

# Aug 13 2017 - 0:45 - closed - Fix PushFrameEvent
* frame queue
Had some bad logic to step through a linked list I clearly didn't test

# Aug 10 2017 - 10:00 - closed - Fix shadow mapping
* opengl
* shadow mapping
Epic quest to fix the shadow mapping turned out to be mostly a wild goose chase
and a couple hours tuning it at the end.  The wild goose chase part was trying
to translate the world geometry using a transform matrix - which I still think
should have worked - instead of repositioning the projection matrix with an
offset.  The other part was changing the shader to use sampler2d instead of
sampler2dShadow, which I'm not sure was a good idea, but I felt like doing it.

# Aug 1 2017 - 0:15 - closed - Systems sometimes unspawning early
* particle system
Parent entities sometimes got to the end of the world before the whole effect
had time to complete, causing it to disappear in a jarring fashon.

# Aug 1 2017 - 1:00 - closed - Inconsistent physics update.
* physics
* frame rate
Introduced during refactor with variable frame rate

# Jul 30 2017 - 1:00 - closed - Globals causing problems; moved into game_state
* globals
* hot reload

# Jul 30 2017 - 0:45 - closed - GameThreadCallback not refreshed on reload
* lifecycle
* hot reload

# Jul 24 2017 - 3:00 - closed - Some entries getting skipped
* thread queue
Problem introduced going from checking the thread queue for work -> semaphore
only for flow control.  Whenever AtomicCompareExchange failed, ThreadSleep
would be called again before ACE was retried, thus leaving the Semaphore out of
sync with the number of entries on the queue.

# Jul 24 2017 - 0:25 - closed - buffer overrun in frame queue
* frame queue

# Jul 24 2017 - 0:05 - closed - Reinitialization error
* particle system

# Jul 24 2017 - 0:05 - closed - frame rate higher than normal; no vsync
* particle system
* frame rate

