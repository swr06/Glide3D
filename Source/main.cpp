#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Core/GL_Classes/VertexBuffer.h"
#include "Core/GL_Classes/IndexBuffer.h"
#include "Core/GL_Classes/VertexArray.h"
#include "Core/Application/Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Glide3D;

class MyApp : public Application
{
public:
	void OnUserCreate(double ts) override
	{

	}

	void OnUserUpdate(double ts) override
	{

	}

	void OnEvent(Event e) override
	{
		if (e.type == EventTypes::KeyPress)
		{
			std::cout << "\nKEY PRESS!";
		}
	}
};

int main()
{
	MyApp app;
	app.Initialize();

	while (!glfwWindowShouldClose(app.GetWindow()))
	{
		app.OnUpdate();
	}
}