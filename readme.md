# Welcome to Bonsai!

Bonsai is a 3D voxel-based engine built with the intention of writing both
fast-paced arcade games and tile/turn-based RPGs

Bonsai is has no dependencies that I didn't write completely from scratch.  The
one external dependency is the C runtime library for startup, and a small
handful of trig functions (sin, cos, atan2).  I have a back-burner task to
remove the CRT entirely, though it's unclear when/if I'll get around to it.

The only requirements to build and run Bonsai are an OpenGL 3.3+ driver, C++
compiler, and a few appropriate system headers.


## Renderer Features

* Deferred Shading
* HDR Lighting
* Lighting Bloom
* Shadow Mapping
* Screen Space Ambient Occlusion

## Engine Features

* Hot Code Reload
* MT Job System
* Entities
* Particles
* Collision
* UI Framework
* Asset Loaders
* Terrain Generators
* Primitive Physics

## Performance Profiler Features

* Manual Instrumentation
* Memory allocation tracking
* Call graph tracking per frame
* Context Switch & Physical Core tracking on Windows

# Wishlist

-------------------------------------------------------------------------------
## Renderer

[ ] Transparency : https://www.jcgt.org/published/0002/02/09/paper.pdf

[ ] SSR : https://lettier.github.io/3d-game-shaders-for-beginners/screen-space-reflection.html

[ ] Screen-space lines : https://mattdesl.svbtle.com/drawing-lines-is-hard

[ ] TAA?
[ ] FXAA : http://blog.simonrodriguez.fr/articles/2016/07/implementing_fxaa.html

-------------------------------------------------------------------------------
## Terrain Generation

[ ] Erosion simulation
* https://inria.hal.science/hal-01262376/document
* https://xing-mei.github.io/files/erosion.pdf
* https://nickmcd.me/2020/04/15/procedural-hydrology/

-------------------------------------------------------------------------------
## Sound

[ ] Write an mp3, ogg, ..? decompresser

[ ] Do sound :'D

-------------------------------------------------------------------------------
## Datastructures

[ ] Better low-discrepency sequences : https://blog.demofox.org/2017/05/29/when-random-numbers-are-too-random-low-discrepancy-sequences/

[ ] Hash-trie as alternative to a table : https://nullprogram.com/blog/2023/09/30/

[ ] Octree ? https://graphics.tudelft.nl/Publications-new/2020/CBE20/ModifyingCompressedVoxels-main.pdf

# Building

See the docs on the [build process](docs/01_build_process.md).
