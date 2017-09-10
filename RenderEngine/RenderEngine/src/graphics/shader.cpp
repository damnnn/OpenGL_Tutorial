#include "shader.h"

namespace graphics {

	// Shader Constructor
	//-------------------
	Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
	{
		std::string vertexShaderStr = readFile(vertexPath, "VERTEX");
		m_VertexShaderSource = vertexShaderStr.c_str();
		std::string fragmentShaderStr = readFile(fragmentPath, "FRAGMENT");
		m_FragmentShaderSource = fragmentShaderStr.c_str();
		if (geometryPath != nullptr) {
			std::string geometryShaderStr = readFile(geometryPath, "GEOMETRY");
			m_GeometryShaderSource = geometryShaderStr.c_str();
		}
		else {
			m_GeometryShaderSource = nullptr;
		}
		m_ShaderID = load();
	}

	// Shader destructor
	//-------------------
	Shader::~Shader() {
		glDeleteProgram(m_ShaderID);
	}

	std::string  Shader::readFile(const char* shaderPath, std::string shaderType)
	{
		std::string shaderCode;
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			shaderFile.open(shaderPath);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			shaderCode = shaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::" << shaderType << "FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		return shaderCode;
	}

	// build and compile our shader program
	// ------------------------------------
	int Shader::load()
	{
		
		int vertexShader, fragmentShader, geometryShader, shaderProgram;
		// vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &m_VertexShaderSource, NULL);
		glCompileShader(vertexShader);
		checkCompileErrors(vertexShader, "VERTEX");

		// fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &m_FragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		checkCompileErrors(fragmentShader, "FRAGMENT");

		// geometry shader
		if (m_GeometryShaderSource != nullptr) {
			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &m_GeometryShaderSource, NULL);
			glCompileShader(geometryShader);
			checkCompileErrors(geometryShader, "GEOMETRY");
		}

		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		if (m_GeometryShaderSource != nullptr)
			glAttachShader(shaderProgram, geometryShader);
		glLinkProgram(shaderProgram);
		checkCompileErrors(shaderProgram, "PROGRAM");

		// delete shaders
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (m_GeometryShaderSource != nullptr)
			glDeleteShader(geometryShader);

		// return linked shader program
		return shaderProgram;
	}

	void Shader::checkCompileErrors(GLuint shader, std::string shaderType)
	{
		GLint success;
		GLchar infoLog[1024];
		if (shaderType != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << shaderType << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << shaderType << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	void Shader::enable() const
	{
		glUseProgram(m_ShaderID);
	}

	// Check if Shader should be closed
	// --------------------------------
	void Shader::disable() const
	{
		glUseProgram(0);
	}

	// utility uniform functions
	// ------------------------------------------------------------------------
	void Shader::setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void Shader::setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void Shader::setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ShaderID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_ShaderID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_ShaderID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, &value[0]);
	}
	void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(m_ShaderID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ShaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


}