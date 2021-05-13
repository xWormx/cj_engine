#version 330 core


in vec4 VS_out_col;
in vec2 VS_out_tCoord;


out vec4 textured_surface;

uniform sampler2D in_texture;

void main()
{
	vec4 col = VS_out_col;
	textured_surface = texture(in_texture, VS_out_tCoord) * col;
}
