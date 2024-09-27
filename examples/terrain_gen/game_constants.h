
#if 0
global_variable chunk_dimension
g_VisibleRegion = Chunk_Dimension(32, 32, 8);

global_variable chunk_dimension
WORLD_CHUNK_DIM = Chunk_Dimension(32, 32, 8);

#else

global_variable chunk_dimension
/* g_VisibleRegion = Chunk_Dimension(8192, 8192, 8192); */
/* g_VisibleRegion = Chunk_Dimension(4096, 4096, 4096); */ // 26km
/* g_VisibleRegion = Chunk_Dimension(2048, 2048, 2048); // 13.11 km */
g_VisibleRegion = Chunk_Dimension(512, 512, 512); //  3.28 km
/* g_VisibleRegion = Chunk_Dimension(256, 256, 256); */
/* g_VisibleRegion = Chunk_Dimension(128, 128, 128); */
/* g_VisibleRegion = Chunk_Dimension(64, 64, 64); */
/* g_VisibleRegion = Chunk_Dimension(32, 32, 32); */
/* g_VisibleRegion = Chunk_Dimension(16, 16, 16); */
/* g_VisibleRegion = Chunk_Dimension(8, 8, 8); */
/* g_VisibleRegion = Chunk_Dimension(4, 4, 4); */
/* g_VisibleRegion = Chunk_Dimension(2, 2, 2); */



global_variable chunk_dimension
WORLD_CHUNK_DIM = Chunk_Dimension(64, 64, 64);

#endif
