
#pragma once

#include "display.hpp"
#include "graphics_int.hpp"

namespace graphics
{
	template <GLenum TARGET, typename TYPE>
	struct buffer
	{
		DO_HEADER(buffer);
	
		buffer(display& host) : host(&host)
		{
			glGenBuffers(1, &id);
		}
	
		~buffer()
		{
			glDeleteBuffers(1, &id);
		}
	
		void bind()
		{
			glBindBuffer(TARGET, id);
		}
	
		void data(const TYPE* data, int size, GLenum usage)
		{
			glBufferData(TARGET, size * sizeof(TYPE), data, usage);
		}
	
		void data(const std::vector<TYPE>& data, GLenum usage)
		{
			glBufferData(TARGET, data.size() * sizeof(TYPE), data.data(), usage);
		}
	
		void sub_data(const TYPE* data, int offset, int size)
		{
			glBufferSubData(TARGET, offset * sizeof(TYPE), size * sizeof(TYPE), data);
		}
		
		void sub_data(const std::vector<TYPE>& data, int offset)
		{
			glBufferSubData(TARGET, offset * sizeof(TYPE), data.size() * sizeof(TYPE), data.data());
		}
	};
};

