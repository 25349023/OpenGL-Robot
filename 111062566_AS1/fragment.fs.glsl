#version 410 core

layout(location = 0) out vec4 fragColor;

uniform float time;
uniform vec2 screenCenter;

void main()
{
	float dis = distance(gl_FragCoord.xy, screenCenter);
	float x = clamp(sin(dis * 0.1 + time * -0.1) * 5.0, 0.0, 1.0);
	fragColor = vec4(vec3(x), 1.0);
}