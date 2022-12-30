
#pragma once

#include "display.hpp"
#include "graphics_int.hpp"

namespace graphics
{
	struct shader
	{
		DO_HEADER(shader);
		GLuint type;
	
		shader(display&, GLuint type, const char* code);
		shader(display&, GLuint type, const std::string& code);
		~shader();
	};
};

