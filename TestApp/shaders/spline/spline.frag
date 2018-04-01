#version 430
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) out vec4 OutColor;
layout(location = 0) in vec3 wPos;
layout(location = 1) in vec3 barys;

void main() {
    
    if (min(min(barys.x, barys.y), barys.z) < 0.05) {
      OutColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
      OutColor = vec4(1.0, wPos.z, 0.0, 1.0);
    }
}