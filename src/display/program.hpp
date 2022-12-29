
#pragma once

#include <unordered_map>

struct display::program
{
	DO_HEADER(program);
	program(display&);
	~program();

	std::unordered_map<std::string, GLuint> cached;

	GLuint operator[] (std::string pos);

	void attach(const shader&);
	void link();
	void use();
};

