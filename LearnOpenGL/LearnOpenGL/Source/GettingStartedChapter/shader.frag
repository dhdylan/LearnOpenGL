#version 330 core
in vec4 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D _texture;
uniform sampler2D _texture2;	

void main()
{
	fragColor = mix(texture(_texture, texCoord),  vertexColor, 0.5);
}