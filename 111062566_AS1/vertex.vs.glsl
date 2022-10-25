#version 410 core

layout(location = 0) in vec3 iv3vertex;

uniform mat4 um4mvp;

void main()
{
	gl_Position = um4mvp * vec4(iv3vertex, 1);
}