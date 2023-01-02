
#include "data/vec.hpp"
#include "data/matrix_type.hpp"
#include "loader/loader.hpp"
#include "graphics/graphics.hpp"

#include <iostream>

struct gldata
{
	vec<3, float> pos;
	vec<2, float> texpos;
	GLint datapos;
};

int main()
{
	graphics::init({4, 3});
	graphics::display d("thingy", {400, 400}, true);

	glEnable(GL_DEPTH_TEST);

	graphics::shader shader_vert(d, GL_VERTEX_SHADER, loader::data["shader.vsh"]);
	graphics::shader shader_frag(d, GL_FRAGMENT_SHADER, loader::data["shader.fsh"]);
	graphics::program prog(d);

	prog.attach(shader_vert);
	prog.attach(shader_frag);
	prog.link();
	prog.use();

	graphics::vertexarray va(d);
	graphics::buffer<GL_ARRAY_BUFFER, gldata> buff(d);
	graphics::buffer<GL_SHADER_STORAGE_BUFFER, vec<4, float>> ssbuff(d);
	graphics::ebuffer e_buff(d, {0, 1, 3, 0, 2, 3}, 4);

	vec<4, float> data_ss[] = {
		{0, 0, 1, 1},
		{0, 1, 0, 1},
		{0, 1, 1, 1},
		{1, 0, 0, 1},
		{1, 0, 1, 1},
		{1, 1, 0, 1},
	};

	ssbuff.bind();
	ssbuff.data(data_ss, 6, GL_STATIC_DRAW);
	ssbuff.bind_base(2);
	ssbuff.unbind();

	graphics::texture tex(d);
	tex.bind();
	tex.param(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tex.param(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tex.param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex.param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex.load_image(loader::data["wall.jpg"], 3);

	va.bind();
	buff.bind();

	gldata points[] = {
			{{-0.5, -0.5, -0.5}, {0, 0}, 0},
			{{-0.5,  0.5, -0.5}, {0, 1}, 1},
			{{ 0.5, -0.5, -0.5}, {1, 0}, 2},
			{{ 0.5,  0.5, -0.5}, {1, 1}, 3},
			
			{{-0.5, -0.5, -0.5}, {0, 0}, 0},
			{{-0.5, -0.5,  0.5}, {1, 0}, 4},
			{{-0.5,  0.5, -0.5}, {0, 1}, 1},
			{{-0.5,  0.5,  0.5}, {1, 1}, 5},
			
			{{-0.5, -0.5, -0.5}, {0, 1}, 0},
			{{-0.5, -0.5,  0.5}, {0, 0}, 4},
			{{ 0.5, -0.5, -0.5}, {1, 1}, 2},
			{{ 0.5, -0.5,  0.5}, {1, 0}, 1},
			
			{{-0.5, -0.5,  0.5}, {0, 0}, 4},
			{{-0.5,  0.5,  0.5}, {0, 1}, 5},
			{{ 0.5, -0.5,  0.5}, {1, 0}, 1},
			{{ 0.5,  0.5,  0.5}, {1, 1}, 0},
			
			{{-0.5,  0.5, -0.5}, {0, 1}, 1},
			{{-0.5,  0.5,  0.5}, {0, 0}, 5},
			{{ 0.5,  0.5, -0.5}, {1, 1}, 3},
			{{ 0.5,  0.5,  0.5}, {1, 0}, 0},
			
			{{ 0.5, -0.5, -0.5}, {0, 0}, 2},
			{{ 0.5, -0.5,  0.5}, {1, 0}, 1},
			{{ 0.5,  0.5, -0.5}, {0, 1}, 3},
			{{ 0.5,  0.5,  0.5}, {1, 1}, 0},
	};

	buff.data(points, 24, GL_STATIC_DRAW);

	e_buff.bind();
	e_buff.update(2);
	
	va.use({
			{0, sizeof(float), 3, GL_FLOAT, GL_FALSE},
			{1, sizeof(float), 2, GL_FLOAT, GL_FALSE},
			{2, sizeof(GLint), 1, GL_INT},
	});

	double r = 0;

	while(!d.should_close())
	{
		d.update();

		vec<2> wsize = d.get_size();
		
		glViewport(0, 0, wsize.x(), wsize.y());
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		r += 0.01;
		matrix<4> camera = matrix_type::projection<4>((double)wsize.x() / (double)wsize.y(), M_PI / 2, 0.1, 1000);
		matrix<4> mat = matrix_type::identity<4>();
		
		mat = matrix_type::rotation3<4>(r * 0.1, r * 0.2, r * 0.3) * mat;
		mat = matrix_type::translation<4>({0, 0, -2, 0}) * mat;

		glUniformMatrix4fv(prog["mat"], 1, GL_FALSE, mat);
		glUniformMatrix4fv(prog["camera"], 1, GL_FALSE, camera);

		prog.use();
		va.bind();
		e_buff.draw(6);
	}

	graphics::terminate();
}

