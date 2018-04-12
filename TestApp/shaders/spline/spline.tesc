#version 430


layout(vertices = 4) out;

layout(location = 0) in vec3 vPosition[];
layout(location = 1) in vec4 vNormal[];

layout(location = 0) out vec3 tcPosition[];
layout(location = 1) out vec4 tcNormal[];

#define ID gl_InvocationID

#define tessLevel 4

void main()
{
    tcPosition[ID] = vPosition[ID];
    
    tcNormal[ID] = vNormal[ID];
    
    if (ID == 0) {
        gl_TessLevelInner[0] = tessLevel;
        gl_TessLevelInner[1] = tessLevel;
        gl_TessLevelOuter[0] = tessLevel;
        gl_TessLevelOuter[1] = tessLevel;
        gl_TessLevelOuter[2] = tessLevel;
        gl_TessLevelOuter[3] = tessLevel;
    }
}