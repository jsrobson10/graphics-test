
#include "display.hpp"

using indexlist = display::indexlist;

indexlist::indexlist(display& d, const std::vector<GLuint>& items) : buffer(d), base(items)
{
	count_now = 0;
}

void indexlist::bind()
{
	buffer.bind();
}

void indexlist::update(int count)
{
	if(count < count_now)
	{
		return;
	}

	int base_len = base.size();
	int data_len = count * base_len;

	std::vector<GLuint> indicies(data_len);

	for(int i = 0; i < data_len; i += base_len)
	{
		for(int j = 0; j < base_len; j += 1)
		{
			indicies[j + i] = base[j] + i;
		}
	}

	count_now = count;
	buffer.data(indicies, GL_STATIC_DRAW);
}

void indexlist::draw(int count)
{
	if(count > count_now)
	{
		bind();
		update(count);
	}
	
	glDrawElements(GL_TRIANGLES, count * base.size(), GL_UNSIGNED_INT, 0);
}

