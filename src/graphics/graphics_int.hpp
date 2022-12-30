
#pragma once

#define DO_HEADER(NAME) \
	display* host;\
	GLuint id;\
	NAME(const NAME&) = delete;\
	NAME& operator=(const NAME&) = delete;

