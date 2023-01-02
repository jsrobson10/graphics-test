
#pragma once

#include "display.hpp"
#include "graphics_int.hpp"
#include "program.hpp"

namespace graphics
{
	template <GLenum TARGET, typename TYPE>
	struct buffer
	{
		DO_HEADER(buffer);
	
		constexpr buffer(display& host) : host(&host)
		{
			glGenBuffers(1, &id);
		}
	
		constexpr ~buffer()
		{
			glDeleteBuffers(1, &id);
		}
	
		constexpr void bind()
		{
			glBindBuffer(TARGET, id);
		}

		constexpr void unbind()
		{
			glBindBuffer(TARGET, 0);
		}

		constexpr void bind_base(int binding)
		{
			glBindBufferBase(TARGET, binding, id);
		}
	
		constexpr void data(const TYPE* data, int size, GLenum usage)
		{
			glBufferData(TARGET, size * sizeof(TYPE), data, usage);
		}
	
		constexpr void data(const std::vector<TYPE>& data, GLenum usage)
		{
			glBufferData(TARGET, data.size() * sizeof(TYPE), data.data(), usage);
		}
	
		constexpr void sub_data(const TYPE* data, int offset, int size)
		{
			glBufferSubData(TARGET, offset * sizeof(TYPE), size * sizeof(TYPE), data);
		}
		
		constexpr void sub_data(const std::vector<TYPE>& data, int offset)
		{
			glBufferSubData(TARGET, offset * sizeof(TYPE), data.size() * sizeof(TYPE), data.data());
		}
	};
};

