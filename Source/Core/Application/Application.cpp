#include "Application.h"
#include <glad/gl/glext.h> // The opengl extensions

namespace Glide3D
{
	// Event callbacks
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseCallback(GLFWwindow* window, int button, int action, int mods);
	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	/*
	The OpenGL Debug callback
	*/
	void APIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar* msg, const void* data)
	{
		char* _source;
		char* _type;
		char* _severity;

		switch (source) {
		case GL_DEBUG_SOURCE_API:
			_source = (char*)"API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			_source = (char*)"WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			_source = (char*)"SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			_source = (char*)"THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			_source = (char*)"APPLICATION";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			_source = (char*)"UNKNOWN";
			break;

		default:
			_source = (char*)"UNKNOWN";
			break;
		}

		switch (type) 
		{
		case GL_DEBUG_TYPE_ERROR:
			_type = (char*)"ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			_type = (char*)"DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			_type = (char*)"UDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			_type = (char*)"PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			_type = (char*)"PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			_type = (char*)"OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			_type = (char*)"MARKER";
			break;

		default:
			_type = (char*)"UNKNOWN";
			break;
		}

		switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			_severity = (char*)"HIGH";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			_severity = (char*)"MEDIUM";
			break;

		case GL_DEBUG_SEVERITY_LOW:
			_severity = (char*)"LOW";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			_severity = (char*)"NOTIFICATION";
			break;

		default:
			_severity = (char*)"UNKNOWN";
			break;
		}

		printf("%d: %s of %s severity, raised from %s: %s\n",
			id, _type, _severity, _source, msg);
	}

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
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

		// Use the latest ogl version
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
		glfwGetFramebufferSize(m_Window, &m_CurrentWidth, &m_CurrentHeight);

#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
		glDebugMessageCallback(gl_debug_callback, nullptr);
#endif

		//glClearColor(0.25f, 0.30f, 0.25f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		char* renderer = (char*)glGetString(GL_RENDERER);
		char* vendor = (char*)glGetString(GL_VENDOR);
		char* version = (char*)glGetString(GL_VERSION);

		std::cout << "-----------  The Glide 3D Game Engine -----------\n";
		std::cout << "\tRENDERER : " << renderer << "\n";
		std::cout << "\tVENDOR : " << vendor << "\n";
		std::cout << "\tVERSION : " << version << "\n";

		if (glfwExtensionSupported("GL_ARB_debug_output"))
		{
			std::cout << "\tDEBUG : THE OPENGL SYNCHRONOUS DEBUG EXTENSION IS AVAILABLE\n";
		}

		else
		{
			std::cout << "\tDEBUG : THE OPENGL SYNCHRONOUS DEBUG EXTENSION IS UN AVAILABLE\n";
		}

		std::cout << "------------------------------------------------\n\n";
		return;
	}

	/*
	This function should be called every frame.
	It updates the window, polls events 
	*/
	void Application::OnUpdate()
	{
		glfwGetFramebufferSize(m_Window, &m_CurrentWidth, &m_CurrentHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, m_CurrentWidth, m_CurrentHeight);

		// Poll the events
		glfwPollEvents();
		PollEvents();
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