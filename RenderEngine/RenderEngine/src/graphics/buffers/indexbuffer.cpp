#include "indexbuffer.h"

namespace graphics {
	IndexBuffer::IndexBuffer(unsigned int * indices, unsigned int primitiveCount, unsigned int componentCount)
		:m_PrimitiveCount(primitiveCount), m_ComponentCount(componentCount)
	{
		glGenBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_PrimitiveCount* m_ComponentCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_IndexBufferID);
	}
	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	}
	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}