#version 330 core

out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;	
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //normalization is kind of redundant here but just for consistency idk
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    //use the max function so if the dot product is negative, we just set it to zero.
    //get the dot product of the normal vector and the light direction;
    //the closer these two vectors are to being at a 90 degree angle from eachother, the closer "diff" will be to one
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.0);
}