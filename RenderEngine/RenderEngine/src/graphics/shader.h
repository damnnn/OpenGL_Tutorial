#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {
	class Shader
	{
	private:
		int m_ShaderID;
		const char *m_VertexShaderSource;
		const char *m_FragmentShaderSource;
		const char *m_GeometryShaderSource;

	public:
		Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		~Shader();

		void enable() const;
		void disable() const;


		// utility uniform functions
		// ------------------------------------------------------------------------
		void setBool(const std::string &name, bool value) const;

		void setInt(const std::string &name, int value) const;

		void setFloat(const std::string &name, float value) const;

		void setVec2(const std::string &name, const glm::vec2 &value) const;

		void setVec2(const std::string &name, float x, float y) const;

		void setVec3(const std::string &name, const glm::vec3 &value) const;

		void setVec3(const std::string &name, float x, float y, float z) const;

		void setVec4(const std::string &name, const glm::vec4 &value) const;

		void setVec4(const std::string &name, float x, float y, float z, float w);

		void setMat2(const std::string &name, const glm::mat2 &mat) const;

		void setMat3(const std::string &name, const glm::mat3 &mat) const;

		void setMat4(const std::string &name, const glm::mat4 &mat) const;


		// getter
		// ------------------------------------------------------------------------

		int getShaderID() const { return m_ShaderID; }

	private:
		std::string readFile(const char* shaderPath, std::string shaderType);
		void checkCompileErrors(GLuint shader, std::string shaderType);
		int load();
	};
}