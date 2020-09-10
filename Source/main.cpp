#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Core/GL_Classes/VertexBuffer.h"
#include "Core/GL_Classes/IndexBuffer.h"
#include "Core/GL_Classes/VertexArray.h"

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Glide 3D", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		std::cin.get();

		return -1;
	}

	std::cin.get();
	std::cin.get();
	glfwDestroyWindow(window);
}