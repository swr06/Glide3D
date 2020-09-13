#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Core/Application/Application.h"
#include "Core/Renderer/CubeRenderer.h"
#include "Core/Renderer/Renderer.h"
#include "Core/FpsCamera.h"
#include "Core/ObjectTypes/Cube.h"
#include "Core/Entity/Entity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Glide3D;

FPSCamera camera(45, 800.0f / 600.0f, 0, 1000, 0.25f);

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
		if (e.type == EventTypes::MouseMove)
		{
			camera.UpdateOnMouseMovement(e.mx, e.my);
		}
	}

};

MyApp app;

int main()
{
	app.Initialize();
	glfwSwapInterval(1);
	glfwSetInputMode(app.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	CubeRenderer cube_renderer;
	Renderer renderer;
	GLFWwindow* window = app.GetWindow();
	const float camera_speed = 0.1f;

	CubeObject cube;
	Entity entity(&cube);
	entity.GetTransform().Translate(glm::vec3(4, 0, 0));
	Entity entity1(&cube);
	entity1.GetTransform().Translate(glm::vec3(6, 0, 0));
	Entity entity2(&cube);
	entity2.GetTransform().Translate(glm::vec3(8, 0, 0));
	Entity entity3(&cube);
	entity3.GetTransform().Translate(glm::vec3(10, 0, 0));
	
	while (!glfwWindowShouldClose(app.GetWindow()))
	{
		app.OnUpdate();

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			// Take the cross product of the camera's right and up.
			glm::vec3 front = -glm::cross(camera.GetRight(), camera.GetUp());
			camera.ChangePosition(front * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			glm::vec3 back = glm::cross(camera.GetRight(), camera.GetUp());
			camera.ChangePosition(back * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ChangePosition(-(camera.GetRight() * camera_speed));
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ChangePosition(camera.GetRight() * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			camera.ChangePosition(camera.GetUp() * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			camera.ChangePosition(-(camera.GetUp() * camera_speed));
		}

		cube_renderer.RenderCube(glm::vec3(0, 0, 0), nullptr, 0, camera.GetProjectionMatrix(), camera.GetViewMatrix(), nullptr);
		renderer.RenderObjects({ entity, entity1, entity2, entity3 }, &camera);

		camera.OnUpdate();
		app.FinishFrame();
	}
}