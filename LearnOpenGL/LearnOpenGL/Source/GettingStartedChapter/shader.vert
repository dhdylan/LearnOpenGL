#version 330 core
#define PI 3.1415926538
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=1) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vertexColor = normalize(model * vec4(aPos, 1.0));
	texCoord = aTexCoord;
}