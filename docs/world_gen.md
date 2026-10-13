# Terrain Generation & Editing

Terrain generation in Bonsai is composed of several stages.  Terrain generation
stages are configurable by writing regular GLSL shaders, while world edits are
either created using the GUI editor in the engine, or spawned programatically
by the user in C++.

Entity spawning is controlled by writing C++ spawning callbacks.

## Terrain Gen Pipeline Stages

1. Terrain Shaping
2. Terrain Gradient Calculation
3. Terrain Decoration
4. Entity & Edit Spawning
5. Edit applications
6. Meshing
7. Drawing


The terrain Shaping and Decoration stages are configurable by end-users of the
engine.  There is an example project named `terrain_gen` that makes it easy to
switch between different shaping and decoration shaders.

The "Entity & Edit Spawning" stage is also user configurable, and gives the
programmer a chance to spawn assets and entities such as trees, buildings,
NPCs, mobs, etc.  Users register "Spawner" callbacks which fire with the results
supplied from the Decoration stage.

From there, the engine applies any edits the user spawns, meshes the result, and
moves onto drawing.


## Stage 1: Terrain Shaping

The Terrain Shaping stage is typically responsible for the overall shape of the
terrain.  Set the `NoiseValue` and `ColorValue` variables inside the block
marked ` -- user code -- ` to any values you like.  A final value above zero is
solid, a value below zero is air.

To add a new Terrain Shaping shader, copy `shaders/terrain/shaping/default.fragmentshader`
to a new file in the same directory.

## Stage 2: Terrain Gradient Calculation

This stage is not user configurable, but some notes on the implementation
details as they exist currently, for those interested in the details..

Terrain gradients are approximated empirically by doing a convolution on the
output of the Shaping stage.  Doing a convolution on the Shaping output is
somewhat of a middle ground between having analytical gradients, and
calculating the gradients empirically by sampling the "Shaping" function twice.
The practical upside of calculating gradients emprically is that you do not
need to use functions which are differentiable .. your shaping function can be
discontinuous, have holes, sample textures .. whatever.  I chose to do it this
way originally because I wanted to sample textures and still get some
*reasonable* approximation of the overall gradient. I no longer sample textures
in the Shaping stage, but I may support that feature again in the future.  It
is also possible, for performance reasons, that I move to strictly requiring
analytical gradients from the Shaping stage and remove this stage entirely,
which is why I've left if unconfigurable.

## Stage 3: Terrain Decoration

The Decoration stage is typically used to apply small details, coloring, and
texturing to the final terrain.  Think cliff faces for steep slopes, and
smoothing out low-slope areas.

The Decoration stage has the `NoiseValue` and `ColorValue` passed through from
the first Shaping stage, as well as an additional `NormalValue`, which is
the normal to the isosurface at that point.  A future revision of the pipeline
will instead pass a full TBN matrix (Tangent, Bitangent, Normal), with the
Tangent pointing downhill, and the Bitangent pointing cross-slope and the
normal being the same as it is today.

To add a new Decoration shader, copy `shaders/terrain/decoration/default.fragmentshader`
to a new filename in the same directory.


## Stage 4: Entity & Edit Spawning

Spawn callbacks can be appended to the `Engine->ChunkSpawnCallbacks` paged_list
and will be automatically run during generation in the order they're added to the list.

Spawner callbacks specifically take the form:

`b32 SpawnerCallback(engine_resources *Engine, v3i NoiseDim, u32 *NoiseValues, octree_node *Node)`

And can be registered with the engine by doing:

```cpp
BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  chunk_completion_callback Callback = SpawnerCallback;
  Push(&Engine->ChunkCompletionCallbacks, &Callback);
}
```

At the time of this writing, `NoiseDim` will always pass `Assert(NoiseDim == V3i(66))`

Noise values come in packed into opaque u32 values, one value per voxel.  They
can be unpacked into the following structure:

```cpp
enum voxel_data_flags
{
  VoxelData_Empty     = 0,
  VoxelData_Solid     = (1 << 0),
  VoxelData_Collision = (1 << 1),
};

struct voxel_data
{
  voxel_data_flags Flags;
  v3 Normal;
  v3 Color;
};
```

By doing:

```cpp
RangeIterator(x, NoiseDim.x)
RangeIterator(y, NoiseDim.y)
RangeIterator(z, NoiseDim.z)
{
  s32 Index = GetIndex(x,y,z, NoiseDim);
  voxel_data Voxel = UnpackVoxelData(NoiseValues[Index]);
}
```

Currently VoxelData_Solid and VoxelData_Collision are both always set, or
unset.  In a future revision of the pipeline, Shaping and Decoration stages will
have the opportunity to set these independantly for effects like foliage that
produce a mesh, but will not run against collision volumes.

The Level of Detail (LoD) of the terrain generated is available in the octree
node pointer at `Node->Resolution`.  `Resolution == 1` is the highest level of
detail (closest to the game camera).  Units in world space can be calculated by
multiplying the x,y,z values in the unpack loop by `Node->Resolution`

TODO(Jesse): Write docs on spawning Entities and Edits programatically.

For details on spawning Edits using the GUI, see [docs/editor.md](editor.md)



## Stage 5: Edit Applications

TODO(Jesse): Detail the shader pipeline for applying edits

## Stage 6: Meshing

TODO(Jesse): Detail the Meshing algorithm

## Stage 7: Drawing

TODO(Jesse): Add docs on details of the renderer internals ..?
