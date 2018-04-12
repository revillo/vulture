#version 430


layout(vertices = 3) out;

layout(location = 0) in vec3 vPosition[];
layout(location = 0) out vec3 tcPosition[];

#define ID gl_InvocationID

const uint subdiv = 30;

void main()
{
    tcPosition[ID] = vPosition[ID];
    if (ID == 0) {
        gl_TessLevelInner[0] = subdiv;
        gl_TessLevelOuter[0] = subdiv;
        gl_TessLevelOuter[1] = subdiv;
        gl_TessLevelOuter[2] = subdiv;
    }
}