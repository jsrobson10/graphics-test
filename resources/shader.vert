#version 430

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vColour;

uniform mat4 mat;
out vec4 colour;

void main()
{
	gl_Position = vec4(vPos, 1) * mat;
	colour = vec4(vColour, 1);
}

