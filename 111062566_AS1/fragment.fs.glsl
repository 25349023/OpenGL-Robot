#version 430 core

in VS_OUT {
	vec2 texcoord;
} fs_in;

layout(location = 1) uniform sampler2D tex;

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = texture(tex, fs_in.texcoord / 4);
}