#version 330 core


in vec4 VS_out_col;
in vec2 VS_out_tCoord;


out vec4 font_surface;


uniform sampler2D font_texture;


void main()
{
	vec4 col = VS_out_col;
	font_surface = vec4(col.rgb, texture(font_texture, VS_out_tCoord).r);
	font_surface *= col.a;
}
