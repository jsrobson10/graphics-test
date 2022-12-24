
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

	vec<3> pos({2, 3, 4});

	std::cout << pos << std::endl;

	std::cout << pos.xy() << std::endl;

	pos.sub_matrix<2,1>({0, 1}) += vec<2>({1, 2});
	
	std::cout << pos.sub_vec<3>({0,2,1}) << std::endl;

	std::cout << pos.zyx() << std::endl;
	std::cout << pos << std::endl;
}

