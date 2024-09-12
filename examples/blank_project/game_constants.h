
#if 0
global_variable chunk_dimension
g_VisibleRegion = Chunk_Dimension(32, 32, 8);

global_variable chunk_dimension
WORLD_CHUNK_DIM = Chunk_Dimension(32, 32, 8);
#else
global_variable chunk_dimension
/* g_VisibleRegion = Chunk_Dimension(8, 8, 8); */
/* g_VisibleRegion = Chunk_Dimension(16, 16, 16); */
/* g_VisibleRegion = Chunk_Dimension(32, 32, 32); */
/* g_VisibleRegion = Chunk_Dimension(64, 64, 64); */
/* g_VisibleRegion = Chunk_Dimension(128, 128, 128); */ 
g_VisibleRegion = Chunk_Dimension(256, 256, 256); // 1.63km
/* g_VisibleRegion = Chunk_Dimension(1024, 1024, 1024); */

global_variable chunk_dimension
WORLD_CHUNK_DIM = Chunk_Dimension(64, 64, 64);

#endif
