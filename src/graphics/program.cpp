
#ifdef GLFW_ENABLED

#include "program.hpp"

#include <vector>

using namespace graphics;

program::program(display& host) : host(&host)
{
	id = glCreateProgram();
}

program::~program()
{
	glDeleteProgram(id);
}

void program::link()
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
		throw std::runtime_error(std::string(&logData[0], logLength));
	}
}

void program::use()
{
	glUseProgram(id);
}

void program::attach(const shader& s)
{
	glAttachShader(id, s.id);
}

GLuint program::operator[] (std::string pos)
{
	auto at = cached.find(pos);
	
	if(at != cached.end())
	{
		return at->second;
	}
	
	GLuint id = glGetUniformLocation(this->id, pos.c_str());
	cached[pos] = id;

	return id;
}

#endif

