#include "Application.h"

namespace Glide3D
{
	// Event callbacks
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseCallback(GLFWwindow* window, int button, int action, int mods);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	Application::Application()
	{

	}

	Application::~Application()
	{
		glfwDestroyWindow(m_Window);
		Logger::Log("A GLFW window was destroyed!");
	}

	/*
	Initializes the window with the defined width and height 
	*/
	void Application::Initialize()
	{
		glfwInit();
		m_Window = glfwCreateWindow(m_Width, m_Height, "Glide 3D", NULL, NULL);

		if (!m_Window)
		{
			Logger::Log("GLFW Window creation failed! (ret_code : NULL)");
			assert(0);
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Logger::Log("GLAD initialization failed! (ret_code : NULL)");
			assert(0);
			glfwTerminate();
		}

		glfwSetKeyCallback(m_Window, KeyCallback);
		glfwSetMouseButtonCallback(m_Window, MouseCallback);
		glfwSetScrollCallback(m_Window, ScrollCallback);
		glfwSetCursorPosCallback(m_Window, CursorPosCallback);
		glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
		glfwSetWindowUserPointer(m_Window, (void*)&m_EventQueue);

		OnUserCreate(glfwGetTime());

		glClearColor(0.25f, 0.30f, 0.25f, 1.0f);
	}

	/*
	This function should be called every frame.
	It updates the window, polls events and 
	*/
	void Application::OnUpdate()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Poll the events
		glfwPollEvents();
		PollEvents();

		glfwGetFramebufferSize(m_Window, &m_CurrentWidth, &m_CurrentHeight);
	}

	void Application::FinishFrame()
	{
		glfwSwapBuffers(m_Window);
	}

	/*
	Polls the events in the event queue
	*/
	void Application::PollEvents()
	{
		while (!m_EventQueue.empty())
		{
			OnEvent(m_EventQueue.front());
			m_EventQueue.pop();
		}
	}

	/*
	Gets the current time of the program (since glfw was initialized)
	*/
	double Application::GetTime()
	{
		return glfwGetTime();
	}

	/*
	Returns the current frame of the application.
	The frame cound is incremented every frame
	*/
	uint64_t Application::GetCurrentFrame()
	{
		return m_CurrentFrame;
	}

	/*
	Returns the width of the window
	*/
	unsigned int Application::GetWidth()
	{
		return m_CurrentWidth;
	}

	/*
	Returns the height of the window
	*/
	unsigned int Application::GetHeight()
	{
		return m_CurrentHeight;
	}

	// Event handling

	void QueueEvent(const Event& e, GLFWwindow* window)
	{
		void* ptr = glfwGetWindowUserPointer(window);

		if (ptr)
		{
			std::queue<Event>* q = (std::queue<Event>*) ptr;
			q->push(e);
		}
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Event e;

		switch (action)
		{
		case GLFW_PRESS:
			e.type = EventTypes::KeyPress;
			break;

		case GLFW_RELEASE:
			e.type = EventTypes::KeyRelease;
			break;

		default:
			e.type = EventTypes::Undefined;
			break;
		}

		e.window = window;
		e.key = key;
		e.mods = mods;
		e.ts = glfwGetTime();
		QueueEvent(e, window);
	}

	void MouseCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Event e;

		switch (action)
		{
		case GLFW_PRESS:
			e.type = EventTypes::MousePress;
			break;

		case GLFW_RELEASE:
			e.type = EventTypes::MouseRelease;
			break;

		default:
			e.type = EventTypes::Undefined;
			break;
		}

		e.window = window;
		e.button = button;
		e.mods = mods;
		e.ts = glfwGetTime();
		QueueEvent(e, window);
	}

	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Event e;

		e.type = EventTypes::MouseScroll;
		e.window = window;
		e.msx = xoffset;
		e.msy = yoffset;
		e.ts = glfwGetTime();
		QueueEvent(e, window);
	}

	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Event e;

		e.type = EventTypes::MouseMove;
		e.window = window;
		e.mx = xpos;
		e.my = ypos;
		e.ts = glfwGetTime();
		QueueEvent(e, window);
	}

	void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		Event e;

		e.type = EventTypes::WindowResize;
		e.wx = width;
		e.wy = height;
		e.window = window;
		e.ts = glfwGetTime();
		QueueEvent(e, window);
	}
}