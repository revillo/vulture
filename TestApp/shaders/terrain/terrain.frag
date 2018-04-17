#version 430
#extension GL_GOOGLE_include_directive : enable

layout (location = 0) out vec4 OutColor;
layout(location = 0) in vec3 wPos;
layout(location = 1) in vec3 barys;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec2 patchDist;

#include "../globals.glsl"

#include "terrain_constants.glsl"

void main() {
  
    vec3 X = dFdx(wPos.xyz);
    vec3 Y = dFdy(wPos.xyz);
    vec3 normal = normalize(cross(Y,X));    
    
    float pb = 0.01;
    float slope = dot(normal, vec3(0.0, 1.0, 0.0));

    if (wPos.y > 0.0) {
      //Grass
      OutColor = vec4(0.4, 0.6, 0.2, 1.0);
      
      if (wPos.y < 0.02 * TERRAIN_Y_SCALE) {
        
        //Beach
        OutColor = vec4(0.8, 0.8, 0.6, 1.0);
      } else {
        
        
        //Rocks
        if (slope > 0.0 && slope < 0.85){
          OutColor = vec4(0.5, 0.5, 0.35, 1.0);
        }
      }
    
    } else {
      //Water
      OutColor = vec4(0.3, 0.3, 0.7, 1.0);
    }
    
   
    
    //OutColor = vec4(normal * 0.5 + vec3(0.5, 0.5, 0.5), 1.0);
    
    if (patchDist.x < pb || patchDist.y < pb || patchDist.x > 1.0 - pb || patchDist.y > 1.0 - pb) 
    {
     // OutColor = vec4(1.0, 1.0, 1.0, 1.0);
    } else {
      
      if (min(min(barys.x, barys.y), barys.z) < 0.02) {
            //OutColor = vec4(0.0, 0.0, 0.0, 1.0);

      }
    }
    
    
  
    
    OutColor.xyz *= dot(normal, uScene.sunDirWorld.xyz) * 0.5 + 0.5;

}