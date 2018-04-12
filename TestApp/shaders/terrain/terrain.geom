#version 430
#extension GL_GOOGLE_include_directive : enable

layout(triangles) in;

layout(triangle_strip, max_vertices = 3) out;


layout(location = 0) in vec2 teUV[];
layout(location = 1) in vec2 tePatchDist[];

layout(location = 0) out vec3 wPos;
layout(location = 1) out vec3 barys;
layout(location = 2) out vec2 uv;
layout(location = 3) out vec2 patchDist;



void main() {

    barys = vec3(1, 0, 0);
    wPos = gl_in[0].gl_Position.xyz;
    uv = teUV[0];
    patchDist = tePatchDist[0];
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    barys = vec3(0, 1, 0);
    wPos = gl_in[1].gl_Position.xyz;
    uv = teUV[1];
    patchDist = tePatchDist[1];
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    barys = vec3(0, 0, 1);
    wPos = gl_in[2].gl_Position.xyz;
    uv = teUV[2];
    patchDist = tePatchDist[2];
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}