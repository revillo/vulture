#version 430
#extension GL_GOOGLE_include_directive : enable
uniform layout(set = 1, binding = 0) sampler2D heightMapSampler;

layout(location = 0) out vec3 vPosition;
layout(location = 1) out vec2 vUV;

const int gridCount = 64;
const float cellWidth = 1.0 / float(gridCount);

vec3 quadVerts[4] = vec3[](

  vec3(0.0, 0.0, 0.0),
  vec3(1.0, 0.0, 0.0),
  vec3(1.0, 0.0, 1.0),
  vec3(0.0, 0.0, 1.0)

);

#include "../globals.glsl"
#include "terrain_constants.glsl"

void main() {

  uint loc = gl_VertexIndex / 4;
  
  vec2 uv = vec2(
    float(loc % gridCount) * cellWidth,
    float(loc / gridCount) * cellWidth
  );
  
  uv += quadVerts[gl_VertexIndex%4].xz * cellWidth;

  //Todo better precision
  
 // vPosition.xz = uv - vec2(0.5, 0.5);
 // vPosition.xz *= 11.5;
 // vPosition.y = 0.0;
  //vPosition.y = texture(heightMapSampler, uv).r;

  vPosition.xyz = terrainLocalToWorld(vec3(uv.x, texture(heightMapSampler, uv).r, uv.y));
  
  vUV = uv;
  
}
