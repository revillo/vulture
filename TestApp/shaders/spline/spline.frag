#version 430
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) out vec4 OutColor;
layout(location = 0) in vec3 wPos;
layout(location = 1) in vec3 barys;
layout (set = 0, binding = 0) uniform SceneGlobals{
	mat4 perspective;
	mat4 view;
	vec4 viewPos;
	vec4 sunDirWorld;
	vec4 time;
} uScene;


void main() {
    vec3 X = dFdx(wPos.xyz);
    vec3 Y = dFdy(wPos.xyz);
    vec3 normal = normalize(cross(X,Y));    
    
    if (min(min(barys.x, barys.y), barys.z) < 0.05) {
      OutColor = vec4(0.2, 0.2, 0.2, 1.0);
    } else {
      OutColor = vec4(0.8, 0.8, 0.8, 1.0);
    }
    
    OutColor.xyz *= dot(normal, uScene.sunDirWorld.xyz) * 0.5 + 0.5;
}