#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuseSpecular;
uniform sampler2D gAmbientShininess;

struct DirLight{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;

	float constant;
	float linear;
	float quadratic;
	float radius;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight{
	vec3 position;
	vec3 direction;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;
};
struct Fog
{
	vec3 color;
	float linearStart;
	float linearEnd;
	float density;
	int equation;
	bool isEnabled;
};



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float getFogFactor(Fog fog, float fogCoordinate);



uniform vec3 viewPos;
#define MAX_NUM_OF_LIGHTS 50
uniform PointLight pointLights[MAX_NUM_OF_LIGHTS];
uniform int numOfPointLights;
uniform SpotLight spotLights[MAX_NUM_OF_LIGHTS];
uniform int numOfSpotLights;
uniform DirLight dirLight;
uniform Fog fog;





vec3 ambientColor;
vec3 diffuseColor;
vec3 specularColor;
float shininess;


void main()
{
	vec3 fragPos = texture(gPosition, TexCoords).rgb;
	vec3 norm = texture(gNormal, TexCoords).rgb;
	vec3 viewDir = normalize(viewPos - fragPos);

	vec4 ambientShininess = texture(gAmbientShininess, TexCoords);
	vec4 diffuseSpecular = texture(gDiffuseSpecular, TexCoords);

	ambientColor = ambientShininess.rgb;
	shininess = ambientShininess.a;

	diffuseColor = diffuseSpecular.rgb;
	specularColor = vec3(1.0, 1.0, 1.0) * diffuseSpecular.a;

	vec3 result = vec3(0.0, 0.0, 0.0);
	result += CalcDirLight(dirLight, norm, viewDir);
	for(int i =0; i< numOfPointLights; i++)
	{
		if(length(pointLights[i].position - fragPos) < pointLights[i].radius)
			result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
	}
	for(int i =0; i< numOfSpotLights; i++)
	{
		result += CalcSpotLight(spotLights[i], norm, fragPos, viewDir);
	}
	vec4 outColor = vec4(result, 1.0);

	if(fog.isEnabled)
	{
		float fogCoordinate = length(fragPos - viewPos);
		outColor = mix(outColor, vec4(fog.color, 1.0), getFogFactor(fog, fogCoordinate));
	}
	FragColor = outColor;
}




vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	//combine results
	vec3 ambient = light.ambient * ambientColor;
	vec3 diffuse = light.diffuse * diff * diffuseColor;
	vec3 specular = light.specular * spec * specularColor;

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = light.ambient * ambientColor;
	vec3 diffuse = light.diffuse * diff * diffuseColor;
	vec3 specular = light.specular * spec * specularColor;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	float theta = dot(normalize(light.position - fragPos), normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// ambient
	vec3 ambient = light.ambient * ambientColor;
	// diffuse
	vec3 norm = normal;
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * diffuseColor;
	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = light.specular * spec * specularColor;

	// attenuation
	float distance = length(light.position - fragPos);
	
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}


float getFogFactor(Fog fog, float fogCoordinate)
{
	float result = 0.0;
	if(fog.equation == 0)
	{
		float fogLength = fog.linearEnd - fog.linearStart;
		result = (fog.linearEnd - fogCoordinate) / fogLength;
	}
	else if(fog.equation == 1)
	{
		result = exp(-fog.density * fogCoordinate);
	}
	else if(fog.equation == 2)
	{
		result = exp(-pow(fog.density * fogCoordinate, 2.0));
	}
	result = 1-clamp(result, 0.0, 1.0);
	return result;
}
