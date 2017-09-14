#include "mesh.h"

namespace graphics {

	// constructor
	Mesh::Mesh(vector<VertexData> vertices, vector<unsigned int> indices, vector<TextureData> textures)
		:m_Vertices(vertices), m_Indices(indices),m_Textures(textures)
	{
		// set the vertex buffers and its attribute pointers.
		setupMesh();
	}

	// render the mesh
	void Mesh::Draw(Shader * shader)
	{
		// bind buffers/arrays
		// bind vertex array
		glBindVertexArray(m_VAO);
		// bind element buffers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			// active proper texture unit before binding
			glActiveTexture(GL_TEXTURE0 + i); 
			// retrieve texture number (the N in diffuse_textureN)
			stringstream ss;
			string number;
			string material = "material.";
			string name = m_Textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // transfer unsigned int to stream
			else if (name == "texture_specular")
				ss << specularNr++; // transfer unsigned int to stream
			else if (name == "texture_normal")
				ss << normalNr++; // transfer unsigned int to stream
			else if (name == "texture_height")
				ss << heightNr++; // transfer unsigned int to stream
			number = ss.str();
			// set the uniform sampler in the shader to the correct texture unit
			glUniform1i(glGetUniformLocation(shader->getShaderID(), (material + name + number).c_str()), i);
			// bind proper texture unit
			glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
		}

		// draw mesh
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);

		
		// unbind buffers/arrays/textures
		// unbind textures
		for (unsigned int i = 0; i < m_Textures.size(); i++)
		{
			// active proper texture unit before unbinding
			glActiveTexture(GL_TEXTURE0 + i); 
			// unbind proper texture unit
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// unbind element buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// unbind vertex array
		glBindVertexArray(0);
		
	}

	// initializes all the buffer objects/arrays
	void Mesh::setupMesh()
	{
		// create buffers/arrays
		// create vertex array
		glGenVertexArrays(1, &m_VAO);
		// create element buffer
		glGenBuffers(1, &m_EBO);
		// create vertex buffer
		glGenBuffers(1, &m_VBO);

		// bind buffers/arrays
		// bind vertex array
		glBindVertexArray(m_VAO);
		// bind element buffers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		// bind vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		
		// load data in buffer
		// load indices in  element buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
		// load vertices in  array buffer
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(VertexData), &m_Vertices[0], GL_STATIC_DRAW);

		// enable and set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, Bitangent));

		
		// unbind buffers/arrays
		// unbind vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// unbind element buffers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// unbind vertex array
		glBindVertexArray(0);
		
	}
}