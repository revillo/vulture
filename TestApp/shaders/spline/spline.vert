#version 430
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(std430, set = 0, binding = 0) buffer Mesh
{
   vec4 points[];
} mesh;

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


layout(location = 0) out vec3 vPosition;

void main() {

  vPosition = mesh.points[indices[gl_VertexIndex]].xyz;
  
}
