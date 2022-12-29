
#pragma once

struct display::vertexarray
{
	struct item
	{
		GLuint loc;
		GLint size;
		GLint count;
		GLenum type;
		GLboolean normalized;

		item(GLuint loc, GLint size, GLint count, GLenum type, GLboolean normalized);
	};

	DO_HEADER(vertexarray);
	vertexarray(display&);
	~vertexarray();

	void bind();
	void use(const std::list<item>& items);
};

