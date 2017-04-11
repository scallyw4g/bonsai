Welcome to the Bonsai engine Readme.




Features:

[X] Shadow Mapping

[X] HDR Lighting

[ ] SSAO

[X] Multi-threaded world generation

# Todo and Known Bugs:

-------------------------------------------------------------------------------
## Collision

[ ] FIXME: Collision should place entity flush with colliding volume

-------------------------------------------------------------------------------
## Renderer

[ ] AO

[ ] Better frustum culling

[ ] Cache which faces are visible so we don't have to check twice

[ ] FIXME: BoundaryVoxel buffer can contain duplicates on the corners and edges

-------------------------------------------------------------------------------
## World

[ ] Better world!

[ ] FIXME: There's a collision detection bug when the world is small and a
model is pretty big.  Think 3^3 8^3 chunks with a 16^3 model

-------------------------------------------------------------------------------
## Misc

[ ] Write keypress callbacks instead of checking if (key == down) { do_thing }

[ ] FIXME: If you're going really fast and try to update your position outside
the VisibleRegion you collide with the edge of the world and stop moving





# Build tools

Ensure you have [CMake](https://cmake.org/download) and either g++ or VS2015

# Building

## On Linux:

```
git clone https://github.com/jjbandit/bonsai
cd bonsai/
git submodule init
git submodule update
mkdir build
cmake .
make
```

## On Windows:
NOTE(Jesse): Tested on VS2012, and there are known problems with VS2015

- git clone https://github.com/jjbandit/bonsai
- from `cmd`/`powershell`/`whatever` run `git submodule init` and `git submodule update`
- Run CMake from the GUI
- Set source code to $BONSAI_DIRECTORY 
- Set "Where to build the binaries" to $BONSAI_DIRECTORY/build
- Click "Configure" followed by "Generate" and finally "Open Project"
- Build with Visual Studio `F7` should do it


