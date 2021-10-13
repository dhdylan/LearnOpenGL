#version 330 core
#define PI 3.1415926538
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

//matrices
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform vec3 u_lightPos;

out vec3 lightPos;
out vec3 normal;
out vec3 fragPos;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0f);
	normal = mat3(transpose(inverse(u_view * u_model))) * aNormal;
	fragPos = vec3(u_view * u_model * vec4(aPos, 1.0f));
	lightPos = vec3(u_view * vec4(u_lightPos, 1.0));
}