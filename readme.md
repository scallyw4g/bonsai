
![hero_banner](screenshots/dusk_defence.png)

# Welcome to Bonsai!

Bonsai is a 3D voxel-based engine built with the intention of writing both
fast-paced arcade games and tile/turn-based RPGs

Bonsai, and nearly all it's dependencies, are written completely from scratch.
One external dependency is the C runtime library for startup, and a small
handful of trig functions (sin, cos, atan2).  I have a back-burner task to
remove the CRT entirely, though it's unclear when/if I'll get around to it.

The only requirements to build and run Bonsai are an OpenGL 3.3+ driver, C++
compiler, and a few appropriate system headers.

![banner](screenshots/5_skele.png)

## Renderer Features

* Deferred Shading
* HDR Lighting
* Order-independant Transparency
* Lighting Bloom
* Shadow Mapping
* Screen Space Ambient Occlusion

![banner](screenshots/3_skele.png)

## Engine Features

* Hot Code Reloading
* MT Job System
* Entities
* Collision
* Transparent & Emissive Particles
* UI Framework
* Asset Loaders
* Terrain Generators
* Primitive Physics

![banner](screenshots/profiler.png)

## Performance Profiler Features

* Manual Instrumentation
* Memory allocation tracing
* Per frame call-graph tracing
* Context Switches (windows only)
* Physical Core  (windows only)


# Getting Started

![banner](screenshots/8_skele.png)

## Building

See the docs on the [build process](docs/01_build_process.md).

![banner](screenshots/grass.png)

# Wishlist

-------------------------------------------------------------------------------
## Renderer

[ ] SSR : https://lettier.github.io/3d-game-shaders-for-beginners/screen-space-reflection.html

[ ] Screen-space lines : https://mattdesl.svbtle.com/drawing-lines-is-hard

[ ] Better shadows : https://developer.nvidia.com/gpugems/gpugems3/part-ii-light-and-shadows/chapter-8-summed-area-variance-shadow-maps

[ ] Screen Space Shadows : https://panoskarabelas.com/posts/screen_space_shadows/

[ ] Motion Blur : https://developer.nvidia.com/gpugems/gpugems3/part-iv-image-effects/chapter-27-motion-blur-post-processing-effect

[ ] TAA?

[ ] FXAA : http://blog.simonrodriguez.fr/articles/2016/07/implementing_fxaa.html

[ ] Water : https://www.youtube.com/watch?v=5yhDb9dzJ58

[ ] Fluids : https://andrewkchan.dev/posts/fire.html

[ ] Remove meshing entirely? https://www.youtube.com/watch?v=4xs66m1Of4A

-------------------------------------------------------------------------------
## Terrain Generation

[ ] Erosion simulation
* https://inria.hal.science/hal-01262376/document
* https://xing-mei.github.io/files/erosion.pdf
* https://nickmcd.me/2020/04/15/procedural-hydrology/

[ ] Biomes
* https://en.wikipedia.org/wiki/Holdridge_life_zones

[ ] Meshing
* Isotropic surface meshing
* https://graphics.stanford.edu/courses/cs164-10-spring/Handouts/isotropic.pdf
* https://inria.hal.science/inria-00071612/document

-------------------------------------------------------------------------------
## Sound

[ ] Write an mp3, ogg, ..? decompresser

[ ] Do sound :'D

-------------------------------------------------------------------------------
## Datastructures

[ ] Better low-discrepency sequences : https://blog.demofox.org/2017/05/29/when-random-numbers-are-too-random-low-discrepancy-sequences/

[ ] Better hash function! : https://nullprogram.com/blog/2018/07/31/

[ ] Better GPU hashing! : https://arugl.medium.com/hash-noise-in-gpu-shaders-210188ac3a3e

[ ] Hash-trie as alternative to a table : https://nullprogram.com/blog/2023/09/30/

[ ] Octree ? https://graphics.tudelft.nl/Publications-new/2020/CBE20/ModifyingCompressedVoxels-main.pdf

[ ] Better floating-point rng : https://www.corsix.org/content/higher-quality-random-floats

[ ] Better greedy meshing? https://www.youtube.com/watch?v=4xs66m1Of4A

[ ] More interpolation goodies : https://paulbourke.net/miscellaneous/interpolation/

-------------------------------------------------------------------------------
## Profiler

[ ] Improve the ETW layer : https://github.com/bombomby/optick/blob/master/src/optick_core.win.h
