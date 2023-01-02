#version 430

uniform sampler2D Tex;
out vec4 ColourOut;
in vec2 TexCoord;
in vec4 Colour;

void main()
{
	ColourOut.rgba = texture(Tex, TexCoord) * Colour;
}

