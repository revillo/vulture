#version 430
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

struct Vertex {
  vec4 position;
  vec4 normal;
};


layout(std430, set = 1, binding = 0) buffer Mesh
{
   Vertex vertices[];
} mesh;


#define BOX_VERTS 16

uint indices[BOX_VERTS] = uint[](
  0, 4, 7, 3,
  
  3, 7, 6, 2,
  
  2, 6, 5, 1,
  
  1, 5, 4, 0
);


layout(location = 0) out vec3 vPosition;
layout(location = 1) out vec4 vNormal;

void main() {

  uint splineIndex = gl_VertexIndex / BOX_VERTS;
  uint splineStart = splineIndex * 8;
  
  Vertex v = mesh.vertices[indices[gl_VertexIndex % BOX_VERTS] + splineStart];
  
  vPosition = v.position.xyz;
  vNormal = v.normal;

}
