
#pragma once

#include "display.hpp"
#include "graphics_int.hpp"
#include "program.hpp"

namespace graphics
{
	template <GLenum c_TARGET, typename TYPE>
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
	
		template <GLenum TARGET = c_TARGET>
		constexpr void bind()
		{
			glBindBuffer(TARGET, id);
		}

		template <GLenum TARGET = c_TARGET>
		constexpr void unbind()
		{
			glBindBuffer(TARGET, 0);
		}

		template <GLenum TARGET = c_TARGET>
		constexpr void bind_base(int binding)
		{
			glBindBufferBase(TARGET, binding, id);
		}
	
		template <GLenum TARGET = c_TARGET>
		constexpr void data(const TYPE* data, int size, GLenum usage)
		{
			glBufferData(TARGET, size * sizeof(TYPE), data, usage);
		}
	
		template <GLenum TARGET = c_TARGET>
		constexpr void data(const std::vector<TYPE>& data, GLenum usage)
		{
			glBufferData(TARGET, data.size() * sizeof(TYPE), data.data(), usage);
		}
	
		template <GLenum TARGET = c_TARGET>
		constexpr void sub_data(const TYPE* data, int offset, int size)
		{
			glBufferSubData(TARGET, offset * sizeof(TYPE), size * sizeof(TYPE), data);
		}
		
		template <GLenum TARGET = c_TARGET>
		constexpr void sub_data(const std::vector<TYPE>& data, int offset)
		{
			glBufferSubData(TARGET, offset * sizeof(TYPE), data.size() * sizeof(TYPE), data.data());
		}

		constexpr void draw(GLenum mode, int first, int count)
		{
			glDrawArrays(mode, first * sizeof(TYPE), count * sizeof(TYPE));
		}

		constexpr void draw(GLenum mode, int count)
		{
			draw(mode, 0, count);
		}
	};
};

