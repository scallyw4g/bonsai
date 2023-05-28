# Welcome to Bonsai!

Bonsai is a 3D voxel-based engine built with the intention of writing both
fast-paced arcade games and tile/turn-based RPGs


## Renderer Features

* Deferred Shading
* HDR Lighting
* Shadow Mapping
* Screen Space Ambient Occlusion

## Engine Features

* General Purpose Thread Queue
<!-- * Gameplay Recording/Playback -->
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
* Context Switch Information on Windows

# Wishlist

-------------------------------------------------------------------------------
## Renderer

[ ] TAA

[ ] Transparency : https://www.jcgt.org/published/0002/02/09/paper.pdf

-------------------------------------------------------------------------------
## Sound

[ ] Write an mp3,ogg,..? decompresser



# Building
Ensure you have an OpenGL driver, X11 and clang

## On Linux:

### Ubuntu
`sudo apt install freeglut3-dev libx11-dev`

### Archlinux
`sudo pacman -S freeglut libx11`

### Common

```
git clone --recursive https://github.com/scallyw4g/bonsai bonsai && cd bonsai
./scripts/make.sh
```

## On Windows:
Bonsai requires clang and bash to build on Windows.  git-bash should do the trick.

```
git clone --recursive https://github.com/scallyw4g/bonsai bonsai && cd bonsai
./scripts/make.sh
```
