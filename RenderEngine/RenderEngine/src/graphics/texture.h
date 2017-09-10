#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace graphics {
	class Texture
	{
	private:
		unsigned int m_TextureID;
		int m_Width, m_Height, m_NrChannels;

	public:
		Texture(const char * textureSource);
		~Texture();

		void Texture::bind(int glTextureN) const;
		void Texture::unbind() const;
	};
}