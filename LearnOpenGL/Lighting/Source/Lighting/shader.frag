#version 330 core

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D emission_map;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;

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

    vec3 lightToFragDir = normalize(light.position - fragPos); //negative because camera position is just 0,0,0
    float theta = dot(lightToFragDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float spotIntensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    float _dot;
    vec3 diffuse;
    vec3 specular;
    vec3 reflectedLightDir;

    if(theta > light.outerCutoff)
    {
        _dot = dot(lightToFragDir, normal);
        diffuse = light.diffuse * vec3(texture(material.diffuse_map, texCoords)) * _dot;

        reflectedLightDir = reflect(-lightToFragDir, normal);
        _dot = pow(max(dot(-light.direction, reflectedLightDir), 0.0), material.shininess);
        specular = light.specular * vec3(texture(material.specular_map, texCoords)) * _dot;
    }


    //vec3 emission = vec3(texture(material.emission_map, texCoords));

    //point-lighting attenuation
    //--------------------------
    float _distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * _distance + light.quadratic * _distance * _distance);

    vec3 result = ambient + (attenuation * spotIntensity * (diffuse + specular));
    fragColor = vec4(result, 1.0);
}