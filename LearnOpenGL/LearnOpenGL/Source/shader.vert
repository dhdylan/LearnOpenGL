#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;

uniform float xOffset;

out vec3 vertexColor;
out vec3 vertexPosition;

void main()
{
	gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0f);
	vertexColor = aColor;
	
	vertexPosition = vec3(normalize(abs(aPos.x + xOffset)), abs(aPos.y + xOffset), abs(aPos.z + xOffset));
}