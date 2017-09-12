#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace graphics {


#define MAX_KEYS 1024
#define MAX_MOUSE_BUTTONS 32

	class Window
	{
	private:
		const char *m_Title;
		unsigned int m_Width, m_Height;
		GLFWwindow *m_Window;
		Camera m_Camera;
		bool m_Closed;

		// timing
		float m_DeltaTime;	// time between current frame and last frame
		float m_LastTime;


		bool m_Keys[MAX_KEYS];
		bool m_MouseButtons[MAX_MOUSE_BUTTONS];


		bool m_FirstMouse;
		float m_MousePosx, m_MousePosy;

	public:
		Window(const char *name, int width, int height);
		~Window(); 

		void processInput();

		void clear() const;
		void update() const;
		bool closed() const;

		void setTime(float currentFrame);

		glm::mat4 GetCamViewMatrix();
		float GetCamZoom();
		glm::vec3 GetCamPos();
		glm::vec3 GetCamFront();


		inline int getWidth() const { return m_Width; }
		inline int getHeight() const { return m_Height; }
		void getMousePosition(double& mx, double& my) const;

		bool isKeyPressed(unsigned int keycode) const;
		bool isMouseButtonPressed(unsigned int buttoncode) const;

	private:
		bool init();
		friend static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend static void mouse_button_callback(GLFWwindow* window, int mousebutton, int action, int mods);
		friend static void mouse_position_callback(GLFWwindow* window, double mx, double my);
		friend static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	};
}