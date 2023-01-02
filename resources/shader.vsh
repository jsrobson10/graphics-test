#version 430
#extension GL_ARB_shader_storage_buffer_object : require

layout(std430, binding=2) buffer ss_data
{
	vec4 SSColour[];
};

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in int vDataPos;

out vec2 TexCoord;
out vec4 Colour;

uniform mat4 camera;
uniform mat4 mat;

void main()
{
	Colour = SSColour[vDataPos];
	TexCoord = vTexCoord;

	gl_Position.xyzw = vec4(vPos, 1) * mat * camera;
}

