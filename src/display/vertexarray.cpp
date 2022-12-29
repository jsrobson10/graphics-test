
#ifdef GLFW_ENABLED

#include "display.hpp"

using vertexarray = display::vertexarray;
using item = vertexarray::item;

vertexarray::vertexarray(display& host) : host(&host)
{
	glGenVertexArrays(1, &id);
}

vertexarray::~vertexarray()
{
	glDeleteVertexArrays(1, &id);
}

void vertexarray::bind()
{
	glBindVertexArray(id);
}

void vertexarray::use(const std::list<item>& items)
{
	GLint size = 0;
	GLint at = 0;

	for(const item& i : items)
	{
		size += i.size * i.count;
	}

	for(const item& i : items)
	{
		glVertexAttribPointer(i.loc, i.count, i.type, i.normalized, size, reinterpret_cast<void*>(at));
		glEnableVertexAttribArray(i.loc);
		at += i.size * i.count;
	}
}

item::item(GLuint loc, GLint size, GLint count, GLenum type, GLboolean normalized)
{
	this->loc = loc;
	this->type = type;
	this->size = size;
	this->count = count;
	this->normalized = normalized;
}
	
#endif

