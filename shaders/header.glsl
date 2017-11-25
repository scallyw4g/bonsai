#version 310 es


precision highp float;
precision highp sampler2DShadow;


#define v4 vec4
#define v3 vec3
#define v2 vec2


// THIS MUST MATCH THE DEFINE IN constants.hpp
#define MAX_LIGHTS 32
struct light
{
  int Type;
  v3 Position;
} Lights[MAX_LIGHTS];


#define USE_AO_SHADER 0
#define USE_SHADOW_MAPPING 0

