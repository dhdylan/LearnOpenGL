#version 330 core

layout (location=0) in vec3 aPosColor;

//matrices
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform float scale;

out vec3 PosColor;

void main()
{
	gl_Position = u_projection * u_view * u_model * (scale * vec4(aPosColor, 1.0f));
	PosColor = aPosColor;
}