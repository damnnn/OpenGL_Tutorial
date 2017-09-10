#pragma once

#include <glad/glad.h>
#include <iostream>

namespace graphics {
	class IndexBuffer
	{
	private:
		unsigned int m_IndexBufferID;
		unsigned int m_PrimitiveCount;
		unsigned int m_ComponentCount;

	public:
		IndexBuffer(unsigned int * indices, unsigned int primitiveCount, unsigned int componentCount);

		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline unsigned int getPrimitiveCount() { return m_PrimitiveCount; }
		inline unsigned int getComponentCount() { return m_ComponentCount; }
	};
}