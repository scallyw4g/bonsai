# Terrain Generation & Editing

## Overview

Terrain generation in Bonsai is a multi-stage, GPU-accelerated pipeline that
creates voxel terrain and populates it with entities and decorations. The
system is designed to be flexible and customizable at several points:

- **Shaders** control the shape and appearance of terrain
- **GUI Editor** allows interactive painting and sculpting of terrain
- **C++ Sppawning Callbacks** enable programmatic spawning of entities, NPCs, and decorations
- **Edit System** lets you make permanent or temporary changes to the generated terrain

The `terrain_gen` example project provides a playground for experimenting with
terrain generation. You can modify shaders and see results instantly through
hot-reloading without needing to restart the game.

## Quick Start: Experimenting with Terrain

1. Launch the loader and select the **terrain_gen** example (see [Getting Started](00_getting_started.md))
2. Open `shaders/terrain/shaping/default.user.fragmentshader` in your editor of choice
3. Modify the Shaping_main() function and save your changes; the terrain shader hot-reloads and regenerates the world
4. Once happy with the shape, try modifying `shaders/terrain/decoration/default.user.fragmentshader`
   to add details and texturing.

## Terrain Gen Pipeline Stages

1. Terrain Shaping
2. Terrain Gradient Calculation
3. Terrain Decoration
4. Entity & Edit Spawning
5. Edit applications
6. Meshing
7. Drawing


The terrain Shaping (1) and Decoration (3) stages are configurable by end-users
of the engine through GLSL shader files. The `terrain_gen` example project
makes it easy to switch between different shaping and decoration shaders.  There
are a number of example shaping and decoration shaders which ship with the engine
to get you started.

The "Entity & Edit Spawning" (4) stage is also user configurable, and gives the
programmer a chance to spawn assets and entities such as trees, buildings,
NPCs, mobs, etc.  Users register "Spawner" callbacks which fire with the
results supplied from the Decoration stage.

From there, the engine applies any edits the user spawns, meshes the result,
and moves onto drawing.


## Stage 1: Terrain Shaping

The Terrain Shaping stage defines the overall shape and structure of your terrain.

### How It Works

For each voxel, the shaping shader outputs two values:
- **NoiseValue**: Determines if the voxel is solid (> 0) or air (<= 0)
- **ColorValue**: The base color of the voxel

Edit the code in the Shaping_main() function to control these outputs. You can
use built-in noise functions, sample from a heightmap, or combine multiple
techniques.

### Creating Custom Shaping Shaders

To add a new terrain shaping shader:

1. Copy `shaders/terrain/shaping/default.user.fragmentshader` to a new filename in the same directory
2. Launch the `terrain_gen` example and select your new shader from the menu
3. Modify the shader code in the Shaping_main() function to your liking.
   Changes hot-reload the shader enabling a fast feedback loop.

### Tips

- Combine multiple noise octaves (Fractional Brownian Motion or FBM) for interesting detail
- Use the Y coordinate to control altitude-based variation
- Keep color channels in the range (0-1)

## Stage 2: Terrain Gradient Calculation

This stage automatically calculates the slope and normal direction of your
terrain. The engine uses this information for visual effects, collision
detection, and decoration placement in the next stage.

**You don't need to do anything here** it's handled automatically. The terrain
normals are used by the Decoration stage to apply different effects based on
slope (e.g., rocky cliff faces on steep slopes, smooth grass on gentle slopes).

## Stage 3: Terrain Decoration

The Decoration stage adds details, textures, and visual enhancements to your
terrain based on its slope. This is where you can apply cliff faces to steep
slopes, add smoothing to flat areas, or add variations in color and texture.

### What the Decoration Shader Receives

- **NoiseValue and ColorValue**: Passed through from the Shaping stage
- **NormalValue**: The surface normal at that location

### Using Slope Information

The Normal tells you how steep the terrain is:
- A Normal pointing straight up (0, 0, 1) means flat terrain
- A Normal at an angle means sloped terrain
- Steep slopes have Normals closer to horizontal
- dot(Normal, vec3(0,0,1)) will return 1 when the normal is exactly vec3(0,0,1)
  and -1 when the normal is exactly vec3(0,0,-1).  Using a dot product with a
  vector pointing directly up is a useful way to mask decoration effects.

You can use this to apply different effects:
- Cliffs and rocky textures on steep slopes
- Grass and moss on gentle slopes
- Snow on high elevations

### Creating Custom Decoration Shaders

To add a new decoration shader:


1. Copy `shaders/terrain/decoration/default.user.fragmentshader` to a new file in the same directory
2. Give it a descriptive name (e.g., `mountainous.fragmentshader` or `desert.fragmentshader`)
3. Launch the `terrain_gen` example and select your new shader from the menu
4. Modify the shader code in the Decoration_main() function to your liking.
   Changes hot-reload the shader enabling a fast feedback loop.

### Tips

- Check the dot product of your Normal with the up vector to measure slope steepness
- Layer multiple decorations for complex visual effects


## Stage 4: Entity & Edit Spawning

This is where your game world comes to life. You can spawn NPCs, monsters,
trees, buildings, and other entities programmatically based on the generated
terrain.

### Using Spawner Callbacks

Spawner callbacks let you inspect the generated terrain data (voxels, normals,
colors) and place entities wherever you want.

To register a spawner callback:

```cpp
BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  chunk_completion_callback Callback = YourSpawnerFunction;
  Push(&Engine->ChunkCompletionCallbacks, &Callback);
}
```

Your spawner function receives:
- **Engine**: Access to the engine for spawning entities
- **NoiseDim**: The dimensions of the terrain data
- **NoiseValues**: Packed voxel data for each position
- **Node**: The octree node, including level-of-detail (LoD) information

### Understanding Voxel Data

Each voxel is packed into a u32 value containing:
- **Flags**: Whether the voxel is solid and/or has collision
- **Normal**: The surface normal at that location
- **Color**: The voxel's color

Unpack voxels like this:

```cpp
// NOTE(Jesse): This iteration order ensures a linear scan over memory

RangeIterator(z, NoiseDim.z)
RangeIterator(y, NoiseDim.y)
RangeIterator(x, NoiseDim.x)
{
  s32 Index = GetIndex(x, y, z, NoiseDim);
  voxel_data Voxel = UnpackVoxelData(NoiseValues[Index]);
  
  // Check if this voxel is solid
  if (Voxel.Flags & VoxelData_Solid)
  {
    // You could spawn an entity at this location
  }
}
```

### Level of Detail (LoD)

Different terrain chunks are generated at different distances from the camera for performance:
- `Node->Resolution == 1` is the closest (highest detail)
- Higher Resolution values are farther away (lower detail)
- Use `Node->Resolution` to scale voxel coordinates to world space

### Practical Tips

- Spawn entities at surface level by checking for the transition from air to solid voxels
- Use the color and normal information to decide what type of entity to spawn
- Higher LoD chunks can spawn fewer or larger entities for performance
- For details on spawning Edits using the GUI, see [docs/editor.md](editor.md)



## Stage 5: Edit Applications

After terrain generation, any world edits you've made (either through the GUI
editor or programmatically) are applied to the generated terrain. This allows
you to paint, sculpt, add, or remove terrain features while preserving the
underlying procedural generation.

### Making Edits Permanent

Edits are applied at this stage, so your manual changes combine with the
procedurally generated terrain to create the final result.

### Tips for Editing

- For more information on the GUI editor, see [docs/editor.md](editor.md)

## Stage 6: Meshing

The terrain mesh is generated from all the solid voxels created by the previous stages. This converts the voxel data into a 3D mesh that can be rendered and used for collision detection.

**You don't need to do anything here**, the meshing happens automatically. 

## Stage 7: Drawing

Your finished terrain is rendered on screen. The mesh, colors, and normals from
the previous stages determine how the terrain looks.

**You don't need to do anything here**, the renderer handles this automatically.

## Next Steps

- For interactive terrain editing with brushes, see [docs/editor.md](editor.md)
- Combine shaping and decoration shaders with spawner callbacks for complete world generation
