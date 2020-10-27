/*
The Glide3D Game Engine by Samuel Rasquinha
*/

/* Contributors
kiroma 
hazurl
*/

#include "Core/GlideEngine.h"

using namespace Glide3D;

FPSCamera camera(70, 800.0f / 600.0f, 0.1f, 1000.0f, 0.25f);
bool wireframe = false;
bool cursor_locked = true;
float exposure = 0.1f;
float camera_speed = 0.02f;
std::stringstream camera_props;

class MyApp : public Application
{
public:
	void OnUserCreate(double ts) override
	{

	}

	void OnUserUpdate(double ts) override
	{

	}

	void OnImguiRender(double ts) override
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoScrollbar;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoBackground;

		if (ImGui::Begin("Settings", false, window_flags))
		{
			const glm::vec3& position = camera.GetPosition();
			const glm::vec3& front = camera.GetFront();

			ImGui::SliderFloat("Exposure", &exposure, 0.0f, 5.0f);
			ImGui::SliderFloat("Speed", &camera_speed, 0.01f, 2.0f);
			ImGui::Text("Camera Position : %f, %f, %f", position.x, position.y, position.z);
			ImGui::Text("Camera Front : %f, %f, %f", front.x, front.y, front.z);
			ImGui::End();
		}   
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

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_ESCAPE)
		{
			cursor_locked = !cursor_locked;
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

	Glide3D::Renderer &renderer = app.GetRenderer();
	GLFWwindow* window = app.GetWindow();
	GLClasses::Framebuffer FBO(800, 600, true);

	glfwMaximizeWindow(window);

	camera.SetPosition(glm::vec3(0, 5, 0));

	Object Sponza_obj;

	FileLoader::LoadOBJFile(&Sponza_obj, "Resources/models/sponza/quintessentials.model");
	//FileLoader::LoadOBJFile(&Sponza_obj, "Resources/models/dabrovic_sponza/sponza.model");

	Entity sponza(&Sponza_obj);

	sponza.GetTransform().Translate(glm::vec3(15, 0, 0));
	sponza.GetTransform().Scale(glm::vec3(0.1f));

	glm::vec3 light_dir = glm::vec3(-0.71f, -0.69f, -0.01f);

	DirectionalLight d_light({ -300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 300.0f }, {4096,4096 });

	d_light.m_Direction = light_dir;
	d_light.m_ShadowPosition = glm::vec3(91, 79, -7); 
	d_light.m_SpecularStrength = 2.0f;
	d_light.m_SpecularExponent = 32;
	d_light.m_IsBlinn = true;
	d_light.m_UpdateRate = 1;


	Skybox skybox({
		"Resources/skybox/px.png",
		"Resources/skybox/nx.png",
		"Resources/skybox/py.png",
		"Resources/skybox/ny.png",
		"Resources/skybox/pz.png",
		"Resources/skybox/nz.png"
		});

	renderer.SetEnvironmentMap(skybox);
	renderer.AddDirectionalLight(&d_light);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	renderer.AddEntity(&sponza);

	while (!glfwWindowShouldClose(app.GetWindow()))
	{
		d_light.m_ShadowPosition = camera.GetPosition();
		d_light.m_Direction = camera.GetFront();
		FBO.SetExposure(exposure);

		app.OnUpdate();
		app.SetCursorLocked(cursor_locked);

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

		renderer.Render(&camera, FBO);  
		renderer.RenderFBO(FBO);
		camera.OnUpdate();
		camera.ResetAcceleration();
		app.FinishFrame(); 

		GLClasses::DisplayFrameRate(app.GetWindow(), "Glide3D ");
	}
} 