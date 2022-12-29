
#pragma once

#ifdef GLFW_ENABLED

#include "../data/vec.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <list>

#define DO_HEADER(NAME) \
	display* host;\
	GLuint id;\
	NAME(const NAME&) = delete;\
	NAME& operator=(const NAME&) = delete;

class display
{
private:

	GLFWwindow* window = nullptr;
	std::string title;
	vec<2, int> size;
	bool fullscreen;

public:

	template <GLenum TARGET, typename TYPE>
		struct buffer;
	struct program;
	struct shader;
	struct texture;
	struct vertexarray;
	struct indexlist;

	display(const display&) = delete;
	display& operator=(const display&) = delete;
	display(std::string title, vec<2, int> size, bool fullscreen);

	inline vec<2, int> get_size()
	{
		return size;
	}

	inline bool is_fullscreen()
	{
		return fullscreen;
	}

	void set_fullscreen(bool state);
	void set_title(std::string title);
	bool should_close();
	void update();

	static void init(vec<2, int>);
	static void terminate();
};

#include "buffer.hpp"
#include "shader.hpp"
#include "program.hpp"
#include "vertexarray.hpp"
#include "texture.hpp"
#include "indexlist.hpp"

#endif

