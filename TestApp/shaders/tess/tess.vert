#version 430
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

/*
layout(std430, set = 0, binding = 0) buffer Mesh
{
   vec4 points[3];
} mesh;
*/

layout(location = 0) in vec4 position;

layout(location = 0) out vec3 vPosition;

void main() {

  vPosition = position.xyz;

}
