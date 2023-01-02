
#pragma once

#include "buffer.hpp"

namespace graphics
{
	struct ebuffer
	{
		buffer<GL_ELEMENT_ARRAY_BUFFER, GLuint> buffer;
		std::vector<GLuint> base;
		int count_now;
		int skip;
		
		ebuffer(display& d, const std::vector<GLuint>& items, int skip);
		void update(int count);
		void draw(int count);
		void unbind();
		void bind();
	};
};

