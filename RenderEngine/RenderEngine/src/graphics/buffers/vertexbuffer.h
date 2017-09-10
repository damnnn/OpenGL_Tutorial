#pragma once

#include <glad/glad.h>
#include <iostream>

namespace graphics {
		class VertexBuffer
		{
		private:
			unsigned int m_VertexBufferID;
			unsigned int m_VertexCount;
			unsigned int m_ComponentCount;

		public:
			VertexBuffer(float * vertices, unsigned int vertexCount, unsigned int componentCount);

			~VertexBuffer();

			void bind() const;
			void unbind() const;

			inline unsigned int getVertexCount() { return m_VertexCount; }
			inline unsigned int getComponentCount() { return m_ComponentCount; }
		};
}