
#pragma once

#include "display.hpp"
#include "graphics_int.hpp"

namespace graphics
{
	struct vertexarray
	{
		struct item
		{
			GLuint loc;
			GLint size;
			GLint count;
			GLenum type;
			GLboolean normalized;
	
			item(GLuint loc, GLint size, GLint count, GLenum type, GLboolean normalized);
		};
	
		DO_HEADER(vertexarray);
		vertexarray(display&);
		~vertexarray();
	
		void bind();
		void use(const std::list<item>& items);
	};
};

