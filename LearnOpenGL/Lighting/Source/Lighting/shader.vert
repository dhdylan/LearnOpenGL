#version 330 core
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0f);
	normal = mat3(transpose(inverse(u_view * u_model))) * aNormal;
	fragPos = vec3(u_view * u_model * vec4(aPos, 1.0f));
    texCoords = aTexCoords;
}