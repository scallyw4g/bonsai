#include <bonsai_types.h>
#include <game_types.h>
#include <game_constants.h>

global_variable memory_arena *TranArena = AllocateArena();

#include <unix_platform.cpp>
#include <matrix.cpp>
#include <bonsai.cpp>
#include <physics.cpp>
#include <entity.cpp>
#include <bonsai_asset_loaders.h>
