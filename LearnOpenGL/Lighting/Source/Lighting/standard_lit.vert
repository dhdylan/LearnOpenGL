#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoords;

//matrices
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;
flat out vec4 vertexPos;

void main()
{
	vertexPos = normalize(u_projection * u_view * u_model * vec4(aPos, 1.0));
	gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0f);
	normal = normalize(mat3(transpose(inverse(u_model))) * aNormal);
	fragPos = vec3(u_model * vec4(aPos, 1.0f));
    texCoords = aTexCoords;
}