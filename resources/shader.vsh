#version 430

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vColour;
layout(location = 2) in vec2 vTexCoord;

out vec3 Colour;
out vec2 TexCoord;

uniform mat4 camera;
uniform mat4 mat;

void main()
{
	Colour = vColour;
	TexCoord = vTexCoord;

	gl_Position.xyzw = vec4(vPos, 1) * mat * camera;
}

