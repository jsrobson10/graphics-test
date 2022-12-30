
#pragma once

#include "graphics_int.hpp"
#include "display.hpp"
#include "buffer.hpp"
#include "ebuffer.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertexarray.hpp"

namespace graphics
{
	void init(vec<2, int>);
	void terminate();
};

#undef DO_HEADER

