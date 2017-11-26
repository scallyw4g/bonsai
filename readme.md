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

# Todo and Known Bugs:


-------------------------------------------------------------------------------
## Renderer

[ ] Some sort of Anti-Aliasing

-------------------------------------------------------------------------------
## Particle System

[ ] Turbulance

-------------------------------------------------------------------------------
## World

[ ] There's a collision detection bug when the world is small and a
model is pretty big.  Think 3^3 8^3 chunks with a 16^3 model

[ ] If you're going really fast and try to update your position outside
the VisibleRegion you collide with the edge of the world and stop moving

-------------------------------------------------------------------------------
## Voxel Loader

[ ] MagicaVoxel has an exporter bug and I don't trust it anymore.  Make an
editor mode!

-------------------------------------------------------------------------------
## Sound

[ ] Write an mp3,ogg,..? decompresser



# Building
Ensure you have [CMake](https://cmake.org/download) and g++, clang or MSVC 

## On Linux:

```
git clone https://github.com/jjbandit/bonsai && cd bonsai/build
cmake .
make && make run
```

## On Windows:
Tested on VS2012 && VS2015

- `git clone https://github.com/jjbandit/bonsai`
- Run CMake from the GUI
- Set source code to $BONSAI_DIRECTORY/build
- Set "Where to build the binaries" to $BONSAI_DIRECTORY/bin
- Click "Configure" followed by "Generate" and finally "Open Project"
- Build with Visual Studio `F7` should do it


