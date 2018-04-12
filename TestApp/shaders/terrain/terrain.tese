#version 430

layout(quads, equal_spacing, cw) in;

layout(location = 0) in vec3 tcPosition[];
//uniform layout(set = 1, binding = 0, r32f) image2D heightMap;
uniform layout(set = 1, binding = 0) sampler2D heightMapSampler;

layout (set = 0, binding = 0) uniform SceneGlobals{
	mat4 perspective;
	mat4 view;
	vec4 viewPos;
	vec4 sunDirWorld;
	vec4 time;
} uScene;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 p0 = mix(tcPosition[0], tcPosition[1], u);
    vec3 p1 = mix(tcPosition[3], tcPosition[2], u);
    vec3 wPos = mix(p0, p1, v);
    
    wPos.xz *= 1000.0;
    //wPos.y = sin(wPos.x * 1.0);
    
    //wPos.y = imageLoad(heightMap, ivec2(u * 63, v * 63)).r;
   
   wPos.y = texture(heightMapSampler, vec2(u,v)).r;
    gl_Position = uScene.perspective * uScene.view * vec4(wPos, 1);
}