#version 430

out vec4 colourOut;
in vec3 colour;

void main()
{
	colourOut.rgb = colour;
	colourOut.a = 1;
}

