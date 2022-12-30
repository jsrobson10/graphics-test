
#ifdef GLFW_ENABLED
#define STB_IMAGE_IMPLEMENTATION

#include "texture.hpp"
#include "stb_image.h"

vec<3, int> graphics::load_stbi_tex(std::vector<char>& out, const std::string& data, int channels)
{
	int x, y, c;
	const char* image = (const char*)stbi_load_from_memory((unsigned char*)data.c_str(), data.length(), &x, &y, &c, channels);

	if(!image)
	{
		throw std::runtime_error("failed to load texture");
	}
	
	out = std::vector<char>(image, &image[x * y * c]);
	stbi_image_free((void*)image);

	return {x, y, c};
}


#endif

