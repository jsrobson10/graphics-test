
#pragma once

struct display::shader
{
	DO_HEADER(shader);
	GLuint type;

	shader(display&, GLuint type, const char* code);
	shader(display&, GLuint type, const std::string& code);
	~shader();
};

