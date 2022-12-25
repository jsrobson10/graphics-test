
#include "vec.hpp"
#include "loader.hpp"
#include "display.hpp"

#include <iostream>

int main()
{
//	display d("thingy", {{400, 400}}, false);
/*
	display::shader shader_vert(d, GL_VERTEX_SHADER, loader::data["shader.vsh"]);
	display::shader shader_frag(d, GL_FRAGMENT_SHADER, loader::data["shader.fsh"]);
	display::program prog(d);

	prog.attach_shader(shader_vert);
	prog.attach_shader(shader_frag);
	prog.link();
*/
//	while(!d.should_close())
//	{
//		prog.use();

//		d.update();
//	}

	matrix<4,3> mat1({
		1,  2,  3,
		4,  5,  6,
		7,  8,  9,
		10, 11, 12
	});

	matrix<3,6> mat2({
		0, 1, 1, 0, 0, 0, 
		0, 0, 1, 1, 1, 0, 
		0, 0, 0, 0, 1, 1, 
	});

	matrix mat3 = mat1 * mat2;

	matrix<6,2> mat4({
		1,0,
		0,1,
		1,0,
		0,1,
		1,0,
		0,1,
	});

	matrix mat5 = mat3 * mat4;

	for(auto& row : mat3.rows())
	{
		std::cout << row << std::endl;
	}

	std::cout << std::endl;

	for(auto& row : mat5.rows())
	{
		std::cout << row << std::endl;
	}
}

