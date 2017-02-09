--- Collision
-------------------------------------------------------------------------------

[ ] FIXME: Collision should place entity flush with colliding volume


--- Renderer
-------------------------------------------------------------------------------

[ ] Add Shadow maps

[ ] Ambient Occlusion ..?

[ ] Better frustum culling

[X] Normal data should go through pipe relative to vertex instead of being
corrected in shader

[ ] FIXME: BoundaryVoxel buffer can contain duplicates on the corners and edges

[ ] Cache which faces are visible so we don't have to check twice

[X] Add BoundaryVoxels buffer and remove Volume_Boundary voxel flag.


--- World
-------------------------------------------------------------------------------

[ ] Better world!

[X] Model Loading

[ ] FIXME: There's a collision detection bug when the world is small and a
model is pretty big.  Think 3^3 8^3 chunks with a 16^3 model

[X] FIXME: Change world iterations to go from index 0->max instead of all over
the map


--- Misc
-------------------------------------------------------------------------------

[ ] Write keypress callbacks instead of checking if (key == down) { do_thing }

[ ] FIXME: If you're going really fast and try to update your position outside
the VisibleRegion you collide with the edge of the world and stop moving

[X] Make bit-packed voxel positions less hard-coded ( currently chunk size is
indirectly hard-coded to 8*8*8 by bit-shifts that assume 9 bits for position )

[X] Fix Canonicalize to work across multiple chunks
