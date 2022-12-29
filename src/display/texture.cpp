
#ifdef GLFW_ENABLED

#include "display.hpp"

using texture = display::texture;

texture::texture(display& host) : host(&host)
{
	glGenTextures(1, &id);
}

texture::~texture()
{
	glDeleteTextures(1, &id);
}

#endif

