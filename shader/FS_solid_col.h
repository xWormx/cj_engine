#version 330 core

in vec4 VS_out_col;


out vec4 solid_col;


void main()
{

	vec4 v4_col;
	v4_col.r = VS_out_col.r;
	v4_col.g = VS_out_col.g;
	v4_col.b = VS_out_col.b;
	v4_col.a = VS_out_col.a;


	solid_col = v4_col;


}
