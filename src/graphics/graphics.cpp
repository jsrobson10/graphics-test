
#include "graphics.hpp"


void graphics::init(vec<2, int> version)
{
	if(!glfwInit())
	{
		throw std::runtime_error("failed to initialize glfw");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.x());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.y());
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void graphics::terminate()
{
	glfwTerminate();
}

