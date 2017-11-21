#version 310 es


precision highp float;
precision highp sampler2DShadow;


#define v3 vec3
#define v2 vec2


#define MAX_LIGHTS 32
struct light
{
  int Type;
  v3 Position;
} Lights[MAX_LIGHTS];


#define USE_AO_SHADER 1
#define USE_SHADOW_MAPPING 0

