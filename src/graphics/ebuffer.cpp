
#ifdef GLFW_ENABLED

#include "ebuffer.hpp"

using namespace graphics;

ebuffer::ebuffer(display& d, const std::vector<GLuint>& items, int skip) : buffer(d), base(items)
{
	this->count_now = 0;
	this->skip = skip;
}

void ebuffer::bind()
{
	buffer.bind();
}

void ebuffer::unbind()
{
	buffer.unbind();
}

void ebuffer::update(int count)
{
	if(count < count_now)
	{
		return;
	}

	int base_len = base.size();
	int data_len = count * base_len;
	int at = 0;

	std::vector<GLuint> indicies(data_len);

	for(int i = 0; i < data_len; i += base_len)
	{
		for(int j = 0; j < base_len; j += 1)
		{
			indicies[j + i] = base[j] + at;
		}

		at += skip;
	}

	for(int i = 0; i < data_len; i++)
	{
		std::cout << indicies[i] << " ";
	}

	std::cout << "\n";

	count_now = count;
	buffer.data(indicies, GL_STATIC_DRAW);
}

void ebuffer::draw(int count)
{
	if(count > count_now)
	{
		bind();
		update(count);
	}
	
	glDrawElements(GL_TRIANGLES, count * base.size(), GL_UNSIGNED_INT, 0);
}

#endif

