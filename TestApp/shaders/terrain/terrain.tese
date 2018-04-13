#version 430
#extension GL_GOOGLE_include_directive : enable

layout(quads, equal_spacing, cw) in;

layout(location = 0) in vec3 tcPosition[];
layout(location = 1) in vec2 tcUV[];

layout(location = 0) out vec2 teUV;
layout(location = 1) out vec2 tePatchDist;
layout(location = 2) out vec3 teWPos;

#include "../globals.glsl"


uniform layout(set = 1, binding = 0) sampler2D heightMapSampler;

void main()
{
    float bx = gl_TessCoord.x;
    float by = gl_TessCoord.y;

    vec3 p0 = mix(tcPosition[0], tcPosition[1], bx);
    vec3 p1 = mix(tcPosition[3], tcPosition[2], bx);
    teWPos = mix(p0, p1, by);
    
    
    vec2 uv0 = mix(tcUV[0], tcUV[1], bx);
    vec2 uv1 = mix(tcUV[3], tcUV[2], bx);
    vec2 globalUV = mix(uv0, uv1, by);
    
    tePatchDist = vec2(bx,by);
    
    teWPos.y = texture(heightMapSampler, globalUV).r;

    
    teUV = globalUV;
    
    gl_Position = uScene.perspective * uScene.view * vec4(teWPos, 1);
}