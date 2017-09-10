#include "vertexbuffer.h"

namespace graphics {
	VertexBuffer::VertexBuffer(float * vertices, unsigned int vertexCount, unsigned int componentCount)
		:m_VertexCount(vertexCount), m_ComponentCount(componentCount)
	{
		glGenBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, m_VertexCount* m_ComponentCount * sizeof(float), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_VertexBufferID);
	}
	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}
	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}