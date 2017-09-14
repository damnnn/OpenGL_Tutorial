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
#include "src/graphics/shader.h"

#include "src/graphics/buffers/vertexarray.h"
#include "src/graphics/buffers/vertexbuffer.h"
#include "src/graphics/buffers/indexbuffer.h"

#include "src/graphics/mesh.h"
#include "src/graphics/model.h"

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

Shader* ourModelShader = new Shader("resources/shaders/model.vs", "resources/shaders/model.fs");
Shader* lampShader = new Shader("resources/shaders/lamp.vs", "resources/shaders/lamp.fs");

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

// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

VertexArray* lightVAO = new VertexArray();
lightVAO->addVertexBuffer(new VertexBuffer(verticesPosition, 4 * 6, 3), 0);
IndexBuffer* IBO = new IndexBuffer(indices, 6 * 6, 3);

// load models
// -----------
Model* ourModel = new Model("resources/models/nanosuit/nanosuit.obj");


// render loop
// -----------
while (!window.closed())
{
window.setTime(glfwGetTime());

window.processInput();

window.clear();

ourModelShader->enable();
// be sure to activate shader when setting uniforms/drawing objects
ourModelShader->setVec3("viewPos", window.GetCamPos());
ourModelShader->setFloat("material.shininess", 32.0f);

/*
Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
by defining light types as classes and set their values in there, or by using a more efficient uniform approach
by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
*/
// directional light
ourModelShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
ourModelShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
ourModelShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
ourModelShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
// point light 1
ourModelShader->setVec3("pointLights[0].position", pointLightPositions[0]);
ourModelShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
ourModelShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
ourModelShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
ourModelShader->setFloat("pointLights[0].constant", 1.0f);
ourModelShader->setFloat("pointLights[0].linear", 0.09);
ourModelShader->setFloat("pointLights[0].quadratic", 0.032);
// point light 2
ourModelShader->setVec3("pointLights[1].position", pointLightPositions[1]);
ourModelShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
ourModelShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
ourModelShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
ourModelShader->setFloat("pointLights[1].constant", 1.0f);
ourModelShader->setFloat("pointLights[1].linear", 0.29);
ourModelShader->setFloat("pointLights[1].quadratic", 0.032);
// point light 3
ourModelShader->setVec3("pointLights[2].position", pointLightPositions[2]);
ourModelShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
ourModelShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
ourModelShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
ourModelShader->setFloat("pointLights[2].constant", 1.0f);
ourModelShader->setFloat("pointLights[2].linear", 0.29);
ourModelShader->setFloat("pointLights[2].quadratic", 0.032);
// point light 4
ourModelShader->setVec3("pointLights[3].position", pointLightPositions[3]);
ourModelShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
ourModelShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
ourModelShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
ourModelShader->setFloat("pointLights[3].constant", 1.0f);
ourModelShader->setFloat("pointLights[3].linear", 0.29);
ourModelShader->setFloat("pointLights[3].quadratic", 0.032);
// spotLight
ourModelShader->setVec3("spotLight.position", window.GetCamPos());
ourModelShader->setVec3("spotLight.direction", window.GetCamFront());
ourModelShader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
ourModelShader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
ourModelShader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
ourModelShader->setFloat("spotLight.constant", 1.0f);
ourModelShader->setFloat("spotLight.linear", 0.29);
ourModelShader->setFloat("spotLight.quadratic", 0.032);

ourModelShader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
ourModelShader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

// view/projection transformations
glm::mat4 projection = glm::perspective(glm::radians(window.GetCamZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
glm::mat4 view = window.GetCamViewMatrix();
ourModelShader->setMat4("projection", projection);
ourModelShader->setMat4("view", view);

// render the loaded model
glm::mat4 model(1.0);
model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
ourModelShader->setMat4("model", model);

// draw the loaded model
ourModel->Draw(ourModelShader);
ourModelShader->disable();


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

delete ourModel;
delete ourModelShader;
return 0;
}
