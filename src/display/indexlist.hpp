
#pragma once

struct display::indexlist
{
	display::buffer<GL_ELEMENT_ARRAY_BUFFER, GLuint> buffer;
	std::vector<GLuint> base;
	int count_now;
	
	indexlist(display& d, const std::vector<GLuint>& items);
	void update(int count);
	void draw(int count);
	void bind();
};

