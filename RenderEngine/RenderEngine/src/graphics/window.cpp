#include "window.h"

namespace graphics { 

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	// Window Constructor
	//-------------------
	Window::Window(const char * name, int width, int height)
		: m_Title(name), m_Width(width), m_Height(height), 
		m_DeltaTime(0.0f), m_LastTime(0.0f),
		m_Camera(Camera(glm::vec3(0.0f, 0.0f, 3.0f))),
		m_FirstMouse(true), m_MousePosx(800.0f / 2.0f), m_MousePosy(600.0f / 2.0f)

	{
		if (!init()) 
			glfwTerminate();
	}

	// Window destructor
	//-------------------
	Window::~Window() {
		glfwTerminate();
	}

	// Window context initialization
	//-------------------
	bool Window::init()
	{
		// glfw: initialize and configure
		// ------------------------------
		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW" << std::endl;
			return false;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// glfw window creation
		// --------------------
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
		if (m_Window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);

		glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
		glfwSetKeyCallback(m_Window, key_callback);
		glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
		glfwSetCursorPosCallback(m_Window, mouse_position_callback);
		glfwSetScrollCallback(m_Window, scroll_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}
		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);

		return true;
	}

	// Clear relevant buffer
	// ---------------------
	void Window::clear() const
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	void Window::update() const
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			std::cout << "OpenGL error: " << error << std::endl;
		glfwSwapBuffers(m_Window);
		glfwPollEvents();

	}

	// Check if Window should be closed
	// --------------------------------
	bool Window::closed() const
	{
		return glfwWindowShouldClose(m_Window) == 1;
	}

	bool Window::isKeyPressed(unsigned int keycode) const
	{
		if (keycode > MAX_KEYS)
			return false;

		return m_Keys[keycode];
	}

	bool Window::isMouseButtonPressed(unsigned int mousebuttoncode) const
	{
		if (mousebuttoncode > MAX_MOUSE_BUTTONS)
			return false;

		return m_MouseButtons[mousebuttoncode];
	}

	void Window::getMousePosition(double& mx, double& my) const
	{
		mx = m_MousePosx;
		my = m_MousePosy;
	}

	glm::mat4 Window::GetCamViewMatrix()
	{
		return m_Camera.GetViewMatrix();
	}

	float Window::GetCamZoom()
	{
		return m_Camera.GetZoom();
	}

	glm::vec3 Window::GetCamPos()
	{
		return m_Camera.GetPosition();
	}

	void Window::setTime(float currentFrame)
	{
		m_DeltaTime = currentFrame - m_LastTime;
		m_LastTime = currentFrame;
	}

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	// ---------------------------------------------------------------------------------------------------------
	void Window::processInput()
	{

		if (isKeyPressed(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(m_Window, true);

		if (isKeyPressed(GLFW_KEY_W))
			m_Camera.ProcessKeyboard(FORWARD, m_DeltaTime);
		if (isKeyPressed(GLFW_KEY_S))
			m_Camera.ProcessKeyboard(BACKWARD, m_DeltaTime);
		if (isKeyPressed(GLFW_KEY_A))
			m_Camera.ProcessKeyboard(LEFT, m_DeltaTime);
		if (isKeyPressed(GLFW_KEY_D))
			m_Camera.ProcessKeyboard(RIGHT, m_DeltaTime);
			
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_Keys[key] = (action != GLFW_RELEASE);
	}

	void mouse_button_callback(GLFWwindow* window, int mouse_button, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_MouseButtons[mouse_button] = (action != GLFW_RELEASE);
	}

	void mouse_position_callback(GLFWwindow* window, double mx, double my)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);

		if (win->m_FirstMouse)
		{
			win->m_MousePosx = mx;
			win->m_MousePosy = my;
			win->m_FirstMouse = false;
		}

		float xoffset = mx - win->m_MousePosx;
		float yoffset = win->m_MousePosy - my; // reversed since y-coordinates go from bottom to top

		win->m_MousePosx = mx;
		win->m_MousePosy = my;

		win->m_Camera.ProcessMouseMovement(xoffset, yoffset);

	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_Camera.ProcessMouseScroll(yoffset);
	}
}