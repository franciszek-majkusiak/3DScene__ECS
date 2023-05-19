#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

uniform mat4 normalMat;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 Tangent;

void main()
{
	vec3 position = aPos;
	gl_Position = projection * view * model * vec4(position, 1.0);
	FragPos = vec3(model * vec4(position, 1.0));
	vec4 normal4 = normalMat * vec4(aNormal, 1.0);
	Normal = vec3(normal4.x, normal4.y, normal4.z);
	TexCoords = aTexCoords;
	Tangent = vec3(model * vec4(aTangent, 1.0));
}