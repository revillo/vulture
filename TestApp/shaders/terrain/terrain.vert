#version 430
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) out vec3 vPosition;

const int sideLength = 64;
const float fSideLength = float(sideLength-1);

vec3 quadVerts[4] = vec3[](

  vec3(0.0, 0.0, 0.0),
  vec3(1.0, 0.0, 0.0),
  vec3(1.0, 0.0, 1.0),
  vec3(0.0, 0.0, 1.0)

);


void main() {

  uint loc = gl_VertexIndex / 4;
  
  vPosition.x = float(loc % sideLength) / fSideLength - 0.5;
  vPosition.z = float(loc / sideLength) / fSideLength - 0.5;
  vPosition.y = 0.0;

  
  vPosition += quadVerts[gl_VertexIndex%4] / fSideLength;
  
}
