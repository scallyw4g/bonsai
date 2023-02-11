#version 310 es


precision highp float;
precision highp sampler2DShadow;
precision highp sampler2D;
precision highp sampler3D;


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
#define DEBUG_TEXTURE_DIM 512
