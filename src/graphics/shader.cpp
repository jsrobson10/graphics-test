
#ifdef GLFW_ENABLED

#include "shader.hpp"

#include <vector>

using namespace graphics;

shader::shader(display& host, GLuint type, const char* code) : host(&host)
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
		throw std::runtime_error(std::string(&logData[0], logLength));
	}
}

shader::shader(display& host, GLuint type, const std::string& code) : shader(host, type, code.c_str())
{

}

shader::~shader()
{
	glDeleteShader(id);
}

#endif

