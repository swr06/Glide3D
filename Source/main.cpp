#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Core/Application/Application.h"
#include "Core/Renderer/CubeRenderer.h"
#include "Core/Renderer/Renderer.h"
#include "Core/FpsCamera.h"
#include "Core/GL_Classes/Fps.h"
#include "Core/ObjectTypes/Cube.h"
#include "Core/Entity/Entity.h"
#include "Core/GL_Classes/Framebuffer.h"
#include "Core/Model Loader/OBJFileLoader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Glide3D;

FPSCamera camera(45, 800.0f / 600.0f, 0.1f, 1000, 0.25f);

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

		else if (e.type == EventTypes::WindowResize)
		{
			if (e.wx > 1 && e.wy > 1) // To prevent division by zero
			{
				camera.SetAspect((float)e.wx / (float)e.wy); // No integer division!
			}
		}

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_V)
		{
			static bool vsync = true;
			vsync = !vsync;
			if (vsync)
			{
				glfwSwapInterval(1);
			}

			else
			{
				glfwSwapInterval(0);
			}

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
	Renderer renderer(app.GetWindow());
	GLFWwindow* window = app.GetWindow();
	GLClasses::Framebuffer FBO(800, 600);
	const float camera_speed = 0.03f;

	CubeObject cube;
	Entity entity(&cube);
	entity.GetTransform().Translate(glm::vec3(4, 0, 0));
	Entity entity1(&cube);
	entity1.GetTransform().Translate(glm::vec3(6, 0, 0));
	Entity entity2(&cube);
	entity2.GetTransform().Translate(glm::vec3(8, 0, 0));
	Entity entity3(&cube);
	entity3.GetTransform().Translate(glm::vec3(10, 0, 0));
	
	camera.SetPosition(glm::vec3(0, 0, -2));

	Object object_1;
	Object object_2;
	Object object_3;

	// Load all the OBJ files
	FileLoader::LoadOBJFile(&object_1, "Resources/teapot.objm");
	FileLoader::LoadOBJFile(&object_2, "Resources/suzanne.objm");
	FileLoader::LoadOBJFile(&object_3, "Resources/12305_backpack_v2_l3.objm");
	object_3.p_AlbedoMap->CreateTexture("Resources/12305_Backpack_v1_diffuse.jpg");

	Entity pot(&object_1);
	Entity suzanne(&object_2);
	Entity backpack(&object_3);

	// Pot Object
	pot.GetTransform().Translate(glm::vec3(15, 0, 0));
	pot.GetTransform().Scale(glm::vec3(0.1f));

	// Suzanne Object
	suzanne.GetTransform().Translate(glm::vec3(20, 0, 0));

	// Backpack
	backpack.GetTransform().Scale(glm::vec3(0.1f, 0.1f, 0.1f));
	backpack.GetTransform().Translate(glm::vec3(24, 0, 0));
	backpack.GetTransform().Rotate(-90.0f);
	backpack.GetTransform().Rotate(-55.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	PointLight light;
	light.m_Position = glm::vec3(15.0f, 1.1f, 13.0f);
	light.m_SpecularStrength = 5;

	renderer.AddPointLight(light);
	//renderer.AddDirectionalLight(light);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(app.GetWindow()))
	{
		renderer.StartRender(&camera);
		app.OnUpdate();
		
		// Clear the framebuffer
		FBO.Bind();
		FBO.OnUpdate(app.GetWidth(), app.GetHeight());

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			glm::vec3 front = -glm::cross(camera.GetRight(), camera.GetUp());
			camera.ApplyAcceleration(front * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			glm::vec3 back = glm::cross(camera.GetRight(), camera.GetUp());
			camera.ApplyAcceleration(back * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ApplyAcceleration(-(camera.GetRight() * camera_speed));
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ApplyAcceleration(camera.GetRight() * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			camera.ApplyAcceleration(camera.GetUp() * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			camera.ApplyAcceleration(-(camera.GetUp() * camera_speed));
		}

		renderer.RenderObjects({ entity, entity1, entity2, entity3 });
		renderer.RenderObjects({ pot });
		renderer.RenderObjects({ suzanne });
		renderer.RenderObjects({ backpack });
		cube_renderer.RenderCube(glm::vec3(15.0f, 1.1f, 13.0f), nullptr, 0, camera.GetViewProjection());
		renderer.RenderFBO(FBO);

		renderer.EndRender();
		camera.OnUpdate();
		camera.ResetAcceleration();
		app.FinishFrame();

		GLClasses::DisplayFrameRate(app.GetWindow(), "Glide3D ");
	}
}