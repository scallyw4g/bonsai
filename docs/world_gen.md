# World Generation

Terrain generation in Bonsai is composed of several stages, some of which are
user reconfigurable.  The stages are as follows:

1. Terrain Shaping
2. Terrain Normal Calculation
3. Terrain Decoration
5. World Edit applications
6. Meshing
7. Drawing


Stages (1) and (3) are easily configurable by users of the engine.  There is an
example project named `terrain_gen` that makes it easy to switch between
different shaping and decoration shaders.


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
is the normal to the surface at that point.  The Terrain Decoration stage is
typically used to apply small details, coloring, and texturing to the final terrain.

To add a new Terrain Decoration shader, copy `shaders/terrain/decoration/default.fragmentshader`
to a new filename in the same directory.


