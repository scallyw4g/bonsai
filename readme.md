Welcome to the Bonsai engine Readme.

Features:

[X] Shadow Mapping

[X] HDR Lighting

[ ] SSAO


TODO/Bugs:

--- Collision
-------------------------------------------------------------------------------

[ ] FIXME: Collision should place entity flush with colliding volume

--- Renderer
-------------------------------------------------------------------------------

[ ] AO

[ ] Better frustum culling

[ ] Cache which faces are visible so we don't have to check twice

[ ] FIXME: BoundaryVoxel buffer can contain duplicates on the corners and edges

--- World
-------------------------------------------------------------------------------

[ ] Better world!

[ ] FIXME: There's a collision detection bug when the world is small and a
model is pretty big.  Think 3^3 8^3 chunks with a 16^3 model

--- Misc
-------------------------------------------------------------------------------

[ ] Write keypress callbacks instead of checking if (key == down) { do_thing }

[ ] FIXME: If you're going really fast and try to update your position outside
the VisibleRegion you collide with the edge of the world and stop moving
