#version 430


layout(vertices = 4) out;

layout(location = 0) in vec3 vPosition[];

layout(location = 0) out vec3 tcPosition[];

#define ID gl_InvocationID

#define tessLevel 16

uint tessForPoint(vec3 point) {
  return uint(max(1, tessLevel /  floor(length(point) * tessLevel + 1)));
}

void main()
{
    tcPosition[ID] = vPosition[ID];
    
   if (ID == 0) {
    
        uint tess = tessForPoint((tcPosition[0] + tcPosition[2]) * 0.5);
   
        gl_TessLevelInner[0] = tess;
        gl_TessLevelInner[1] = tess;
    }
    
    
    vec3 center = vPosition[(ID) % 4] + vPosition[(ID + 3) % 4];

    gl_TessLevelOuter[ID] = tessForPoint(center * 0.5);
    
    
}