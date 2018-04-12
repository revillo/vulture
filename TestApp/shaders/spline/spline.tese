#version 430

layout(quads, equal_spacing, cw) in;

layout(location = 0) in vec3 tcPosition[];
layout(location = 1) in vec4 tcNormal[];

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

    vec3 p0 = mix(tcPosition[0], tcPosition[3], u);
    vec3 p1 = mix(tcPosition[1], tcPosition[2], u);
    vec3 wPos = mix(p0, p1, v);
    
    
    vec4 n0 = mix(tcNormal[0].xyzw, tcNormal[3].xyzw, u);
    vec4 n1 = mix(tcNormal[1].xyzw, tcNormal[2].xyzw, u);
    vec4 normal = mix(n0, n1, v);
    
    //float normalScale = mix(scale1, scale2, v);
    
   float normalScale = mix(tcNormal[0].w, tcNormal[1].w, v * v * (3.0 - 2.0 * v));
    
    wPos += normalize(normal.xyz) * sqrt(normalScale);
    
    gl_Position = uScene.perspective * uScene.view * vec4(wPos, 1);
}