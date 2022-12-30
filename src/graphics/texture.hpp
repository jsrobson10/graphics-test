
#pragma once

#include "display.hpp"
#include "graphics_int.hpp"

namespace graphics
{
	vec<3, int> load_stbi_tex(std::vector<char>& out, const std::string& data, int channels);
	
	template <int D = 2, std::enable_if_t<D >= 1 && D <= 3>* = nullptr>
	struct texture
	{
		DO_HEADER(texture);
		vec<D+1, int> detail;

		texture(display& host) : host(&host)
		{
			glGenTextures(1, &id);
		}

		~texture()
		{
			glDeleteTextures(1, &id);
		}

		void bind()
		{
			switch(D)
			{
			case 1:
				glBindTexture(GL_TEXTURE_1D, id);
				break;
			case 2:
				glBindTexture(GL_TEXTURE_2D, id);
				break;
			case 3:
				glBindTexture(GL_TEXTURE_3D, id);
				break;
			}
		}

		void param(GLenum key, GLenum val)
		{
			switch(D)
			{
			case 1:
				glTexParameteri(GL_TEXTURE_1D, key, val);
				break;
			case 2:
				glTexParameteri(GL_TEXTURE_2D, key, val);
				break;
			case 3:
				glTexParameteri(GL_TEXTURE_3D, key, val);
				break;
			}
		}

		template <std::enable_if_t<D == 2>* = nullptr>
		inline void load_image(const std::string& data, int channels)
		{
			std::vector<char> image;
			vec<3, int> detail = load_stbi_tex(image, data, channels);
			load_raw(image, detail);
		}

		inline void load_raw(const std::vector<char>& data, vec<D+1, int> detail)
		{
			load_raw(data.data(), detail);
		}

		void load_raw(const char* data, vec<D+1, int> detail)
		{
			this->detail = detail;

			GLenum format;

			switch(detail[detail.size() - 1])
			{
			case 1:
				format = GL_RED;
				break;
			case 2:
				format = GL_RG;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			}
			
			switch(D)
			{
			case 1:
				glTexImage1D(GL_TEXTURE_1D, 0, format, detail.x(), 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_1D);
				break;
			case 2:
				glTexImage2D(GL_TEXTURE_2D, 0, format, detail.x(), detail.y(), 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				break;
			case 3:
				glTexImage3D(GL_TEXTURE_3D, 0, format, detail.x(), detail.y(), detail.z(), 0, format, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_3D);
				break;
			}
		}
	};
};

