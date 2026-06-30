# World Generation

World/Terrain generation in Bonsai is composed of several stages.  Terrain
generation stages are configurable by writing custom GPU shaders, while entity
spawning is controlled by writing CPU-side spawning callbacks.

## Stages

1. Terrain Shaping
2. Terrain Normal Calculation
3. Terrain Decoration
4. Entity Spawning
5. World Edit applications
6. Meshing
7. Drawing


The terrain Shaping and Decoration stages are easily configurable by users of
the engine.  There is an example project named `terrain_gen` that makes it easy
to switch between different shaping and decoration shaders.

The Entity Spawning stage is also user configurable, and gives the programmer
a chance to spawn assets and entities such as trees, buildings, NPCs, mobs, etc.


## Terrain Shaping

The Terrain Shaping stage is typically responsible for the overall shape of the
terrain.  Set the `NoiseValue` and `ColorValue` variables inside the block
marked ` -- user code -- ` to any values you like.  A final value above zero is
solid, a value below zero is air.

To add a new Terrain Shaping shader, copy `shaders/terrain/shaping/default.fragmentshader`
to a new file in the same directory. 


## Terrain Decoration

The Terrain Decoration stage has the `NoiseValue` and `ColorValue` passed through
from the previous Shaping stage, as well as an additional `NormalValue`, which
is the normal to the isosurface at that point.  The Terrain Decoration stage is
typically used to apply small details, coloring, and texturing to the final terrain.

To add a new Terrain Decoration shader, copy `shaders/terrain/decoration/default.fragmentshader`
to a new filename in the same directory.


## Entity Spawning

Spawn callbacks can be appended to the `Engine->ChunkSpawnCallbacks` paged_list
and will be automatically run during generation in the order they're added to the list.

