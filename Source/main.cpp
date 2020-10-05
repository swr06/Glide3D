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
#include "Core/ObjectTypes/Plane.h"
#include "Core/Model Loader/OBJFileLoader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Glide3D;

FPSCamera camera(45, 800.0f / 600.0f, 0.1, 1000, 0.25f);
bool wireframe = false;

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

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_M)
		{
			wireframe = !wireframe;
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
	Entity entity1(&cube);
	Entity entity2(&cube);
	Entity entity3(&cube);

	entity.GetTransform().Translate(glm::vec3(4, 0, 0));
	entity1.GetTransform().Translate(glm::vec3(6, 0, 0));
	entity2.GetTransform().Translate(glm::vec3(8, 0, 0));
	entity3.GetTransform().Translate(glm::vec3(10, 0, 0));
	
	camera.SetPosition(glm::vec3(0, 0, -2));

	Object object_1;
	Object object_2;
	Object object_3;
	CubeObject object_4;
	CubeObject beacon;
	CubeObject floor_obj;
	Object object_5;

	// Load all the OBJ files
	FileLoader::LoadOBJFile(&object_1, "Resources/teapot.objm");
	FileLoader::LoadOBJFile(&object_2, "Resources/suzanne.objm");
	FileLoader::LoadOBJFile(&object_3, "Resources/12305_backpack_v2_l3.objm");
	FileLoader::LoadOBJFile(&object_5, "Resources/globe-sphere.objm");
	floor_obj.AddTextureMapToMesh("Resources/marble.jpg", TextureType::Albedo);

	object_1.p_CanFacecull = false;
	object_4.AddTextureMapToMesh("Resources/brickwall.jpg", TextureType::Albedo, true);
	object_4.AddTextureMapToMesh("Resources/brickwall_normal.jpg", TextureType::Normal, true);

	Entity pot(&object_1);
	Entity suzanne(&object_2);
	Entity backpack(&object_3);
	Entity brickwall(&object_4);
	Entity sphere(&object_5);
	Entity floor_entity(&floor_obj);

	// Pot Object
	pot.GetTransform().Translate(glm::vec3(15, 0, 0));
	pot.GetTransform().Scale(glm::vec3(0.1f));

	// Suzanne Object
	suzanne.GetTransform().Translate(glm::vec3(20, 0, 0));

	// Backpack
	backpack.GetTransform().Translate(glm::vec3(24, 0, 0));
	backpack.GetTransform().Scale(glm::vec3(0.1f, 0.1f, 0.1f));
	backpack.GetTransform().Rotate(-90.0f);
	backpack.GetTransform().Rotate(-55.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	// The sphere
	sphere.GetTransform().Translate(glm::vec3(1, 0, 0));
	sphere.GetTransform().Scale(glm::vec3(2, 2, 2));

	// Brick Wall
	brickwall.GetTransform().Translate(glm::vec3(27, 0, 0));
	brickwall.GetTransform().Scale(glm::vec3(10,10,0.5f));

	// The floor
	floor_entity.GetTransform().Translate(glm::vec3(-100, -4, -100));
	floor_entity.GetTransform().Scale (glm::vec3(200, 2, 200));
	glm::vec3 light_pos = glm::vec3(0.00349f, -0.59832f, -0.80124f);

	DirectionalLight d_light;

	d_light.m_Direction = light_pos; 
	d_light.m_SpecularStrength = 1.0f;
	d_light.m_SpecularExponent = 32;
	d_light.m_IsBlinn = true;

	PointLight p_light; // -73 8 81 
	p_light.m_Position = glm::vec3(-73 ,8, 81);
	p_light.m_SpecularStrength = 0.1f;
	p_light.m_SpecularExponent = 0.1;
	p_light.m_IsBlinn = true;
	p_light.m_Linear = 0.09f;
	p_light.m_Quadratic = 0.0032f;
	p_light.m_Constant = 0.1f;

	Skybox skybox({
		"Resources/skybox/px.png",
		"Resources/skybox/nx.png",
		"Resources/skybox/py.png",
		"Resources/skybox/ny.png",
		"Resources/skybox/pz.png",
		"Resources/skybox/nz.png"
	});

	renderer.SetEnvironmentMap(skybox);
	renderer.AddDirectionalLight(d_light);
	renderer.AddPointLight(p_light);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	while (!glfwWindowShouldClose(app.GetWindow()))
	{
		app.OnUpdate();
		
		// Clear the framebuffer
		FBO.Bind();
		FBO.OnUpdate(app.GetWidth(), app.GetHeight(), wireframe);

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

		renderer.AddEntityToRenderQueue({ entity, entity1, entity2, entity3 });
		renderer.AddEntityToRenderQueue({ pot });
		renderer.AddEntityToRenderQueue({ suzanne });
		renderer.AddEntityToRenderQueue({ backpack });
		renderer.AddEntityToRenderQueue({ sphere });
		renderer.AddEntityToRenderQueue({ brickwall });
		renderer.AddEntityToRenderQueue({ floor_entity });
		renderer.Render(&camera);
		renderer.RenderFBO(FBO);

		camera.OnUpdate();
		camera.ResetAcceleration();
		app.FinishFrame();

		GLClasses::DisplayFrameRate(app.GetWindow(), "Glide3D ");
	}
}