Welcome to the Bonsai engine Readme.




Features:

[X] Shadow Mapping

[X] HDR Lighting

[X] Deferred Shading

[ ] SSAO

[X] Multi-threaded world generation

[X] Looped Game Recording/Playback

[X] Hot Reload

# Todo and Known Bugs:


-------------------------------------------------------------------------------
## Renderer

[ ] SSAO

[ ] Shaders need to be pre-processed for the available version of GLSL


-------------------------------------------------------------------------------
## World

[ ] There's a collision detection bug when the world is small and a
model is pretty big.  Think 3^3 8^3 chunks with a 16^3 model


-------------------------------------------------------------------------------
## Misc

[ ] If you're going really fast and try to update your position outside
the VisibleRegion you collide with the edge of the world and stop moving

-------------------------------------------------------------------------------
## Voxel Loader

[ ] MagicaVoxel is pretty crappy at following its own file format, and I don't
trust it anymore.  Make an editor mode!




# Build tools

Ensure you have [CMake](https://cmake.org/download) and either g++ or MSVC 

# Building

## On Linux:

```
git clone https://github.com/jjbandit/bonsai
cd bonsai/build
cmake .
make
../bin/Bonsai
```

## On Windows:
Tested on VS2012 && VS2015

- `git clone https://github.com/jjbandit/bonsai`
- Run CMake from the GUI
- Set source code to $BONSAI_DIRECTORY/build
- Set "Where to build the binaries" to $BONSAI_DIRECTORY/bin
- Click "Configure" followed by "Generate" and finally "Open Project"
- Build with Visual Studio `F7` should do it


