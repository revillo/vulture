#version 430
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout(location = 0) out vec3 wPos;
layout(location = 1) out vec3 barys;

void main() {

    barys = vec3(1, 0, 0);
    wPos = gl_in[0].gl_Position.xyz;
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    barys = vec3(0, 1, 0);
    wPos = gl_in[1].gl_Position.xyz;
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    barys = vec3(0, 0, 1);
    wPos = gl_in[2].gl_Position.xyz;
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}