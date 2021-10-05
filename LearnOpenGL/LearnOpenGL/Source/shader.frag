#version 330 core
in vec4 vertexColor;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D _texture;
uniform sampler2D _texture2;
uniform float time;
uniform float deltaOpacity;

void main()
{
	fragColor = mix(texture(_texture, mix(vec2(0.5, 0.5), texCoord/2, sin(time)/2 + .5)), texture(_texture2, vec2(-texCoord.x, texCoord.y)), 0.5 + deltaOpacity);
}