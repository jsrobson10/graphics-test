
#pragma once

#include <string>
#include <unordered_map>
#include <ostream>

namespace loader
{
	extern std::unordered_map<std::string, std::string> data;
	struct load { load(const char* data); };
	void import_tree(std::string path);
	std::ostream& dump(std::ostream& o);
};

