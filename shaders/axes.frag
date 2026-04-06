#version 330 core

in vec3 PosColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(PosColor, 1.0);
}