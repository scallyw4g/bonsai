#ifndef BONSAI_PLAT_CONTANTS_H
#define BONSAI_PLAT_CONTANTS_H

/*
 *  MSVC specific
 */
#ifdef _MSC_VER

#define GLOBAL_VARIABLE static
#define DEBUG_GLOBAL static

/*
 *  GCC on Linux && Cygwin
 */
#else

// Hush up gcc about unreferenced globals
#define GLOBAL_VARIABLE static __attribute__((unused))
#define DEBUG_GLOBAL static __attribute__((unused))

#endif // _MSC_VER

/*
 *  Common
 */

#define True  1
#define False 0

// Screen Resolution

#define SCREEN_RATIO 2

#define SCR_WIDTH (3840/SCREEN_RATIO)
#define SCR_HEIGHT (2160/SCREEN_RATIO)


/*
 *  Paths
 */
#define MODELS_PATH "models/"
#define SHADER_PATH "shaders/"

#endif // BONSAI_PLAT_CONTANTS_H
