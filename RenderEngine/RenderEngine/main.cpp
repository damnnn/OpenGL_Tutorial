#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "src/graphics/window.h"
#include "src/graphics/camera.h"
#include "src/graphics/texture.h"
#include "src/graphics/shader.h"

#include "src/graphics/buffers/vertexarray.h"
#include "src/graphics/buffers/vertexbuffer.h"
#include "src/graphics/buffers/indexbuffer.h"

using namespace graphics;

// settings
#define SCR_WIDTH 800.0f
#define SCR_HEIGHT 600.0f


int main()
{
	// Init glfw, glad and window context
	// -----------------------------------
	Window window("Learn OpenGL", SCR_WIDTH, SCR_HEIGHT);


	// build and compile our shader program
	// ------------------------------------
	Shader* lightingShader = new Shader("resources/shaders/lighting.vs", "resources/shaders/lighting.fs");
	Shader* lampShader = new Shader("resources/shaders/lamp.vs", "resources/shaders/lamp.fs");
	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float verticesPosition[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f, 
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,

		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f
	};

	float verticesNormal[] = {

		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,
		0.0f,  0.0f, -1.0f,

		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,

		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,
		0.0f, -1.0f,  0.0f,

		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f,
		0.0f,  1.0f,  0.0f
	};

	unsigned int indices[6 * 6];
		
	for (int i = 0; i < 6; i++)
	{
		indices[i * 6 + 0] = 0 + i * 4;
		indices[i * 6 + 1] = 1 + i * 4;
		indices[i * 6 + 2] = 2 + i * 4;

		indices[i * 6 + 3] = 2 + i * 4;
		indices[i * 6 + 4] = 3 + i * 4;
		indices[i * 6 + 5] = 0 + i * 4;
	}

	// lighting
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


	VertexArray* cubeVAO = new VertexArray();
	cubeVAO->addVertexBuffer(new VertexBuffer(verticesPosition, 4 * 6, 3), 0);
	cubeVAO->addVertexBuffer(new VertexBuffer(verticesNormal, 4 * 6, 3), 1);
	VertexArray* lightVAO = new VertexArray();
	lightVAO->addVertexBuffer(new VertexBuffer(verticesPosition, 4 * 6, 3), 0);
	IndexBuffer* IBO = new IndexBuffer(indices, 6 * 6, 3);
	

	// render loop
	// -----------
	while (!window.closed())
	{
		window.setTime(glfwGetTime());
		// input
		// -----
		window.processInput();

		// render
		// ------
		window.clear();


		lightingShader->enable();
		cubeVAO->bind();
		IBO->bind();
		lightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader->setVec3("lightPos", lightPos);
		lightingShader->setVec3("viewPos", window.GetCamPos());
		glm::mat4 view = window.GetCamViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(window.GetCamZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader->setMat4("view", view);
		lightingShader->setMat4("projection", projection);
		glm::mat4 model(1.0f);
		lightingShader->setMat4("model", model);
		glDrawElements(GL_TRIANGLES, IBO->getPrimitiveCount()*IBO->getComponentCount(), GL_UNSIGNED_INT, 0);
		IBO->unbind();
		cubeVAO->unbind();
		lightingShader->disable();



		lampShader->enable();
		lightVAO->bind();
		IBO->bind();
		lampShader->setMat4("view", view);
		lampShader->setMat4("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader->setMat4("model", model);
		glDrawElements(GL_TRIANGLES, IBO->getPrimitiveCount()*IBO->getComponentCount(), GL_UNSIGNED_INT, 0);
		IBO->unbind();
		lightVAO->unbind();
		lampShader->disable();


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		window.update();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	delete lightingShader;
	delete cubeVAO;
	delete lampShader;
	delete lightVAO;
	delete IBO;

	return 0;
}