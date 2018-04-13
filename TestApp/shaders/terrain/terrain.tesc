#version 430
#extension GL_GOOGLE_include_directive : enable

layout(vertices = 4) out;

layout(location = 0) in vec3 vPosition[];
layout(location = 1) in vec2 vUV[];

layout(location = 0) out vec3 tcPosition[];
layout(location = 1) out vec2 tcUV[];

#define ID gl_InvocationID

#define tessLevel 8

#include "../globals.glsl"


uint tessForPoint(in vec3 point) {
  
  float dist = length(point - uScene.viewPos.xyz) / 20.0;
  //float distinv = 8.0 / (length(point - uScene.viewPos.xyz) + 1);
  //float dist = length(point) / 5.0;
  
  //return uint(max(1, distinv));
  return uint(max(1, floor(tessLevel * (1.0 - dist) + 0.5)));
}

void main()
{
    tcPosition[ID] = vPosition[ID];
    tcUV[ID] = vUV[ID];
    
    
   if (ID == 0) {
    
        uint tess = tessForPoint((vPosition[0] + vPosition[2]) * 0.5);
   
        gl_TessLevelInner[0] = tess;
        gl_TessLevelInner[1] = tess;
 
    }
    
    
    vec3 center = vPosition[(ID) % 4] + vPosition[(ID + 3) % 4];

    gl_TessLevelOuter[ID] = tessForPoint(center * 0.5);
   
}