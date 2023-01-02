
#pragma once

#include <unordered_map>

#include "graphics_int.hpp"
#include "display.hpp"
#include "shader.hpp"

namespace graphics
{
	struct program
	{
		DO_HEADER(program);
		
		std::unordered_map<std::string, GLuint> cached;

		program(display& host);
		~program();

		GLuint operator[] (std::string pos);

		void attach(const shader& s);
		void link();
		void use();
	};
};

