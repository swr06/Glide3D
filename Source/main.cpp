/*
The Glide3D Game Engine by Samuel Rasquinha
*/

/* Contributors
kiroma
hazurl
*/

#include "Core/GlideEngine.h"

using namespace Glide3D;

FPSCamera camera(90, 800.0f / 600.0f, 0.1f, 1000.0f, 0.25f);
bool wireframe = false;
bool cursor_locked = true;
bool pbr = false;
float exposure = 0.1f;
float camera_speed = 0.02f;
std::stringstream camera_props;
TonemappingType tonetype = TonemappingType::ACES;

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

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_F2)
		{
			RecompileShaders();
		}

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_F1)
		{
			pbr = !pbr;
		}

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_ESCAPE)
		{
			cursor_locked = !cursor_locked;
		}

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_M)
		{
			wireframe = !wireframe;
		}

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_E)
		{
			tonetype = (TonemappingType)((int)tonetype + 1);

			if (tonetype > TonemappingType::Unreal)
			{
				tonetype = TonemappingType::ACES;
			}
		}
	}

};

MyApp app;

int main()
{
	app.Initialize();
	glfwSwapInterval(1);

	Glide3D::Renderer& renderer = app.GetRenderer();
	GLFWwindow* window = app.GetWindow();
	GLClasses::Framebuffer FBO(800, 600, true);

	glfwMaximizeWindow(window);

	camera.SetPosition(glm::vec3(0, 5, 0));

	Object object;

	FileLoader::LoadOBJFile(&object, "Resources/models/sponza/quintessentials.model");

	//FileLoader::LoadOBJFile(&object, "Resources/sphere.model");
	//
	//object.AddTextureMapToMesh("Resources/pbr/mat2/Albedo.png", TextureType::Albedo);
	//object.AddTextureMapToMesh("Resources/pbr/mat2/Normal.png", TextureType::Normal);
	//object.AddTextureMapToMesh("Resources/pbr/mat2/Metallic.png", TextureType::Metalness);
	//object.AddTextureMapToMesh("Resources/pbr/mat2/Roughness.png", TextureType::Roughness);
	//object.AddTextureMapToMesh("Resources/pbr/mat2/AO.png", TextureType::AO);

	Entity entity1(&object);

	entity1.GetTransform().Scale(glm::vec3(0.1f));
	entity1.GetTransform().Translate(glm::vec3(0, 0, 0));

	glm::vec3 light_dir = glm::vec3(-0.71f, -0.69f, -0.01f);

	DirectionalLight d_light({ -300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 300.0f }, { 4096,4096 });

	d_light.m_Direction = light_dir;
	d_light.m_ShadowPosition = glm::vec3(91, 79, -7);
	d_light.m_SpecularColor = glm::vec3(0.36745f, 0.21843f, 0.08627f);
	d_light.m_SpecularStrength = 0.4f;
	d_light.m_SpecularExponent = 64;
	d_light.m_IsBlinn = true;
	d_light.m_UpdateRate = 0;

	/*PointLight p_light(512);
	p_light.m_Position = glm::vec3(28, 14, -37);
	p_light.m_SpecularStrength = 4.0f;
	p_light.m_Linear = 0.0014f;
	p_light.m_Quadratic = 0.000007f;
	p_light.m_ShadowMapUpdateRate = 0;*/

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

	renderer.AddEntity(&entity1);

	while (!glfwWindowShouldClose(app.GetWindow()))
	{
		renderer.SetUsePBR(pbr);
		FBO.SetExposure(exposure);
		FBO.p_TonemappingType = tonetype;

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