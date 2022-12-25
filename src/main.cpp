
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
		11, 12, 13,
		14, 15, 16,
		17, 18, 19,
		20, 21, 22
	});

	mat1.col(1) *= vec<4>({1,2,1,2});

	for(vec<3> row : mat1.rows())
	{
		std::cout << row << std::endl;

		row /= 2;
	}
	
	for(vec<3> row : mat1.rows())
	{
		std::cout << row << std::endl;
	}
}

