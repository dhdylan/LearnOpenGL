#version 330 core
#define PI 3.1415926538
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

//matrices
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
//positions
uniform vec3 u_lightPos;
uniform vec3 u_cameraPos;
//colors
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;

out vec3 fragColor;

vec3 normal;

vec3 lightPos;
vec3 initialLightDir;
vec3 reflectedLightDir;
vec3 specularResult;
float specularStrength = 0.5;
float specularValue;
vec3 diffuseResult;
float diffuseStrength = 0.5;
float diffuseValue;
vec3 ambientResult;
float ambientStrength = 0.1;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0f);
	normal = mat3(transpose(inverse(u_view * u_model))) * aNormal;
	lightPos = vec3(u_view * vec4(u_lightPos, 1.0));

	//specular
	initialLightDir = normalize(lightPos - vec3(u_view * u_model * vec4(aPos, 1.0)));
	reflectedLightDir = reflect(-initialLightDir, normal);
	specularValue = pow(max(dot(reflectedLightDir, -aPos), 0.0), 32);
	specularResult = specularStrength * specularValue * u_lightColor;
	
	//diffuse
	diffuseValue = max(dot(initialLightDir, normal), 0.0);
	diffuseResult = diffuseStrength * diffuseValue * u_lightColor;

	//ambient
	ambientResult = ambientStrength * u_lightColor;

	fragColor = (ambientResult + diffuseResult + specularResult) * u_objectColor;
}