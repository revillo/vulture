#version 430

layout(triangles, equal_spacing, cw) in;

layout(location = 0) in vec3 tcPosition[];


void main()
{
    vec3 p0 = gl_TessCoord.x * tcPosition[0];
    vec3 p1 = gl_TessCoord.y * tcPosition[1];
    vec3 p2 = gl_TessCoord.z * tcPosition[2];
    
    vec3 wPos = normalize(p0 + p1 + p2);
    wPos = p0 + p1 + p2;
    
    
    
    gl_Position = vec4(wPos, 1);
}