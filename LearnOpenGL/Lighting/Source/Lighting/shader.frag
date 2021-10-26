#version 330 core

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D emission_map;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform mat4 u_view;
uniform mat4 u_model;
  
uniform Material material;
uniform Light light;

out vec4 fragColor;

void main()
{
    //ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse_map, texCoords));

    //normalization is kind of redundant here but just for consistency idk
    vec3 normal = normalize(normal);
    vec3 lightDir = normalize(fragPos - vec3(u_view * vec4(light.position, 1.0)));
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectedLightDir = reflect(-lightDir, normal);

    float _dot = pow(max(dot(viewDir, reflectedLightDir), 0.0), material.shininess);
    vec3 specular = light.specular * vec3(texture(material.specular_map, texCoords)) * _dot;

    //use the max function so if the dot product is negative, we just set it to zero.
    //get the dot product of the normal vector and the light direction;
    //the closer these two vectors are to being at a 90 degree angle from eachother, the closer "diff" will be to one
    _dot = max(dot(normal, lightDir), 0.0); // range between 0 and 1
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse_map, texCoords)) * _dot;

    vec3 emission = vec3(texture(material.emission_map, texCoords));

    //point-lighting attenuation
    //--------------------------
    float _distance = length(light.position - fragPos);
    float attenuation = 1.0 / light.constant + light.linear * _distance + light.quadratic * _distance;

    vec3 result = attenuation * (ambient + diffuse + specular);
    fragColor = vec4(result, 1.0);
}