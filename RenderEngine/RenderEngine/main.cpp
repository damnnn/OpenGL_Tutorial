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

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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


	float verticesTextureCoord[] = {
		  // texture coords
		  0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		  0.0f,  1.0f,

		  0.0f,  0.0f,
		 1.0f,  0.0f,
		 1.0f,  1.0f,
		  0.0f,  1.0f,

		  1.0f,  0.0f,
		  1.0f,  1.0f,
		  0.0f,  1.0f,
		  0.0f,  0.0f,

		 1.0f,  0.0f,
		 1.0f,  1.0f,
		 0.0f,  1.0f,
		 0.0f,  0.0f,

		  0.0f,  1.0f,
		 1.0f,  1.0f,
		 1.0f,  0.0f,
		  0.0f,  0.0f,

		  0.0f,  1.0f,
		 1.0f,  1.0f,
		 1.0f,  0.0f,
		  0.0f,  0.0f
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

	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};


	VertexArray* cubeVAO = new VertexArray();
	cubeVAO->addVertexBuffer(new VertexBuffer(verticesPosition, 4 * 6, 3), 0);
	cubeVAO->addVertexBuffer(new VertexBuffer(verticesNormal, 4 * 6, 3), 1);
	cubeVAO->addVertexBuffer(new VertexBuffer(verticesTextureCoord, 4 * 6, 2), 2);
	VertexArray* lightVAO = new VertexArray();
	lightVAO->addVertexBuffer(new VertexBuffer(verticesPosition, 4 * 6, 3), 0);

	IndexBuffer* IBO = new IndexBuffer(indices, 6 * 6, 3);
	

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	Texture* diffuseMap = new Texture("resources/textures/container2.png");
	Texture* specularMap = new Texture("resources/textures/container2_specular.png");

	// shader configuration
	// --------------------
	lightingShader->enable();
	lightingShader->setInt("material.diffuse", 0);
	lightingShader->setInt("material.specular", 1);
	lightingShader->disable();

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

		diffuseMap->bind(0);
		specularMap->bind(1);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader->setVec3("viewPos", window.GetCamPos());
		lightingShader->setFloat("material.shininess", 32.0f);

		/*
		Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		lightingShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightingShader->setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader->setFloat("pointLights[0].constant", 1.0f);
		lightingShader->setFloat("pointLights[0].linear", 0.09);
		lightingShader->setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader->setFloat("pointLights[1].constant", 1.0f);
		lightingShader->setFloat("pointLights[1].linear", 0.09);
		lightingShader->setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		lightingShader->setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader->setFloat("pointLights[2].constant", 1.0f);
		lightingShader->setFloat("pointLights[2].linear", 0.09);
		lightingShader->setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		lightingShader->setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader->setFloat("pointLights[3].constant", 1.0f);
		lightingShader->setFloat("pointLights[3].linear", 0.09);
		lightingShader->setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		lightingShader->setVec3("spotLight.position", window.GetCamPos());
		lightingShader->setVec3("spotLight.direction", window.GetCamFront());
		lightingShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader->setFloat("spotLight.constant", 1.0f);
		lightingShader->setFloat("spotLight.linear", 0.09);
		lightingShader->setFloat("spotLight.quadratic", 0.032);
		lightingShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 view = window.GetCamViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(window.GetCamZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		lightingShader->setMat4("projection", projection);
		lightingShader->setMat4("view", view);


		// world transformation
		glm::mat4 model(1.0f);
		lightingShader->setMat4("model", model);

		// render containers
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader->setMat4("model", model);
			glDrawElements(GL_TRIANGLES, IBO->getPrimitiveCount()*IBO->getComponentCount(), GL_UNSIGNED_INT, 0);
		}

		diffuseMap->unbind();
		specularMap->unbind();
		IBO->unbind();
		cubeVAO->unbind();
		lightingShader->disable();

		// also draw the lamp object(s)
		lampShader->enable();
		lightVAO->bind();
		IBO->bind();
		lampShader->setMat4("view", view);
		lampShader->setMat4("projection", projection);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader->setMat4("model", model);
			glDrawElements(GL_TRIANGLES, IBO->getPrimitiveCount()*IBO->getComponentCount(), GL_UNSIGNED_INT, 0);
		}


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