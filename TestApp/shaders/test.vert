#version 430
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(std430, set = 0, binding = 0) buffer Mesh
{
   vec4 points[3];
} mesh;


void main() {

  gl_Position = mesh.points[gl_VertexIndex];

}
