#version 430

layout(triangles, equal_spacing, cw) in;

layout(location = 0) in vec3 tcPosition[];

layout (set = 0, binding = 0) uniform SceneGlobals{
	mat4 perspective;
	mat4 view;
	vec4 viewPos;
	vec4 sunDirWorld;
	vec4 time;
} uScene;

void main()
{
    vec3 p0 = gl_TessCoord.x * tcPosition[0];
    vec3 p1 = gl_TessCoord.y * tcPosition[1];
    vec3 p2 = gl_TessCoord.z * tcPosition[2];
    
    vec3 wPos = normalize(p0 + p1 + p2);
    wPos = p0 + p1 + p2;
    
  
    gl_Position = uScene.perspective * uScene.view * vec4(wPos, 1);
}