#version 430

layout(location = 0) in vec2 vPos;
layout(location = 1) in vec3 vColour;

out vec3 colour;

uniform mat4 mat;

void main()
{
	colour = vColour;

	gl_Position.xyzw = vec4(vPos, 0, 1) * mat;
}

