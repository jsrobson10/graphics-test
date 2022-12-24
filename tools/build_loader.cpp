
#include "../src/loader.hpp"

#include <iostream>

int main(int cargs, const char** vargs)
{
	std::string path = ".";
	std::string include = "\"loader.hpp\"";

	if(cargs > 1)
	{
		path = std::string(vargs[1]);
	}

	if(cargs > 2)
	{
		include = std::string(vargs[2]);
	}

	loader::import_tree(path);

	std::cout << "#include " << include << std::endl << loader::dump << std::endl;
}

