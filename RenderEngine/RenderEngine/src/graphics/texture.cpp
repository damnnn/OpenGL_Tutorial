#include "texture.h"
#include <stb/stb_image.h>
namespace graphics {

	Texture::Texture(const char * textureSource)
	{
		
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load image, create texture and generate mipmaps
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
												// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.

		unsigned char *data = stbi_load(textureSource, &m_Width, &m_Height, &m_NrChannels, 0);
		if (data)
		{
			GLenum format;
			if (m_NrChannels == 1)
				format = GL_RED;
			else if (m_NrChannels == 3)
				format = GL_RGB;
			else if (m_NrChannels == 4)
				format = GL_RGBA;
			
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		stbi_image_free(data);
		
	}
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	void Texture::bind(int glTextureN) const
	{
		glActiveTexture(GL_TEXTURE0 + glTextureN);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	// Check if Shader should be closed
	// --------------------------------
	void Texture::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}