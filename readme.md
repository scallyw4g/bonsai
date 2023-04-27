# Welcome to Bonsai!

Bonsai is a 3D voxel-based engine built with the intention of writing
fast-paced arcade style games.



## Renderer Features

* Deferred Shading
* HDR Lighting
* Shadow Mapping
* Screen Space Ambient Occlusion

## Engine Features

* General Purpose Thread Queue
* Gameplay Recording/Playback
* Hot Code Reloading
* Particle System
* Entity System
* Asset Loader

## Performance Profiler Features

* Memory allocation tracking
* Call graph tracking per frame (routine % of frame, CPU cycles, # of calls)
* Works across all threads

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
git clone --recursive https://github.com/jjbandit/bonsai bonsai && cd bonsai
./scripts/make.sh
```

## On Windows:
Bonsai requires clang and bash to build on Windows.  git-bash should do the trick.

```
git clone --recursive https://github.com/jjbandit/bonsai bonsai && cd bonsai
./scripts/make.sh
```
