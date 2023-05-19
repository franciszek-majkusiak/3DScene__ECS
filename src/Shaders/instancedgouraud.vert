#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in mat4 aInstanceMatrix;


out vec4 color;

struct Material{
	bool isUsingDiffuseTexture;
	sampler2D texture_diffuse;
	bool isUsingSpecularTexture;
	sampler2D texture_specular;
	bool isUsingNormalsTexture;
	sampler2D texture_normals;
	float shininess;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emmision;
};

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

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float radius;
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
vec3 CalcBumpedNormal();
float getFogFactor(Fog fog, float fogCoordinate);

uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
#define MAX_NUM_OF_LIGHTS 50
uniform PointLight pointLights[MAX_NUM_OF_LIGHTS];
uniform int numOfPointLights;
uniform SpotLight spotLights[MAX_NUM_OF_LIGHTS];
uniform int numOfSpotLights;
uniform Fog fog;


vec3 FragPos;
vec3 Normal;
vec3 Tangent;
vec2 TexCoords;

vec3 ambientColor;
vec3 diffuseColor;
vec3 specularColor;
vec3 emmisionColor;

void main()
{
	vec3 position = aPos;
	gl_Position = projection * view * aInstanceMatrix * vec4(position, 1.0);
	FragPos = vec3(aInstanceMatrix * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
	TexCoords = aTexCoords;
	Tangent = vec3(aInstanceMatrix * vec4(aTangent, 1.0));


	ambientColor = material.ambient * material.diffuse;
	diffuseColor = material.diffuse;
	specularColor = material.specular;
	emmisionColor = material.emmision;
	if(material.isUsingDiffuseTexture)
	{
		diffuseColor = vec3(texture(material.texture_diffuse, TexCoords));
	}
	if(material.isUsingSpecularTexture)
	{
		specularColor = vec3(texture(material.texture_diffuse, TexCoords));
	}
	vec3 norm  = normalize(Normal);
	if(material.isUsingNormalsTexture)
	{
		norm = CalcBumpedNormal();
	}
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = ambientColor;
	result += CalcDirLight(dirLight, norm, viewDir);
	for(int i =0; i< numOfPointLights; i++)
	{
		if(length(pointLights[i].position - FragPos) < pointLights[i].radius)
			result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	for(int i =0; i< numOfSpotLights; i++)
	{
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
	}
	vec4 outColor = vec4(result, 1.0);

	if(fog.isEnabled)
	{
		float fogCoordinate = length(FragPos - viewPos);
		outColor = mix(outColor, vec4(fog.color, 1.0), getFogFactor(fog, fogCoordinate));
	}
	color = outColor;

}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//combine results
	vec3 ambient = light.ambient * ambientColor;
	vec3 diffuse = light.diffuse * diff * diffuseColor;
	vec3 specular = light.specular * spec * specularColor;

	return /*ambient + */diffuse + specular + emmisionColor;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

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
	return /*ambient + */diffuse + specular + emmisionColor;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	float theta = dot(normalize(light.position - fragPos), normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// ambient
	vec3 ambient = light.ambient * ambientColor;
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * diffuseColor;
	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * specularColor;

	// attenuation
	float distance = length(light.position - fragPos);
	
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return /*ambient + */diffuse + specular + emmisionColor;
}


vec3 CalcBumpedNormal()
{
    vec3 normal = normalize(Normal);
    vec3 tangent = normalize(Tangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    vec3 bumpMapNormal = vec3(texture(material.texture_normals, TexCoords));
    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 newNormal;
    mat3 TBN = mat3(tangent, bitangent, normal);
    newNormal = TBN * bumpMapNormal;
    newNormal = normalize(newNormal);
    return newNormal;
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