#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 position;

void main()
{
	TexCoords = aPos;
	vec4 position = projection * view * vec4(aPos + position, 1.0);
	gl_Position = position.xyww;
}
