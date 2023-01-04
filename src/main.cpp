
#include "data/vec.hpp"
#include "data/matrix_type.hpp"
#include "loader/loader.hpp"
#include "graphics/graphics.hpp"

#include <iostream>
#include <random>

const int POINTS = 10000;

struct gldata_vertex
{
	vec<3, float> pos;
	float padding1 = 0;
	vec<3, float> colour;
	float padding2 = 0;
};

struct gldata_point
{
	vec<3, float> pos;
	float padding1;
	vec<3, float> vel;
	float acc;
};

static matrix<4> mat_cur = matrix_type::identity<4>();
static vec<2> cursor_last = {0, 0};
static double zoom = 4;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	vec<2> now = {xpos / 1000, ypos / 1000};
	vec<2> change = now - cursor_last;
	cursor_last = now;

	mat_cur = matrix_type::rotation3<4>(0, change.x(), change.y()) * mat_cur;
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoom -= yoffset;
}

int main()
{
	graphics::init({4, 3});
	graphics::display display("my sim", {400, 400}, true);

	glfwSetInputMode(display.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if(glfwRawMouseMotionSupported())
	{
    	glfwSetInputMode(display.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	glfwSetCursorPosCallback(display.window, cursor_position_callback);
	glfwSetScrollCallback(display.window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	graphics::program prog_render(display);
	graphics::program prog_sim(display);

	{
		graphics::shader shader_comp(display, GL_COMPUTE_SHADER, loader::data["shader.comp"]);
		graphics::shader shader_vert(display, GL_VERTEX_SHADER, loader::data["shader.vert"]);
		graphics::shader shader_frag(display, GL_FRAGMENT_SHADER, loader::data["shader.frag"]);

		prog_render.attach(shader_vert);
		prog_render.attach(shader_frag);
		prog_render.link();

		prog_sim.attach(shader_comp);
		prog_sim.link();
	}

	graphics::vertexarray g_vao_lines(display);
	graphics::vertexarray g_vao_points(display);
	graphics::buffer<GL_ARRAY_BUFFER, gldata_vertex> g_vbo(display);
	graphics::buffer<GL_ARRAY_BUFFER, gldata_vertex> g_vbo_lines(display);
	graphics::buffer<GL_SHADER_STORAGE_BUFFER, gldata_point> g_points(display);
	graphics::ebuffer g_ebo(display, {
		0, 2, 6, 0, 4, 6, 
		1, 3, 7, 1, 5, 7, 
		0, 1, 3, 0, 2, 3, 
		4, 5, 7, 4, 6, 7, 
		0, 1, 5, 0, 4, 5, 
		2, 3, 7, 2, 6, 7, 
	}, 8);

	gldata_point data[POINTS];
	std::uniform_real_distribution<double> unif(0, 1);
	std::random_device re;

	const int d = 1;

	gldata_vertex data_lines[6] = {
		{{-256,   d,   d}, 0, {1.0/4.0, 1.0/8.0, 1.0/8.0}},
		{{ 256,   d,   d}, 0, {1.0/4.0, 1.0/8.0, 1.0/8.0}},
		{{   d,-256,   d}, 0, {1.0/8.0, 1.0/4.0, 1.0/8.0}},
		{{   d, 256,   d}, 0, {1.0/8.0, 1.0/4.0, 1.0/8.0}},
		{{   d,   d,-256}, 0, {1.0/8.0, 1.0/8.0, 1.0/4.0}},
		{{   d,   d, 256}, 0, {1.0/8.0, 1.0/8.0, 1.0/4.0}},
	};

	for(int i = 0; i < POINTS; i++)
	{
		matrix<3> r_mat = matrix_type::rotation3<3>(unif(re) * M_PI * 2, unif(re) * M_PI * 2, unif(re) * M_PI * 2);

		if(i < POINTS / 2)
		{
			data[i].vel = {1/32.0, 0, 0};
			data[i].pos = r_mat * vec<3>{unif(re), 0, 0} + vec<3>{0, 4, 0};
		}

		else
		{
			data[i].vel = {-1/32.0, 0, 0};
			data[i].pos = r_mat * vec<3>{unif(re), 0, 0} + vec<3>{0, -4, 0};
		}
	}

	prog_render.use();
	g_vao_points.bind();
	g_vbo.bind();
	g_ebo.bind();

	g_vbo.data(nullptr, POINTS * 8, GL_STATIC_DRAW);
	g_ebo.update(POINTS);
	g_vao_points.use({
		{0, sizeof(float), 4, GL_FLOAT, GL_FALSE},
		{1, sizeof(float), 4, GL_FLOAT, GL_FALSE},
	});

	g_vao_lines.bind();
	g_vbo_lines.bind();
	g_vbo_lines.data(data_lines, 6, GL_STATIC_DRAW);
	g_vao_lines.use({
		{0, sizeof(float), 4, GL_FLOAT, GL_FALSE},
		{1, sizeof(float), 4, GL_FLOAT, GL_FALSE},
	});

	prog_sim.use();
	g_vbo.bind<GL_SHADER_STORAGE_BUFFER>();
	g_vbo.bind_base<GL_SHADER_STORAGE_BUFFER>(2);
	g_points.bind();
	g_points.data(data, POINTS, GL_STATIC_DRAW);
	g_points.bind_base(3);

	glUniform1i(prog_sim["count"], POINTS);

	while(!display.should_close())
	{
		display.update();
		
		prog_render.use();

		{
			matrix<4> mat = mat_cur;
			vec<2, int> wsize = display.get_size();

			mat = matrix_type::translation<4>({0, 0, -zoom, 0}) * mat;
			mat = matrix_type::projection<4>((double)wsize.x() / (double)wsize.y(), M_PI / 2, 0.1, 1000) * mat;

			glViewport(0, 0, wsize.x(), wsize.y());
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glUniformMatrix4fv(prog_render["mat"], 1, false, mat);
		}

		g_vao_points.bind();
		g_ebo.draw(GL_TRIANGLES, POINTS);
		g_vao_lines.bind();
		g_vbo_lines.draw(GL_LINES, 6);
		
		prog_sim.use();

		for(int i = 0; i < 2; i++)
		{
			glUniform1i(prog_sim["state"], i);
			glDispatchCompute(POINTS, 1, 1);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}
	}

	graphics::terminate();
}

