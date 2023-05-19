#include "Window.h"
#include "Input/Input.h"
#include "iostream"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);



Window::Window(std::string title, int width, int height)
{

	m_Title = title;
	m_Width = width;
	m_Height = height;
	// glfw: initialize and configure
	// ------------------------------
	int isError = glfwInit();
	if (isError == GLFW_FALSE) {
		fprintf(stderr, "Failed to init GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	m_Window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (m_Window == nullptr)
	{
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_Window);


	glfwSetKeyCallback(m_Window, key_callback);
	glfwSetCursorPosCallback(m_Window, cursor_position_callback);
	glfwSetScrollCallback(m_Window, scroll_callback);
	glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

	//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Input::Resize(width, height);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr,  "Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}

	auto mess = glGetString(GL_RENDERER);
	std::cout << mess << std::endl;
}
Window::~Window()
{
	glfwTerminate();
}

bool Window::Update()
{
	if (glfwWindowShouldClose(m_Window)) return false;
	Input::Reset();
	glfwSwapBuffers(m_Window);
	glfwSwapInterval(0);
	glfwPollEvents();
	return true;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		Input::KeyPressed(key);
	if (action == GLFW_RELEASE)
		Input::KeyReleased(key);
}


static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	float xpos = static_cast<float>(xPos);
	float ypos = static_cast<float>(yPos);

	if (Input::isFirstMove)
	{
		Input::lastMousePosition = std::pair<float, float>(xpos, ypos);
		Input::isFirstMove = false;
	}

	float xoffset = xpos - Input::lastMousePosition.first;
	float yoffset = Input::lastMousePosition.second - ypos;

	Input::lastMousePosition = std::pair<float, float>(xpos, ypos);

	Input::MouseMoved(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::Scrolled(static_cast<float>(yoffset));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		Input::RPMPressed();
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		Input::RPMReleased();
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		Input::LPMPressed();
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		Input::LPMReleased();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Input::Resize(width, height);
}