#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in mat4 aInstanceMatrix;


uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 Tangent;

void main()
{
	vec3 position = aPos;
	gl_Position = projection * view * aInstanceMatrix * vec4(position, 1.0);
	FragPos = vec3(aInstanceMatrix * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
	TexCoords = aTexCoords;
	Tangent = vec3(aInstanceMatrix * vec4(aTangent, 1.0));
}
