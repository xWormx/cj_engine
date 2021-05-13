#version 330 core


layout (location = 0) in vec2 in_Pos;
layout (location = 1) in vec4 in_Col;
layout (location = 2) in vec2 in_tCoord;


out vec4 VS_out_col;
out vec2 VS_out_tCoord;


void main()
{

	vec4 v4_pos;
	v4_pos.x = in_Pos.x - 1.0f;
	v4_pos.y = in_Pos.y - 1.0f;
	v4_pos.z = 0.0f;
	v4_pos.w = 1.0f;

	gl_Position = v4_pos;
	VS_out_col = in_Col;
	VS_out_tCoord = in_tCoord;


}
