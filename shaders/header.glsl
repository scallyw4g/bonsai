#version 310 es


precision highp float;
precision highp sampler2DShadow;


#define v4 vec4
#define v3 vec3
#define v2 vec2

#define V4 vec4
#define V3 vec3
#define V2 vec2

#define r32 float
#define u32 unsigned int
#define s32 int


// THIS MUST MATCH THE DEFINE IN constants.hpp
#define MAX_LIGHTS 1024
struct light
{
  v3 Position;
  v3 Color;
};


#define USE_AO_SHADER 1
#define USE_SHADOW_MAPPING 0

