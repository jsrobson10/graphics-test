
#pragma once

#ifdef GLFW_ENABLED

#include "vec.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <memory>

#define DO_HEADER(NAME) \
	display* host;\
	GLuint id;\
	NAME(const NAME&) = delete;\
	NAME& operator=(const NAME&) = delete;\
	~NAME()

class display
{
private:

	GLFWwindow* window;
	std::string title;
	vec<2, int> size;
	bool fullscreen;

public:

	display(const display&) = delete;
	display& operator=(const display&) = delete;

	struct shader
	{
		DO_HEADER(shader);
		GLuint type;

		shader(display&, GLuint type, const char* code);
		shader(display&, GLuint type, const std::string& code);
	};

	struct program
	{
		DO_HEADER(program);
		program(display&);

		void attach(const shader&);
		GLint uniform(std::string pos);
		void link();
		void use();
	};

	struct buffer
	{
		DO_HEADER(buffer);
		buffer(display&);
	};

	struct vertexarray
	{
		DO_HEADER(vertexarray);
		vertexarray(display&);
	};

	struct texture
	{
		DO_HEADER(texture);
		texture(display&);
	};
	
	display(std::string title, vec<2, int> size, bool fullscreen);

	vec<2, int> get_size();
	void set_fullscreen(bool state);
	void set_title(std::string title);
	bool is_fullscreen();
	bool should_close();
	void update();
};

#endif

