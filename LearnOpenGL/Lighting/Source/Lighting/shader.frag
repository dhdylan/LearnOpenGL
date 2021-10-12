#version 330 core

out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;	
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform mat4 view;
uniform mat4 model;

in vec3 Normal;
in vec3 FragPos;

void main()
{
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    float specularStrength = 0.5;

    //normalization is kind of redundant here but just for consistency idk
    vec3 norm = normalize(Normal);
    vec3 lightDir = vec3(normalize(view * model * vec4(lightPos, 1.0) - view * model * vec4(FragPos, 1.0)));
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectedLightDir = reflect(-lightDir, norm);

    //float spec = pow(max(dot(viewDir, reflectedLightDir), 0.0), 32);
    //vec3 specular = specularStrength * spec * lightColor;  

    //use the max function so if the dot product is negative, we just set it to zero.
    //get the dot product of the normal vector and the light direction;
    //the closer these two vectors are to being at a 90 degree angle from eachother, the closer "diff" will be to one
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.0);
}