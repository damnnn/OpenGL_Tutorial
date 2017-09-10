#include "vertexarray.h"

namespace graphics {

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_VertexArrayID);
	}

	VertexArray::~VertexArray()
	{
		for (int i = 0; i < m_VertexBuffers.size(); i++)
			delete m_VertexBuffers[i];

		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void VertexArray::addVertexBuffer(VertexBuffer * vertexBuffer, unsigned int index)
	{
		bind();
		vertexBuffer->bind();
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, vertexBuffer->getComponentCount(), GL_FLOAT, GL_FALSE,
			vertexBuffer->getComponentCount() * sizeof(float), (void*)(0));
		vertexBuffer->unbind();
		unbind();
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(m_VertexArrayID);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

}