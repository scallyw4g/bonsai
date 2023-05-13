#version 310 es
#extension GL_NV_shader_buffer_load : enable


precision highp float;
precision highp sampler2DShadow;
precision highp sampler2D;
precision highp sampler3D;

#define f32_MAX (1E+37f)
#define f32_MIN (1E-37f)

#define v4 vec4
#define v3 vec3
#define v2 vec2

#define V4 vec4
#define V3 vec3
#define V2 vec2

#define r32 float
#define u32 unsigned int
#define s32 int

#define USE_SSAO_SHADER 1
#define USE_SHADOW_MAPPING 1

// Note(Jesse): Must match corresponding C++ define
#define SHADOW_MAP_RESOLUTION_X (16*1024)
#define SHADOW_MAP_RESOLUTION_Y (16*1024)

// Note(Jesse): Must match corresponding C++ define
#define DEBUG_TEXTURE_DIM 512

// Note(Jesse): Must match corresponding C++ define
#define SCREEN_RATIO (1)
#define SCR_WIDTH (3840/SCREEN_RATIO)
#define SCR_HEIGHT (2160/SCREEN_RATIO)

#define PoissonDiskSize 16
vec2 poissonDisk[PoissonDiskSize] = vec2[](
   vec2( -0.94201624, -0.39906216 ),
   vec2(  0.94558609, -0.76890725 ),
   vec2( -0.09418410, -0.92938870 ),
   vec2(  0.34495938,  0.29387760 ),
   vec2( -0.91588581,  0.45771432 ),
   vec2( -0.81544232, -0.87912464 ),
   vec2( -0.38277543,  0.27676845 ),
   vec2(  0.97484398,  0.75648379 ),

   vec2(  0.44323325, -0.97511554 ),
   vec2(  0.53742981, -0.47373420 ),
   vec2( -0.26496911, -0.41893023 ),
   vec2(  0.79197514,  0.19090188 ),
   vec2( -0.24188840,  0.99706507 ),
   vec2( -0.81409955,  0.91437590 ),
   vec2(  0.19984126,  0.78641367 ),
   vec2(  0.14383161, -0.14100790 )
);

// TODO(Jesse): Metaprogram these.  I've already got them in the C++ math lib
float MapValueToRange(float value, float inMin, float inMax, float outMin, float outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

vec2 MapValueToRange(vec2 value, vec2 inMin, vec2 inMax, vec2 outMin, vec2 outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

vec3 MapValueToRange(vec3 value, vec3 inMin, vec3 inMax, vec3 outMin, vec3 outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

vec4 MapValueToRange(vec4 value, vec4 inMin, vec4 inMax, vec4 outMin, vec4 outMax) {
  return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}
