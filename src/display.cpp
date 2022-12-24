
#ifdef GLFW_ENABLED

#include "display.hpp"

#include <vector>

display::display(std::string title, vec<2, int> size, bool fullscreen)
{
	this->title = title;
	this->fullscreen = fullscreen;
	this->size = size;

	if(!glfwInit())
	{
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(size.x(), size.y(), title.c_str(), nullptr, nullptr);

	if(!window)
	{
		throw "failed to create glfw window";
	}

	glfwMakeContextCurrent(window);
}

void display::update()
{
	glfwGetFramebufferSize(window, &size.x(), &size.y());
	glfwPollEvents();
}

bool display::should_close()
{
	return glfwWindowShouldClose(window);
}

vec<2, int> display::get_size()
{
	return size;
}

void display::set_fullscreen(bool state)
{
	
}

void display::set_title(std::string title)
{
	
}

bool display::is_fullscreen()
{
	return fullscreen;
}

display::shader::shader(display& host, GLuint type, const char* code) : host(&host)
{
	GLint result;
	GLint logLength;

	id = glCreateShader(type);
	glShaderSource(id, 1, &code, nullptr);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

	if(logLength > 0)
	{
		std::vector<char> logData(logLength + 1);
		glGetShaderInfoLog(id, logLength, nullptr, &logData[0]);
		throw std::string(&logData[0]);
	}
}

display::shader::shader(display& host, GLuint type, const std::string& code) : shader(host, type, code.c_str())
{

}

display::shader::~shader()
{
	glDeleteShader(id);
}

display::program::program(display& host) : host(&host)
{
	id = glCreateProgram();
}

display::program::~program()
{
	glDeleteProgram(id);
}

void display::program::link()
{
	GLint result;
	GLint logLength;
	
	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

	if(logLength > 0)
	{
		std::vector<char> logData(logLength + 1);
		glGetProgramInfoLog(id, logLength, nullptr, &logData[0]);
		throw std::string(&logData[0], logLength);
	}
}

void display::program::use()
{
	glUseProgram(id);
}

void display::program::attach(const shader& s)
{
	glAttachShader(id, s.id);
}

GLint display::program::uniform(std::string pos)
{
	return glGetUniformLocation(id, pos.c_str());
}

display::buffer::buffer(display& host) : host(&host)
{
	glGenBuffers(1, &id);
}

display::buffer::~buffer()
{
	glDeleteBuffers(1, &id);
}

display::vertexarray::vertexarray(display& host) : host(&host)
{
	glGenVertexArrays(1, &id);
}

display::vertexarray::~vertexarray()
{
	glDeleteVertexArrays(1, &id);
}

display::texture::texture(display& host) : host(&host)
{
	glGenTextures(1, &id);
}

display::texture::~texture()
{
	glDeleteTextures(1, &id);
}

#endif

