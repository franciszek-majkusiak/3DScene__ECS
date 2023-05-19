#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 normalMat;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Normal;

void main()
{
	vec3 position = aPos;
	vec4 Position = projection * view * model * vec4(position, 1.0);
	gl_Position = Position;
	Normal = normalMat * vec4(aNormal, 1.0);

}