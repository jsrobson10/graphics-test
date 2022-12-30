#version 430

out vec4 ColourOut;
in vec3 Colour;
in vec2 TexCoord;

uniform sampler2D Tex;

void main()
{
	ColourOut.rgba = texture(Tex, TexCoord);
}

