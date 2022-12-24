
#include "loader.hpp"
#include "data.hpp"

#include <fstream>
#include <filesystem>

std::unordered_map<std::string, std::string> loader::data;

static void to_hex(std::ostream& out, const char* data, size_t len)
{
	const char HEX[] = "0123456789abcdef";

	for(int i = 0; i < len; i++)
	{
		unsigned char b = data[i];
		out << "\\x" << HEX[b >> 4] << HEX[b & 15];
	}
}

loader::load::load(const char* data)
{
	const char* at = data;

	while(true)
	{
		uint64_t name_len = data::load<uint64_t>(at);

		if(name_len == -1)
		{
			break;
		}

		uint64_t data_len = data::load<uint64_t>(&at[name_len + 8]);
		loader::data[std::string(&at[8], name_len)] = std::string(&at[name_len + 16], data_len);
		at = &at[name_len + data_len + 16];
	}
}

void loader::import_tree(std::string path)
{
	int split_from = path.length();

	if(path[path.length() - 1] != '/')
	{
		split_from += 1;
	}

	for(auto& entry : std::filesystem::recursive_directory_iterator(path))
	{
		if(entry.is_regular_file())
		{
			std::string at(entry.path().string());
			std::ifstream file(at, std::ios::binary);
			at = at.substr(split_from);
			
			if(!file)
			{
				continue;
			}

			loader::data[at] = std::string(std::istreambuf_iterator<char>(file), {});
		}
	}
}

std::ostream& loader::dump(std::ostream& o)
{
	char buff[8];

	o << "static loader::load l(\"";

	for(auto& pair : loader::data)
	{
		to_hex(o, data::store<uint64_t>(buff, pair.first.length()), 8);
		to_hex(o, pair.first.c_str(), pair.first.length());
		to_hex(o, data::store<uint64_t>(buff, pair.second.length()), 8);
		to_hex(o, pair.second.c_str(), pair.second.length());
	}

	to_hex(o, data::store<uint64_t>(buff, -1), 8);
	return o << "\");";
}

