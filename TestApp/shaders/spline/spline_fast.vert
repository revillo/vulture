#version 430
#extension GL_GOOGLE_include_directive : enable
#include "../globals.glsl"

struct Vertex {
  vec4 position;
  vec4 normal;
};

layout(std430, set = 1, binding = 0) buffer Mesh
{
   Vertex vertices[];
} mesh;


#define BOX_VERTS 24

uint indices[24] = uint[](
  0, 4, 7,
  0, 7, 3,
  3, 7, 6,
  3, 6, 2,
  2, 6, 1,
  1, 6, 5,
  1, 5, 0,
  0, 5, 4
);



//layout(location = 0) out vec3 vPosition;
//layout(location = 1) out vec4 vNormal;

void main() {

  uint splineIndex = gl_VertexIndex / BOX_VERTS;
  uint splineStart = splineIndex * 8;
  
  Vertex v = mesh.vertices[indices[gl_VertexIndex % BOX_VERTS] + splineStart];
  //vPosition = v.position.xyz;
  //vNormal = v.normal;

  vec4 pos = vec4(v.position.xyz, 0.0);
  vec4 norm = vec4(normalize(v.normal.xyz), 0.0) * v.normal.w;
  
  gl_Position = uScene.perspective * uScene.view * (v.position + norm);
  
}
