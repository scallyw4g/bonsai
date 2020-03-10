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

[ ] Some sort of Anti-Aliasing

-------------------------------------------------------------------------------
## Particle System

[ ] Turbulance

-------------------------------------------------------------------------------
## Voxel Loader

[ ] MagicaVoxel has an exporter bug and I don't trust it anymore.  Make an
editor mode!

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
git clone https://github.com/jjbandit/bonsai bonsai && cd bonsai
./scripts/make.sh
```

## On Windows:
```
TODO(Jesse, platform_windows): Fix the build.
```
