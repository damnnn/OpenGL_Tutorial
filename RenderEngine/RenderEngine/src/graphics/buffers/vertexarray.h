#pragma once

#include <glad/glad.h>
#include <vector>

#include "vertexbuffer.h"


namespace graphics {

	class VertexArray
	{
	private:
		unsigned int m_VertexArrayID;
		std::vector<VertexBuffer*> m_VertexBuffers;

	public:

		VertexArray();
		~VertexArray();

		void addVertexBuffer(VertexBuffer * vertexBuffer, unsigned int index);
		void SetAttribOffset();
		void bind() const;
		void unbind() const;


	};

}