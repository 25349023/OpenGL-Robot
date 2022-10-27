#version 430 core

layout(location = 0) out vec4 fragColor;

void main()
{
	// float dis = distance(gl_FragCoord.xy, screenCenter);
	// float x = clamp(sin(dis * 0.1 + time * -0.1) * 5.0, 0.0, 1.0);
	fragColor = vec4(1.0, 1.0, 0.5, 1.0);
}