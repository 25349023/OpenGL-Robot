#version 430 core

layout(location = 0) in vec3 iv3vertex;
layout(location = 1) in vec2 texcoord;

layout(location = 0) uniform mat4 um4mvp;

out VS_OUT {
	vec2 texcoord;
} vs_out;

void main()
{
	gl_Position = um4mvp * vec4(iv3vertex, 1);
	vs_out.texcoord = texcoord;
}