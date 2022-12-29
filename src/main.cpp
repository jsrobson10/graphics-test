
#include "data/vec.hpp"
#include "loader/loader.hpp"
#include "display/display.hpp"

#include <iostream>

int main()
{
	display::init({4, 3});
	display d("thingy", {{400, 400}}, true);

	display::shader shader_vert(d, GL_VERTEX_SHADER, loader::data["shader.vsh"]);
	display::shader shader_frag(d, GL_FRAGMENT_SHADER, loader::data["shader.fsh"]);
	display::program prog(d);

	prog.attach(shader_vert);
	prog.attach(shader_frag);
	prog.link();
	prog.use();

	display::vertexarray va(d);
	display::buffer<GL_ARRAY_BUFFER, vec<5, float>> buff(d);
	display::indexlist i_buff(d, {0, 1, 3, 0, 2, 3});

	va.bind();
	buff.bind();

	vec<5, float> points[] = {
			{-0.5, -0.5, 0, 1, 0},
			{-0.5,  0.5, 1, 0, 0},
			{ 0.5, -0.5, 0, 0, 1},
			{ 0.5,  0.5, 0, 1, 0},
	};

	buff.data(points, 5, GL_STATIC_DRAW);

	i_buff.bind();
	i_buff.update(1);
	
	va.use({
			{0, sizeof(float), 2, GL_FLOAT, GL_FALSE},
			{1, sizeof(float), 3, GL_FLOAT, GL_FALSE},
	});

	//double r = 0;

	while(!d.should_close())
	{
		d.update();

		glViewport(0, 0, d.get_size().x(), d.get_size().y());
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		//r += 0.001;
	
		//glUniformMatrix4fv(prog["mat"], 1, GL_FALSE, &mat[0]);

		prog.use();
		va.bind();
		i_buff.draw(1);
	}

	display::terminate();
}

