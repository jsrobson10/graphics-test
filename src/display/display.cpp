
#ifdef GLFW_ENABLED

#include "display.hpp"

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "GL CALLBACK (";

	switch(type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "INFO";
		break;
	default:
		std::cout << "UNKNOWN[" << type << "]";
	}

	std::cout << "): " << message << std::endl;
}

void display::init(vec<2, int> version)
{
	if(!glfwInit())
	{
		throw std::runtime_error("failed to initialize glfw");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.x());
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.y());
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void display::terminate()
{
	glfwTerminate();
}

display::display(std::string title, vec<2, int> size, bool fullscreen)
{
	this->title = title;
	this->fullscreen = fullscreen;
	this->size = size;

	set_fullscreen(fullscreen);

	if(glewInit() != GLEW_OK)
	{
		throw std::runtime_error("failed to initialize glew");
	}
	
	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
}

void display::update()
{
	glfwSwapBuffers(window);
	glfwGetFramebufferSize(window, &size.x(), &size.y());
	glfwPollEvents();
}

bool display::should_close()
{
	return glfwWindowShouldClose(window);
}

void display::set_fullscreen(bool state)
{
	if(window && fullscreen == state)
	{
		return;
	}
	
	fullscreen = state;

	if(fullscreen)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, window);
	}

	else
	{
		window = glfwCreateWindow(size.x(), size.y(), title.c_str(), nullptr, window);
	}
	
	if(!window)
	{
		throw std::runtime_error("failed to create glfw window");
	}
	
	glfwMakeContextCurrent(window);
}

void display::set_title(std::string title)
{
	glfwSetWindowTitle(window, title.c_str());
}

#endif

