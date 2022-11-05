#version 430 core

in VS_OUT {
	// vec3 vcolor;	
	vec2 texcoord;
} fs_in;

layout(location = 1) uniform sampler2D tex;

layout(location = 0) out vec4 fragColor;

void main()
{
	// float dis = distance(gl_FragCoord.xy, screenCenter);
	// float x = clamp(sin(dis * 0.1 + time * -0.1) * 5.0, 0.0, 1.0);
	// fragColor = vec4(fs_in.vcolor, 1.0);
	fragColor = texture(tex, fs_in.texcoord / 4);

}