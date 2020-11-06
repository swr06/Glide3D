#include "Texture.h"

namespace GLClasses
{
	struct TextureMapData
	{
		std::string path;
		GLuint id;

		int width;
		int height;
		int bpp;
		GLenum intformat; 
		GLenum type;
	};

	std::unordered_map<std::string, TextureMapData> CreatedTextures;

	void Texture::CreateTexture(const string& path, bool hdr, bool flip, GLenum type, GLenum min_filter, GLenum mag_filter, GLenum texwrap_s, GLenum texwrap_t, bool clean_up)
	{
		/*
		Check if the texture is already created, if it is, then use the existing texture. Else create a new one :)
		*/
		auto exists = CreatedTextures.find(path);

		if (exists == CreatedTextures.end())
		{
			if (flip)
				stbi_set_flip_vertically_on_load(true);
			else
				stbi_set_flip_vertically_on_load(false);

			GLenum internalformat = 0;
			GLenum _internalformat = 0;

			m_delete_texture = true;
			m_clean_up = clean_up;
			m_type = type;
			m_path = path;

			glGenTextures(1, &m_Texture);
			glBindTexture(type, m_Texture);
			glTexParameteri(type, GL_TEXTURE_WRAP_S, texwrap_s);
			glTexParameteri(type, GL_TEXTURE_WRAP_T, texwrap_t);

			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, mag_filter);

			unsigned char* image = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 0);

			if (m_BPP == 1)
			{
				internalformat = GL_RED;
				_internalformat = GL_RED;
			}

			else if (m_BPP == 3)
			{
				internalformat = GL_RGB;
				_internalformat = hdr ? GL_SRGB : GL_RGB;
			}

			else if (m_BPP == 4)
			{
				internalformat = GL_RGBA;
				_internalformat = hdr ? GL_SRGB_ALPHA : GL_RGBA;
			}

			if (image)
			{
				{
					TextureMapData data =
					{
						path,
						m_Texture,
						m_width,
						m_height,
						m_BPP,
						internalformat,
						type,
					};

					CreatedTextures[path] = data;
				}

				glTexImage2D(type, 0, _internalformat, m_width, m_height, 0, internalformat, GL_UNSIGNED_BYTE, image);
				glGenerateMipmap(type);

				if (clean_up)
				{
					stbi_image_free(image);
				}
			}
		}

		else
		{
			const TextureMapData& tex = CreatedTextures.at(path);
			m_Texture = tex.id;
			m_BPP = tex.bpp;
			m_path = tex.path;
			m_width = tex.width;
			m_height = tex.height;
			m_intformat = tex.intformat;
		}
	}

	ExtractedImageData ExtractTextureData(const std::string& path)
	{
		ExtractedImageData return_val;

		return_val.image_data = stbi_load(path.c_str(), &return_val.width, &return_val.height, &return_val.channels, 0);
		return return_val;
	}
}