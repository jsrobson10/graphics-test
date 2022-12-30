
#pragma once

#ifdef GLFW_ENABLED

#include "../data/vec.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <list>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace graphics
{
	struct display
	{
		GLFWwindow* window = nullptr;
		std::string title;
		vec<2, int> size;
		bool fullscreen;
	
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
	
	};
};

#endif

