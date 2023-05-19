#version 460 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gDiffuseSpecular;
layout (location = 3) out vec4 gAmbientShininess;

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

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Tangent;


uniform Material material;

vec3 CalcBumpedNormal();


vec3 ambientColor;
vec3 diffuseColor;
vec3 specularColor;
vec3 emmisionColor;


void main()
{
	ambientColor = material.ambient;
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

	gPosition = FragPos;
	gNormal = norm;
	gDiffuseSpecular.rgb = diffuseColor;
	gDiffuseSpecular.a = (specularColor.x + specularColor.y + specularColor.z) / 3.0;

	gAmbientShininess.rgb = ambientColor;
	gAmbientShininess.a = material.shininess;

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