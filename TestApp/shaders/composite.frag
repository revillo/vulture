#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec2 uv;

layout (location = 0) out vec4 OutColor;

layout (set = 0, binding = 0) uniform sampler2D colorSampler;

float kernelSize = 6;

void main() {
    
    OutColor = texture(colorSampler, uv);
    
    if (OutColor.a < 0.1) {
      OutColor = vec4(0.5, 0.5, 0.7, 1.0);
    }
    OutColor.a = 1.0;

}