#pragma once

#include "../Macros.h"

#ifdef GLIDE3D_WIN32
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// Include Stuff
#include "../Model Loader/OBJFileLoader.h"
#include "../Application/Logger.h"
#include "../Renderer/Renderer.h"
#include "../Entity/Entity.h"
#include "../Object/Object.h"
#include "../ObjectTypes/Cube.h"
#include "../ObjectTypes/Cone.h"
#include "../ObjectTypes/Sphere.h"
#include "../ObjectTypes/Plane.h"

namespace Glide3D
{
	class Editor
	{
	public:

		Editor(std::shared_ptr<Renderer> renderer, GLFWwindow* window);
		~Editor() { ; }
		void OnUpdate();

	private:
		void InitializeEditor();
		std::vector<std::unique_ptr<Object>> m_Objects;
		std::shared_ptr<Renderer> m_Renderer;
		GLFWwindow* m_Window;
	};
}