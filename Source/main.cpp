#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Core/GL_Classes/VertexBuffer.h"
#include "Core/GL_Classes/IndexBuffer.h"
#include "Core/GL_Classes/VertexArray.h"

int main()
{
	std::cout << "Welcome To Glide 3D\n";

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

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
}