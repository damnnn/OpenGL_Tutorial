#pragma once

#include <glad/glad.h> // holds all OpenGL type declarations

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

namespace graphics {

	struct VertexData {
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		// tangent
		glm::vec3 Tangent;
		// bitangent
		glm::vec3 Bitangent;
	};

	struct TextureData {
		unsigned int id;
		string type;
		aiString path;
	};

	class Mesh {
	public:
		/*  Mesh Data  */
		vector<VertexData> m_Vertices;
		vector<unsigned int> m_Indices;
		vector<TextureData> m_Textures;
		unsigned int m_VAO;

	public:
		/*  Functions  */
		// constructor
		Mesh(vector<VertexData> vertices, vector<unsigned int> indices, vector<TextureData> textures);

		// render the mesh
		void Draw(Shader* shader);

	private:
		/*  Render data  */
		unsigned int m_VBO, m_EBO;
	private:
		/*  Functions    */
		// initializes all the buffer objects/arrays
		void setupMesh();
		
	};
}