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
* Shadow Mapping
* Screen Space Ambient Occlusion

## Engine Features

* General Purpose Thread Queue
* Game Code Hot Reload
* Particles
* Entity System
* Collision
* Primitive Physics
* Various Asset Loaders
* Several Terrain Generators
* Primitive UI framework

## Performance Profiler Features

* Manual Instrumentation
* Memory allocation tracking
* Call graph tracking per frame
* Context Switch & Physical Core tracking on Windows

# Feature Wishlist

-------------------------------------------------------------------------------
## Renderer

[ ] TAA

[ ] Transparency : https://www.jcgt.org/published/0002/02/09/paper.pdf

-------------------------------------------------------------------------------
## Sound

[ ] Write an mp3, ogg, ..? decompresser

[ ] Do sound :'D



# Building

NOTE: The officially supported compiler is clang-15.  Different versions may
work, but may also emit warnings, or errors.  If the following instructions do
not work for you, by all means open an issue and I will do what I can to assist.

## On Windows:
Bonsai requires `clang` and a unix-like shell to build on Windows.  Installing `git-bash` usually works for me.

## On Linux:
Ensure you have an OpenGL driver, GL headers, X11 headers, and clang

### Ubuntu
`sudo apt install freeglut3-dev libx11-dev clang-15`

### Archlinux
`sudo pacman -S freeglut libx11`

### Common

```
git clone --recursive https://github.com/scallyw4g/bonsai bonsai && cd bonsai
./scripts/make.sh
```

