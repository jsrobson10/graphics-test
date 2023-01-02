
#pragma once

namespace data
{
	template <typename T>
	constexpr T load(const char* data)
	{
		T num = 0;

		for(int i = 0; i < sizeof(data); i++)
		{
			num = (num << 8) + (data[i] & 0xff);
		}

		return num;
	}

	template <typename T>
	constexpr char* store(char* data, T val)
	{
		for(int i = sizeof(data) - 1; i >= 0; i--)
		{
			data[i] = static_cast<char>(val & 0xff);
			val >>= 8;
		}

		return data;
	}
};

