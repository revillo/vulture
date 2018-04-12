#version 430
#extension GL_GOOGLE_include_directive : enable

layout (location = 0) out vec4 OutColor;
layout(location = 0) in vec3 wPos;
layout(location = 1) in vec3 barys;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec2 patchDist;

#include "../globals.glsl"

void main() {
    vec3 X = dFdx(wPos.xyz);
    vec3 Y = dFdy(wPos.xyz);
    vec3 normal = normalize(cross(X,Y));    
    
    float pb = 0.01;
    OutColor = vec4(uv, wPos.y, 1.0);
    OutColor = vec4(uv, 0.0, 1.0);

    if (patchDist.x < pb || patchDist.y < pb || patchDist.x > 1.0 - pb || patchDist.y > 1.0 - pb) 
    {
      OutColor = vec4(1.0, 1.0, 1.0, 1.0);
    } else {
      
      if (min(min(barys.x, barys.y), barys.z) < 0.02) {
            OutColor = vec4(0.0, 0.0, 0.0, 1.0);

       // OutColor += vec4(0.5, 0.5, 0.2, 0.0);
      }
    }
    
    OutColor.xyz *= dot(normal, uScene.sunDirWorld.xyz) * 0.5 + 0.5;

}