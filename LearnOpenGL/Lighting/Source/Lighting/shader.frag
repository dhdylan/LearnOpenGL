#version 330 core

out vec4 fragColor;

uniform mat4 u_view;
uniform mat4 u_model;

in vec3 normal;
in vec3 fragPos;
in vec3 lightPos;

uniform vec3 u_cameraPos;
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;

void main()
{
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;
    float specularStrength = 0.5;

    //normalization is kind of redundant here but just for consistency idk
    vec3 normal = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectedLightDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectedLightDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColor;  

    //use the max function so if the dot product is negative, we just set it to zero.
    //get the dot product of the normal vector and the light direction;
    //the closer these two vectors are to being at a 90 degree angle from eachother, the closer "diff" will be to one
    float diff = max(dot(normal, lightDir), 0.0); // range between 0 and 1
    vec3 diffuse = diff * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor;
    fragColor = vec4(result, 1.0);
}