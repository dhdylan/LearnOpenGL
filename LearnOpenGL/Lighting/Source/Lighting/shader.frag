#version 330 core

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D emission_map;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float innerCutoff;
    float outerCutoff;

    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

vec3 DirLight_calc(DirLight light, vec3 normal, vec3 viewDirection);
vec3 PointLight_calc(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 SpotLight_calc(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform mat4 u_view;
uniform mat4 u_model;

//camera position
uniform vec3 u_viewPos;

//lights
uniform DirLight u_dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight u_pointLight[NR_POINT_LIGHTS];
uniform SpotLight u_spotLight;

uniform Material u_material;

out vec4 fragColor;

void main()
{
    vec3 viewDir = normalize(u_viewPos - fragPos);

    //directional lighting
    vec3 result = DirLight_calc(u_dirLight, normal, viewDir);

    //point lighting
    for(int i=0; i<NR_POINT_LIGHTS; i++)
    {
        result += PointLight_calc(u_pointLight[i], normal, fragPos, viewDir);
    }

    //spot lighting
    result += SpotLight_calc(u_spotLight, normal, fragPos, viewDir);

    fragColor = vec4(result, 1.0);
}

vec3 DirLight_calc(DirLight light, vec3 normal, vec3 viewDir)
{
    //diffuse lighting
    float diffuseIntensity = max(dot(-normalize(light.direction), normal), 0.0);
    //speclar lighting
    vec3 reflectedLightDir = reflect(-normalize(light.direction), normal);
    float specularIntensity = pow(max(dot(reflectedLightDir, viewDir), 0.0), u_material.shininess);

    vec3 ambient  = light.ambient  * vec3(texture(u_material.diffuse_map, texCoords));
    vec3 diffuse  = light.diffuse  * diffuseIntensity * vec3(texture(u_material.diffuse_map, texCoords));
    vec3 specular = light.specular * specularIntensity * vec3(texture(u_material.specular_map, texCoords));

    return ambient + diffuse + specular;
}

vec3 PointLight_calc(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightToFragDir = normalize(light.position - fragPos);
    float _distance = length(light.position - fragPos);    
    float attenuation = 1.0 / (light.constant + light.linear * _distance + light.quadratic * _distance * _distance);

    //diffuse lighting
    float diffuseIntensity = max(dot(lightToFragDir, normal), 0.0) * attenuation;
    //specular lighting
    vec3 reflectedLightDir = reflect(lightToFragDir, normal);
    float specularIntensity = pow(max(dot(reflectedLightDir, -viewDir), 0.0), u_material.shininess) * attenuation;

    vec3 diffuse = light.diffuse * diffuseIntensity * vec3(texture(u_material.diffuse_map, texCoords));
    vec3 specular = light.specular * specularIntensity * vec3(texture(u_material.specular_map, texCoords));

    return diffuse + specular;
}

vec3 SpotLight_calc(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightToFragDir = normalize(light.position - fragPos);

    //basic diffuse and specular calculations calculations
    vec3 diffuse = max(dot(lightToFragDir, normal), 0.0) * light.diffuse * vec3(texture(u_material.diffuse_map, texCoords));
    vec3 reflectedLightDir = reflect(-lightToFragDir, normal);
    vec3 specular = pow(max(dot(reflectedLightDir, viewDir), 0.0), u_material.shininess) * light.specular * vec3(texture(u_material.specular_map, texCoords));

    //distance attenuation calculations
    float _distance = length(light.position - fragPos);    
    float attenuation = 1.0 / (light.constant + light.linear * _distance + light.quadratic * _distance * _distance);
    diffuse *= attenuation;
    specular *= attenuation;
    
    //spotlight angle calculations    
    float theta = dot(lightToFragDir, -light.direction);
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    return diffuse + specular;
}