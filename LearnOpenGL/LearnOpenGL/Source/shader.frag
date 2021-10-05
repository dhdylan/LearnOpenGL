#version 330 core
in vec3 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D _texture;
uniform sampler2D _texture2;

void main()
{
	fragColor = mix(texture(_texture, texCoord), texture(_texture2, vec2(-texCoord.x, texCoord.y)), 0.2);
}